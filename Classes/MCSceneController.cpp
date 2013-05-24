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
MCSceneController::loadSpawnScene(float delay)
{
    CCScheduler *m_pScheduler = CCDirector::sharedDirector()->getScheduler();
    
    m_pScheduler->scheduleSelector(schedule_selector(MCSceneController::_loadSpawnScene),
                                   this,
                                   0.0f,
                                   0,
                                   delay,
                                   false);
}

void
MCSceneController::_loadSpawnScene(float dt)
{
    pushExpectedScene(MCDungeonMaster::sharedDungeonMaster()->getSpawnPointID(), NULL);
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
        entranceName_ = anEntranceName;
    } else {
        entranceName_.clear();
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
    MCScene *newScene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(expectedSceneId_);
    expectedScene_ = newScene;
    if (! entranceName_.empty()) { /* entranceName_在载入到重生点场景时为NULL */
        newScene->setEntranceName(entranceName_);
    }
    
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
        || (entranceName_.empty() && !loadSpawnScene_)
        || (!entranceName_.empty() && !expectedScene_->hasEntrance(entranceName_.c_str()) && method_ != MCPopScene)) {
        expectedScene_ = NULL;
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
    entranceName_.clear();
    
#if MC_DEBUG_SERVER == 1
    MCSimpleGameSceneContextServer::defaultSimpleGameSceneContextServer()->notifySceneDidChange();
#endif
}
