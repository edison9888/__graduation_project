//
//  MCScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-15.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSceneManager.h"
#include "MCScene.h"
#include "MCEntrance.h"
#include "MCActionMenu.h"

#pragma mark *** MCSceneContextManager ***

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

/* 从场景包加载场景初始化 */
bool
MCScene::initWithScenePackage(MCScenePackage *aPackage)
{
    if (CCScene::init() && aPackage) {
        scenePackage_ = aPackage;
        background_ = MCBackgroundLayer::create(aPackage->getTMXTiledMapPath()->getCString(),
                                                aPackage->getBackgroundMusicPath()->getCString());
        addChild(background_);
        
        objects_ = MCObjectLayer::create();
        objects_->setTMXTiledMap(background_->getMap());
        objects_->setSceneDelegate(this);
        objects_->loadEntrancesFromScenePackage(aPackage);
        addChild(objects_);
        
        controller_ = MCControllerLayer::create();
        controller_->setDelegate(objects_);
        addChild(controller_);
        
        viewport_ = MCViewportLayer::create();
        addChild(viewport_);
        
#warning debug
//        controller_->getActionMenu()->attach(this);
        
        /* entrances */
        entrances_ = aPackage->getScenes();
        
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

void
MCScene::onEnter()
{
    MCSceneContext *context = new MCSceneContext;
    context->scene_ = this;
    MCSceneContextManager::sharedGameSceneContextManager()->pushContext(context);
    CCScene::onEnter();
    /* 预加载场景 */
}

void
MCScene::onExit()
{
    
    CCScene::onExit();
    
    MCSceneContext *context = MCSceneContextManager::sharedGameSceneContextManager()->currentContext();
    MCSceneContextManager::sharedGameSceneContextManager()->popContext();
    CC_SAFE_RELEASE(context);
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
    MCSceneManager::sharedSceneManager()->changeSceneWithObjectId(aSceneId,
                                                                  anEntranceName,
                                                                  isInternal ? MCPushScene : MCReplaceScene);
}

/**
 * 从内部场景(比如房子、商店)出去
 */
void
MCScene::goOut()
{
    if (isInternalScene_) {
        
        MCSceneManager::sharedSceneManager()->changeScene(NULL, NULL, MCPopScene);
    }
}

bool
MCScene::hasEntrance(const char *anEntranceName)
{
    return entrances_->objectForKey(anEntranceName) ? true : false;
}

CCDictionary *
MCScene::getEntrances()
{
    return entrances_;
}
