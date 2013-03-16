//
//  MCBattleFieldScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-15.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCBattleFieldScene.h"

#include "MCActionMenu.h"

void
MCBattleFieldScene::installController()
{
    MCScenePackageType scenePackageType = scenePackage_->getScenePackageType();
    
    CCAssert(scenePackageType != MCUnknownPackage, "unknown scene package type!");
    if (MCBattleFieldScenePackage == scenePackageType) {
        controller_ = MCControllerLayer::create();
        controller_->setObjectDataSource(objects_);
        addChild(controller_);
        
        MCActionMenu *actionMenu = MCActionMenu::create(MCFloatMenu);
        controller_->setActionMenu(actionMenu);
        actionMenu->attach(this);
    }
}

void
MCBattleFieldScene::dragScene(const CCPoint &anOffset)
{
    
}
