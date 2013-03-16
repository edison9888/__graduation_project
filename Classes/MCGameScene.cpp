//
//  MCGameScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCGameScene.h"

void
MCGameScene::installController()
{
    MCScenePackageType scenePackageType = scenePackage_->getScenePackageType();

    CCAssert(scenePackageType != MCUnknownPackage, "unknown scene package type!");
    if (MCGameScenePackage == scenePackageType) {
        controller_ = MCControllerLayer::create();
        controller_->setDelegate(objects_);
        addChild(controller_);
        
        MCActionMenu *actionMenu = MCActionMenu::create(MCFixedMenu);
        controller_->setActionMenu(actionMenu);
        actionMenu->attach(this);
    }
}