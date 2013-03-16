//
//  MCControllerLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCControllerLayer.h"
#include "MCActionMenu.h"
#include "MCRoleEntity.h"
#include "MCObjectLayer.h"

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
        
        actionMenu_ = MCActionMenu::create(MCFixedMenu);
        
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
    } else {
        joypad_->setVisible(false);
        joypad_->setTouchEnabled(false);
    }
    joypad_->setDelegate(aDelegate);
}

MCActionMenu *
MCControllerLayer::actionMenu()
{
    return actionMenu_;
}

void
MCControllerLayer::actionMenuDidOpen(MCActionMenu *anActionMenu)
{
    actionMenu_->openMenu(actionMenu_->getChildren()->objectAtIndex(0));
}

void
MCControllerLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLayer::ccTouchesBegan(pTouches, pEvent);
    if (! objectDataSource_) {
        return;
    }
    CCObject *obj;
    MCRole *role;
    MCRoleEntity *entity;
    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    CCPoint touchedLoaction = touch->getLocation();
    CCArray *objects = objectDataSource_->objects();
    bool selected = false;
    
    CCARRAY_FOREACH(objects, obj) {
        role = (MCRole *)obj;
        entity = role->getEntity();
        if (entity->shouldBeSelected(touchedLoaction)) {
            actionMenuDidOpen(actionMenu_);
            selected = true;
            break;
        }
    }
    
    if (!selected) {
#warning hero/mercenary moveTo
        ;
    }
}
