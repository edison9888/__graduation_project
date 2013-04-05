//
//  MCBattleFieldScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-15.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCBattleFieldScene.h"
#include "MCBattleControllerLayer.h"

void
MCBattleFieldScene::installController()
{
    MCScenePackageType scenePackageType = scenePackage_->getScenePackageType();
    
    CCAssert(scenePackageType != MCUnknownPackage, "unknown scene package type!");
    if (MCBattleFieldScenePackage == scenePackageType) {
        MCBattleControllerLayer *battleController = MCBattleControllerLayer::create();
        battleController->setDelegate(objects_);
        addChild(battleController);
        controller_ = battleController;
    }
}

void
MCBattleFieldScene::dragScene(const CCPoint &anOffset)
{
}

