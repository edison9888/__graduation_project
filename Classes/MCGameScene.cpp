//
//  MCGameScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCGameScene.h"

/* 从场景包加载场景初始化 */
bool
MCGameScene::initWithScenePackage(MCScenePackage *aPackage)
{
    if (CCScene::init()) {
        
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
    
    /* 预加载场景 */
}

void
MCGameScene::onExit()
{
    CCScene::onExit();
}

void
MCGameScene::controllerMove(MCControllerDelegate *sender, const CCPoint &delta)
{
    
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
MCGameScene::gotoScene(mc_object_id_t aSceneId, const char *anEntranceName)
{
    
}
