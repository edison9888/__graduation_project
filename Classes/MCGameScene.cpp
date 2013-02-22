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
    
}

void
MCGameScene::onExit()
{
    
}

void
MCGameScene::controllerMoveUp(MCControllerDelegate *sender)
{
    
}

void
MCGameScene::controllerMoveDown(MCControllerDelegate *sender)
{
    
}

void
MCGameScene::controllerMoveLeft(MCControllerDelegate *sender)
{
    
}

void
MCGameScene::controllerMoveRight(MCControllerDelegate *sender)
{
    
}
