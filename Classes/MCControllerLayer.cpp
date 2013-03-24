//
//  MCControllerLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCControllerLayer.h"
#include "MCRoleEntity.h"
#include "MCObjectLayer.h"

#include "MCTeamLayer.h"

bool
MCControllerLayer::init()
{
    if (CCLayer::init()) {
        CCSprite *bg = CCSprite::create ("joystick-background.png");
        CCSprite *control = CCSprite::create ("joystick-control.png");
        
        joypad_ = MCJoypad::create();
        addChild (joypad_);
        
        joypad_->setJoystick(MCJoystick::create(bg, control));
        joypad_->setVisible(false);
        joypad_->setTouchEnabled(false);
        
    //warning: 木有debug
//        teamLayer_ = MCTeamLayer::create();
//        addChild(teamLayer_);
        
        CCMenu *menu;
        CCMenuItemLabel *menuItem;
        CCLabelTTF *label;
        
        menu = CCMenu::create();
        label = CCLabelTTF::create("全选", "Marker Felt", 24);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCControllerLayer::didSelectAll));
        addChild(menu);
        CCSize menuSize = menuItem->getContentSize();
        menu->setPosition(ccp(menuSize.width / 2, menuSize.height / 2));
        menu->setVisible(false);
        selectAllMenu_ = menu;
        
        setTouchEnabled(true);
        
        return true;
    }
    
    return false;
}

MCControllerDelegate * 
MCControllerLayer::getDelegate()
{
    return joypad_->getDelegate();
}

void
MCControllerLayer::setDelegate(MCControllerDelegate* aDelegate)
{
    if (aDelegate) {
        joypad_->setVisible(true);
        joypad_->setTouchEnabled(true);
        selectAllMenu_->setVisible(true);
    } else {
        joypad_->setVisible(false);
        joypad_->setTouchEnabled(false);
        selectAllMenu_->setVisible(false);
    }
    joypad_->setDelegate(aDelegate);
}

void
MCControllerLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLayer::ccTouchesBegan(pTouches, pEvent);
}

void
MCControllerLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCLayer::ccTouchesMoved(pTouches, pEvent);
}

void
MCControllerLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    CCLayer::ccTouchesEnded(pTouches, pEvent);
}

void
MCControllerLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    CCLayer::ccTouchesCancelled(pTouches, pEvent);
}

void
MCControllerLayer::didSelectAll(CCObject *aSender)
{
    if (delegate_) {
        delegate_->controllerDidSelectAll(delegate_, teamLayer_->getTeam());
    }
}
