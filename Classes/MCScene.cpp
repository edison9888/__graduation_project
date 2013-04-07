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
#include "MCCamera.h"
#include "MCControllerLayer.h"
#include "MCDetail.h"
#include "MCEntrance.h"
#include "MCAStar.h"
#include "MCTeam.h"
#include "MCFlagManager.h"

#pragma mark *** MCSceneContextManager ***

static const char *kMCDetailButtonPressedFilepath = "UI/d_details_pressed.png";
static const char *kMCDetailButtonFilepath = "UI/d_details.png";

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
    CC_SAFE_DELETE(sceneCamera_);
    CC_SAFE_RELEASE(aStar_);
}

/* 从场景包加载场景初始化 */
bool
MCScene::initWithScenePackage(MCScenePackage *aPackage)
{
    if (CCScene::init() && aPackage) {
        CCSize winSize = CCDirectorGetWindowsSize();
        scenePackage_ = aPackage;
        background_ = MCBackgroundLayer::create(aPackage->getTMXTiledMapPath()->getCString(),
                                                aPackage->getBackgroundMusicPath()->getCString());
        addChild(background_);
        CCSize sceneSize = background_->getSceneSize();
        defaultLocation_ = ccp(sceneSize.width > winSize.width
                               ? 0
                               : (winSize.width - sceneSize.width) / 2,
                               sceneSize.height > winSize.height
                               ? 0
                               : (winSize.height - sceneSize.height) / 2);
        isInternalScene_ = aPackage->isInternalScene();
        
        sceneCamera_ = new MCCamera;
        sceneCamera_->setSceneDelegate(this);
        
        MCScenePackageType scenePackageType = aPackage->getScenePackageType();
        if (scenePackageType == MCBattleFieldScenePackage
            && !MCFlagManager::sharedFlagManager()->isTaskStarted()) {
            scenePackageType = MCGameScenePackage;
        }
        objects_ = MCObjectLayer::create(scenePackageType);
        objects_->setSceneDelegate(this);
        objects_->setTMXTiledMap(background_->getMap());
        objects_->loadEntrancesFromScenePackage(aPackage);
        addChild(objects_);
        
        aStar_ = new MCAStar;
        aStar_->init(background_->getMap());
        aStar_->setBarriers(objects_->getbarriers());
        
        installController();
        
        /* entrances */
        entrances_ = aPackage->getScenes();
        
        /* detail menu */
        CCMenu *detailMenu = CCMenu::create();
        addChild(detailMenu);
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

/**
 * 获取场景大小
 */
CCSize &
MCScene::getSceneSize() const
{
    return objects_->mapSize_;
}

void
MCScene::onEnter()
{
    MCSceneContext *context = new MCSceneContext;
    context->scene_ = this;
    MCSceneContextManager::sharedSceneContextManager()->pushContext(context);
    
    CCScene::onEnter();
    /* 已加载玩对象了现在 */
    background_->loadEnemies(objects_->objects());
    background_->loadTeam(MCTeam::sharedTeam());
    
    /* 设置地图位置 */
    sceneCamera_->restore();
    sceneCamera_->locate();
    sceneCamera_->focusHero();
    
    schedule(schedule_selector(MCScene::update));
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
    CCScene::update(dt);
//    CCPointLog(sceneCamera_->getViewport().origin);
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
    /* 一切replace */
    sceneController->pushExpectedScene(aSceneId,
                                       anEntranceName,
                                       MCReplaceScene);
    sceneController->requestChangingScene();
}

/**
 * 从内部场景(比如房子、商店)出去
 */
void
MCScene::goOut()
{
//    if (isInternalScene_) {
//        MCSceneController *sceneController = MCSceneController::sharedSceneController();
//        sceneController->pushExpectedScene(NULL, NULL, MCPopScene);
//        sceneController->requestChangingScene();
//    }
}

void
MCScene::moveSceneToLocation(const CCPoint &aLocation, bool adjusted)
{
    CCPoint offset = ccpSub(aLocation, getMapOffset());
//    sceneCamera_->translate
//    (objects_->setSceneOffset(offset, adjusted));
    objects_->setSceneOffset(offset);
    CCPointLog(sceneCamera_->getViewport().origin);
}

bool
MCScene::hasEntrance(const char *anEntranceName)
{
    return entrances_->objectForKey(anEntranceName) ? true : false;
}

MCScene *
MCScene::getScene()
{
    return this;
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
