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

bool
MCJoypadControllerLayer::init()
{
    if (CCLayer::init()) {
        CCSprite *bg = CCSprite::create ("joystick-background.png");
        CCSprite *control = CCSprite::create ("joystick-control.png");
        
        joypad_ = MCJoypad::create();
        addChild (joypad_);
        
        joypad_->setJoystick(MCJoystick::create(bg, control));
        joypad_->setVisible(false);
        joypad_->setTouchEnabled(false);
        
        setTouchEnabled(true);
        
        return true;
    }
    
    return false;
}

MCJoypadControllerDelegate *
MCJoypadControllerLayer::getDelegate()
{
    return joypad_->getDelegate();
}

void
MCJoypadControllerLayer::setDelegate(MCJoypadControllerDelegate* aDelegate)
{
    if (aDelegate) {
        joypad_->setVisible(true);
        joypad_->setTouchEnabled(true);
    } else {
        joypad_->setVisible(false);
        joypad_->setTouchEnabled(false);
    }
    joypad_->setDelegate(aDelegate);
}

void
MCJoypadControllerLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLayer::ccTouchesBegan(pTouches, pEvent);
}

void
MCJoypadControllerLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCLayer::ccTouchesMoved(pTouches, pEvent);
}

void
MCJoypadControllerLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    CCLayer::ccTouchesEnded(pTouches, pEvent);
}

void
MCJoypadControllerLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    CCLayer::ccTouchesCancelled(pTouches, pEvent);
}

void
MCJoypadControllerLayer::didSelectAll(CCObject *aSender)
{
}
