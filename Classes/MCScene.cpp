//
//  MCScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-15.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCSceneController.h"
#include "MCScene.h"
#include "MCControllerLayer.h"
#include "MCDetail.h"
#include "MCEntrance.h"
#include "MCAStar.h"

#pragma mark *** MCSceneContextManager ***

const char *kMCDetailButtonPressedFilepath = "UI/d_details_pressed.png";
const char *kMCDetailButtonFilepath = "UI/d_details.png";

static MCSceneContextManager *__shared_scene_context_manager = NULL;

MCSceneContextManager::MCSceneContextManager()
{
    contextStack_ = CCArray::create();
    contextStack_->retain();
}

MCSceneContextManager::~MCSceneContextManager()
{
    CC_SAFE_RELEASE(contextStack_);
}

MCSceneContextManager *
MCSceneContextManager::sharedSceneContextManager()
{
    if (__shared_scene_context_manager == NULL) {
        __shared_scene_context_manager = new MCSceneContextManager;
    }
    
    return __shared_scene_context_manager;
}

void
MCSceneContextManager::pushContext(MCSceneContext *aContext)
{
    contextStack_->addObject(aContext);
}

void MCSceneContextManager::popContext()
{
    contextStack_->removeLastObject();
}

MCSceneContext *
MCSceneContextManager::currentContext()
{
    return (MCSceneContext *) contextStack_->lastObject();
}

#pragma mark -
#pragma mark *** MCScene ***

MCScene::~MCScene()
{
    CC_SAFE_RELEASE(aStar_);
}

/* 从场景包加载场景初始化 */
bool
MCScene::initWithScenePackage(MCScenePackage *aPackage)
{
    if (CCScene::init() && aPackage) {
        scenePackage_ = aPackage;
        background_ = MCBackgroundLayer::create(aPackage->getTMXTiledMapPath()->getCString(),
                                                aPackage->getBackgroundMusicPath()->getCString());
        addChild(background_);
        
        objects_ = MCObjectLayer::create(aPackage->getScenePackageType());
        objects_->setSceneDelegate(this);
        objects_->setTMXTiledMap(background_->getMap());
        objects_->loadEntrancesFromScenePackage(aPackage);
        addChild(objects_);
        
        aStar_ = new MCAStar;
        aStar_->init(background_->getMap());
        
        installController();
        
        /* entrances */
        entrances_ = aPackage->getScenes();
        
        /* detail menu */
        CCMenu *detailMenu = CCMenu::create();
        addChild(detailMenu);
        CCSize winSize = CCDirectorGetWindowsSize();
        CCMenuItemImage *detailMenuItem = CCMenuItemImage::create(kMCDetailButtonFilepath,
                                                                  kMCDetailButtonPressedFilepath);
        detailMenuItem->setTarget(this, menu_selector(MCScene::showDetail));
        detailMenu->addChild(detailMenuItem);
        detailMenu->alignItemsHorizontally();
        CCSize buttonSize = detailMenuItem->getContentSize();
        detailMenu->setPosition(ccp(winSize.width - buttonSize.width,
                                    winSize.height - buttonSize.height));
        
        detail_ = MCDetail::create();
        detail_->initPosition();
        addChild(detail_);
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                      callfuncO_selector(MCScene::detailDidHide),
                                                                      kMCDetailDidHideNotification,
                                                                      NULL);
        
        return true;
    }
    
    return false;
}

/* 从场景包加载场景 */
MCScene *
MCScene::createWithScenePackage(MCScenePackage *aPackage)
{
    MCScene *scene = new MCScene;
    
    if (scene && scene->initWithScenePackage(aPackage)) {
        scene->autorelease();
    } else {
        CC_SAFE_DELETE(scene);
        scene = NULL;
    }
    
    return scene;
}

CCPoint
MCScene::getMapOffset() const
{
    return background_->getMap()->getPosition();
}

CCTMXTiledMap *
MCScene::getMap() const
{
    return background_->getMap();
}

void
MCScene::onEnter()
{
    MCSceneContext *context = new MCSceneContext;
    context->scene_ = this;
    MCSceneContextManager::sharedSceneContextManager()->pushContext(context);
    CCScene::onEnter();
    /* 预加载场景 */
    schedule(schedule_selector(MCScene::update));
    //warning: for debug
    return;
    if (viewport_ == NULL) {
        viewport_ = MCViewportLayer::create();
        addChild(viewport_);
        viewport_->loadObjects(objects_->objects_);
        viewport_->loadSemis(objects_->semiTransparents_);
        viewport_->loadBarriers(objects_->barriers_);
        viewport_->map = background_->getMap();
    }
}

void
MCScene::onExit()
{
    unschedule(schedule_selector(MCScene::update));
    
    CCScene::onExit();
    
    MCSceneContext *context = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCSceneContextManager::sharedSceneContextManager()->popContext();
    CC_SAFE_RELEASE(context);
}

void
MCScene::update(float dt)
{
}

/**
 * 安装触发器
 */
void
MCScene::installTrigger(MCTrigger *aTrigger)
{
    CCObject *object = (CCObject *)aTrigger;
    if (! triggers_->containsObject(object)) {
        triggers_->addObject(object);
    }
}

/**
 * 卸载触发器
 */
void
MCScene::uninstallTrigger(MCTrigger *aTrigger)
{
    CCObject *object = (CCObject *)aTrigger;
    triggers_->removeObject(object);
}

/**
 * 移动到场景
 * aSceneId(in): 场景ID
 * anEntranceName(in): 场景入口名
 */
void
MCScene::gotoScene(mc_object_id_t aSceneId, const char *anEntranceName, bool isInternal)
{
    MCSceneController *sceneController = MCSceneController::sharedSceneController();
    sceneController->pushExpectedScene(aSceneId,
                                       anEntranceName,
                                       isInternal ? MCPushScene : MCReplaceScene);
    sceneController->requestChangingScene();
}

/**
 * 从内部场景(比如房子、商店)出去
 */
void
MCScene::goOut()
{
    if (isInternalScene_) {
        MCSceneController *sceneController = MCSceneController::sharedSceneController();
        sceneController->pushExpectedScene(NULL, NULL, MCPopScene);
        sceneController->requestChangingScene();
    }
}

bool
MCScene::hasEntrance(const char *anEntranceName)
{
    return entrances_->objectForKey(anEntranceName) ? true : false;
}

void
MCScene::showDetail()
{
    controller_->setEnable(false);
    detail_->show();
}

void
MCScene::detailDidHide()
{
    controller_->setEnable(true);
}
