//
//  MCSceneController.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-28.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSceneController.h"
#include "MCSceneManager.h"
#include "MCScript.h"
#include "MCDungeonMaster.h"
#include "MCFlagManager.h"

#if MC_DEBUG_SERVER == 1
#include "MCSimpleGameSceneContextServer.h"
#endif

const char *kMCSceneDidLoadNotification = "kMCSceneDidLoadNotification";

static const char *kMCChangingSceneScriptFilepath = "scripts/changing-scene.lua";

static MCSceneController *__shared_scene_controller = NULL;

MCSceneController::MCSceneController()
{
    loadSpawnScene_ = false;
    lastScene_ = NULL;
    currentScene_ = NULL;
}

MCSceneController::~MCSceneController()
{
    
}

MCSceneController *
MCSceneController::sharedSceneController()
{
    if (__shared_scene_controller == NULL) {
        __shared_scene_controller = new MCSceneController;
    }
    
    return __shared_scene_controller;
}

/**
 * 死亡或者开始游戏的时候加载，场景为最后的重生点场景
 */
void
MCSceneController::loadSpawnScene()
{
    MCDungeonMaster *dm = MCDungeonMaster::sharedDungeonMaster();
    
    pushExpectedScene(dm->getSpawnPointID(), NULL);
    loadSpawnScene_ = true;
    requestChangingScene();
}

/**
 * 提交期待转换的场景信息
 */
void
MCSceneController::pushExpectedScene(mc_object_id_t anObjectId, const char *anEntranceName, MCChangeSceneMethod method)
{
    expectedSceneId_ = anObjectId;
    if (anEntranceName) {
        entranceName_ = new CCString;
        entranceName_->initWithFormat("%s", anEntranceName);
    } else {
        entranceName_ = NULL;
    }
    method_ = method;
}

/**
 * 请求切换场景
 */
void
MCSceneController::requestChangingScene()
{
    MCScriptRun(kMCChangingSceneScriptFilepath);
}

/**
 * so-called private method!
 * 不该自己来执行这个方法，转交给lua来运行！
 * 加载场景包
 */
void
MCSceneController::__loadScene()
{
    CCLog("will load scene");
    MCScene *newScene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(expectedSceneId_);
    expectedScene_ = newScene;
    CCString *entranceName = newScene->getEntranceName();
    if (entranceName) {
        entranceName->release();
    }
    if (entranceName_) { /* entranceName_在载入到重生点场景时为NULL */
        entranceName = CCString::create(entranceName_->getCString());
        newScene->setEntranceName(entranceName);
        entranceName->retain();
    }
    CCLog("did load scene");
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCSceneDidLoadNotification);
}

/**
 * so-called private method!
 * 不该自己来执行这个方法，转交给lua来运行！
 * 切换当前场景为aNewScene
 */
void
MCSceneController::__changeScene()
{
    if ((expectedScene_ == NULL && method_ != MCPopScene)
        || (entranceName_ == NULL && !loadSpawnScene_)
        || (entranceName_ != NULL && !expectedScene_->hasEntrance(entranceName_->getCString()) && method_ != MCPopScene)) {
        expectedScene_ = NULL;
        CC_SAFE_RELEASE_NULL(entranceName_);
        return;
    }
    
    loadSpawnScene_ = false;
    
//    if (MCPushScene == method_) {
//        CCDirector::sharedDirector()->pushScene(expectedScene_);
//        lastScene_ = currentScene_;
//        currentScene_ = expectedScene_;
//    } else if (MCPopScene == method_) {
//        CCDirector::sharedDirector()->popScene();
//        MCScene *tmp = lastScene_;
//        lastScene_ = currentScene_;
//        currentScene_ = tmp; /* lastScene_ */
//    } else {
        CCDirector::sharedDirector()->replaceScene(expectedScene_);
//        lastScene_ = currentScene_;
//        currentScene_ = expectedScene_;
//    }
    expectedScene_ = NULL;
    CC_SAFE_RELEASE_NULL(entranceName_);
    
#if MC_DEBUG_SERVER == 1
    MCSimpleGameSceneContextServer::defaultSimpleGameSceneContextServer()->notifySceneDidChange();
#endif
}
