//
//  MCGameScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSceneManager.h"
#include "MCGameScene.h"
#include "MCEntrance.h"

#pragma mark *** MCGameSceneContextManager ***

static MCGameSceneContextManager *__shared_game_scene_context_manager = NULL;

MCGameSceneContextManager::MCGameSceneContextManager()
{
    contextStack_ = CCArray::create();
    contextStack_->retain();
}

MCGameSceneContextManager::~MCGameSceneContextManager()
{
    CC_SAFE_RELEASE(contextStack_);
}

MCGameSceneContextManager *
MCGameSceneContextManager::sharedGameSceneContextManager()
{
    if (__shared_game_scene_context_manager == NULL) {
        __shared_game_scene_context_manager = new MCGameSceneContextManager;
    }
    
    return __shared_game_scene_context_manager;
}

void
MCGameSceneContextManager::pushContext(MCGameSceneContext *aContext)
{
    contextStack_->addObject(aContext);
}

void MCGameSceneContextManager::popContext()
{
    contextStack_->removeLastObject();
}

MCGameSceneContext *
MCGameSceneContextManager::currentContext()
{
    return (MCGameSceneContext *) contextStack_->lastObject();
}

#pragma mark -
#pragma mark *** MCGameScene ***

/* 从场景包加载场景初始化 */
bool
MCGameScene::initWithScenePackage(MCScenePackage *aPackage)
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
        
        /* entrances */
        entrances_ = aPackage->getScenes();
        
        return true;
    }
    
    return false;
}

/* 从场景包加载场景 */
MCGameScene *
MCGameScene::createWithScenePackage(MCScenePackage *aPackage)
{
    MCGameScene *scene = new MCGameScene;
    
    if (scene && scene->initWithScenePackage(aPackage)) {
        scene->autorelease();
    } else {
        CC_SAFE_DELETE(scene);
        scene = NULL;
    }
    
    return scene;
}

void
MCGameScene::onEnter()
{
    CCScene::onEnter();
    MCGameSceneContext *context = new MCGameSceneContext;
    context->scene_ = this;
    MCGameSceneContextManager::sharedGameSceneContextManager()->pushContext(context);
    /* 预加载场景 */
}

void
MCGameScene::onExit()
{
    
    CCScene::onExit();
    
    MCGameSceneContext *context = MCGameSceneContextManager::sharedGameSceneContextManager()->currentContext();
    MCGameSceneContextManager::sharedGameSceneContextManager()->popContext();
    CC_SAFE_RELEASE(context);
}

/**
 * 安装触发器
 */
void
MCGameScene::installTrigger(MCTrigger *aTrigger)
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
MCGameScene::uninstallTrigger(MCTrigger *aTrigger)
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
MCGameScene::gotoScene(mc_object_id_t aSceneId, const char *anEntranceName, bool isInternal)
{
    MCSceneManager::sharedSceneManager()->changeSceneWithObjectId(aSceneId,
                                                                  anEntranceName,
                                                                  isInternal ? MCPushScene : MCReplaceScene);
}

/**
 * 从内部场景(比如房子、商店)出去
 */
void
MCGameScene::goOut()
{
    if (isInternalScene_) {
        MCSceneManager::sharedSceneManager()->changeScene(NULL, NULL, MCPopScene);
    }
}

bool
MCGameScene::hasEntrance(const char *anEntranceName)
{
    return entrances_->objectForKey(anEntranceName) ? true : false;
}

CCDictionary *
MCGameScene::getEntrances()
{
    return entrances_;
}
