//
//  MCJoypadControllerLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCJoypadControllerLayer.h"
#include "MCRoleEntity.h"
#include "MCObjectLayer.h"

#include "MCTeamLayer.h"

static const char *kMCActionButtonFilepath = "UI/activate.png";
static const char *kMCActionButtonPressedFilepath = "UI/activate_pressed.png";

bool
MCJoypadControllerLayer::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *bg = CCSprite::create ("joystick-background.png");
        CCSprite *control = CCSprite::create ("joystick-control.png");
        
        joypad_ = MCJoypad::create();
        addChild (joypad_);
        
        joypad_->setJoystick(MCJoystick::create(bg, control));
        joypad_->setVisible(false);
        joypad_->setTouchEnabled(false);
        
        CCMenu *actionButton = CCMenu::create();
        addChild(actionButton);
        CCMenuItemImage *imageButton = CCMenuItemImage::create(kMCActionButtonFilepath,
                                                               kMCActionButtonPressedFilepath);
        imageButton->setTarget(this, menu_selector(MCJoypadControllerLayer::activate));
        actionButton->addChild(imageButton);
        actionButton->setPosition(ccp(winSize.width - imageButton->getContentSize().width / 2,
                                      winSize.height / 2));
        actionButton_ = actionButton;
        
        setTouchEnabled(true);
        
        return true;
    }
    
    return false;
}

MCJoypadControllerDelegate *
MCJoypadControllerLayer::getJoypadDelegate()
{
    return joypad_->getDelegate();
}

void
MCJoypadControllerLayer::setJoypadDelegate(MCJoypadControllerDelegate *aJoypadDelegate)
{
    if (aJoypadDelegate) {
        joypad_->setVisible(true);
        joypad_->setTouchEnabled(true);
    } else {
        joypad_->setVisible(false);
        joypad_->setTouchEnabled(false);
    }
    joypad_->setDelegate(aJoypadDelegate);
}

bool
MCJoypadControllerLayer::isEnabled()
{
    return joypad_->isTouchEnabled();
}

void
MCJoypadControllerLayer::setEnabled(bool var)
{
    joypad_->setVisible(var);
    joypad_->setTouchEnabled(var);
    actionButton_->setVisible(var);
}

void
MCJoypadControllerLayer::activate(CCObject *aSender) /* 行动按键 */
{
    joypad_->getDelegate()->controllerDidActivate();
}
