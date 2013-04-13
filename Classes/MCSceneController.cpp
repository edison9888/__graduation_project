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

const char *kMCChangingSceneScriptFilepath = "scripts/changing-scene.lua";

static MCSceneController *__shared_scene_controller = NULL;

MCSceneController::MCSceneController()
{
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
 * 提交期待转换的场景信息
 */
//void
//MCSceneController::pushExpectedScene(MCScene *aNewScene, const char *anEntranceName, MCChangeSceneMethod method)
//{
//    expectedScene_ = aNewScene;
//    entranceName_ = new CCString;
//    entranceName_->initWithFormat("%s", anEntranceName);
//    CCString *entranceName = aNewScene->getEntranceName();
//    if (entranceName) {
//        entranceName->release();
//    }
//    entranceName = CCString::create(entranceName_->getCString());
//    aNewScene->setEntranceName(entranceName);
//    entranceName->retain();
//    method_ = method;
//}

/**
 * 提交期待转换的场景信息
 */
void
MCSceneController::pushExpectedScene(mc_object_id_t anObjectId, const char *anEntranceName, MCChangeSceneMethod method)
{
    expectedSceneId_ = anObjectId;
    entranceName_ = new CCString;
    entranceName_->initWithFormat("%s", anEntranceName);
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
    CCString *entranceName = newScene->getEntranceName();
    if (entranceName) {
        entranceName->release();
    }
    entranceName = CCString::create(entranceName_->getCString());
    newScene->setEntranceName(entranceName);
    entranceName->retain();
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
        || (!expectedScene_->hasEntrance(entranceName_->getCString()) && method_ != MCPopScene)) {
        expectedScene_ = NULL;
        CC_SAFE_RELEASE_NULL(entranceName_);
        return;
    }
    
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
        lastScene_ = currentScene_;
        currentScene_ = expectedScene_;
//    }
    expectedScene_ = NULL;
    CC_SAFE_RELEASE_NULL(entranceName_);
}
