//
//  MCBattleControllerLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCBattleControllerLayer.h"

#include "MCBattleController.h"

MCBattleControllerLayer::MCBattleControllerLayer()
{
    
}

MCBattleControllerLayer::~MCBattleControllerLayer()
{
    
}

bool
MCBattleControllerLayer::init()
{
    if (CCLayer::init()) {
        controller_ = MCBattleController::create();
        addChild(controller_);
        return true;
    }
    
    return false;
}

MCBattleControllerDelegate *
MCBattleControllerLayer::getDelegate()
{
    return controller_->getDelegate();
}

void
MCBattleControllerLayer::setDelegate(MCBattleControllerDelegate* aDelegate)
{
    if (aDelegate) {
        controller_->setVisible(true);
        controller_->setTouchEnabled(true);
    } else {
        controller_->setVisible(false);
        controller_->setTouchEnabled(false);
    }
    controller_->setDelegate(aDelegate);
}

void
MCBattleControllerLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
}

void
MCBattleControllerLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    
}

void
MCBattleControllerLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    
}

void
MCBattleControllerLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    
}

