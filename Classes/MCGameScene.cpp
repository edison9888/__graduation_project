//
//  MCGameScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCGameScene.h"
#include "MCObjectLayer.h"
#include "MCJoypadControllerLayer.h"

void
MCGameScene::installController()
{
    MCJoypadControllerLayer *controller = MCJoypadControllerLayer::create();
    controller->setJoypadDelegate(objects_);
    addChild(controller);
    controller_ = controller;
}