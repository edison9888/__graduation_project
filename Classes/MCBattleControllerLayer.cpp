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
    if (MCJoypadControllerLayer::init()) {
        controller_ = MCBattleController::create();
        addChild(controller_);
        
        CCMenuItem *menuItem = dynamic_cast<CCMenuItem *>(actionButton_->getChildren()->objectAtIndex(0));
        menuItem->setTarget(this,
                            menu_selector(MCBattleControllerLayer::activate));
        
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

bool
MCBattleControllerLayer::isEnable()
{
    return controller_->isTouchEnabled();
}

void
MCBattleControllerLayer::setEnable(bool var)
{
    MCJoypadControllerLayer::setEnable(var);
    controller_->setVisible(var);
    controller_->setTouchEnabled(var);
}

bool
MCBattleControllerLayer::getJoypadEnable()
{
    return joypad_->isTouchEnabled();
}

void
MCBattleControllerLayer::setJoypadEnable(bool var)
{
    joypad_->setVisible(var);
    joypad_->setTouchEnabled(var);
}

void
MCBattleControllerLayer::activate(CCObject *aSender) /* 在这里是切换为Joypad控制的按钮 */
{
    if (joypad_->isVisible()) {
        setJoypadEnable(false);
        controller_->setVisible(true);
        controller_->setTouchEnabled(true);
    } else {
        setJoypadEnable(true);
        controller_->setVisible(false);
        controller_->setTouchEnabled(false);
    }
}
