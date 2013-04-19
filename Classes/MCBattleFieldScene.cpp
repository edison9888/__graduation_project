//
//  MCBattleFieldScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-15.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCBattleFieldScene.h"
#include "MCFlagManager.h"
#include "MCJoypadControllerLayer.h"
#include "MCBattleControllerLayer.h"
#include "MCObjectLayer.h"

void
MCBattleFieldScene::installController()
{
    MCScenePackageType scenePackageType = scenePackage_->getScenePackageType();
    
    CCAssert(scenePackageType != MCUnknownPackage, "unknown scene package type!");
    if (MCBattleFieldScenePackage == scenePackageType) {
        if (MCFlagManager::sharedFlagManager()->isTaskStarted()) {
            MCBattleControllerLayer *battleController = MCBattleControllerLayer::create();
            battleController->setJoypadDelegate(objects_);
            battleController->setDelegate(objects_);
            battleController->setJoypadEnable(false);
            addChild(battleController);
            controller_ = battleController;
        } else {
            MCJoypadControllerLayer *joypadController = MCJoypadControllerLayer::create();
            joypadController->setJoypadDelegate(objects_);
            addChild(joypadController);
            controller_ = joypadController;
        }
    }
}

void
MCBattleFieldScene::showDetail()
{
    usingJoypad_ = dynamic_cast<MCBattleControllerLayer *>(controller_)->isJoypadEnable();
    MCScene::showDetail();
}

void
MCBattleFieldScene::detailDidHide()
{
    MCScene::detailDidHide();
    dynamic_cast<MCBattleControllerLayer *>(controller_)->setJoypadEnable(usingJoypad_);
}
