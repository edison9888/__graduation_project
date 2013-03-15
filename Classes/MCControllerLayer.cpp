//
//  MCControllerLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCControllerLayer.h"
#include "MCActionMenu.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include "MCKeyboardDispatcher.h"
#endif /* MacOSX、Windows和Linux控制响应 */

bool
MCControllerLayer::init()
{
    if (CCLayer::init()) {
        CCSprite *bg = CCSprite::create ("joystick-background.png");
        CCSprite *control = CCSprite::create ("joystick-control.png");
        
        joypad_ = MCJoypad::create();
        addChild (joypad_);
        
        joypad_->setJoystick(MCJoystick::create(bg, control));
        
        actionMenu_ = MCActionMenu::create(MCFixedMenu);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//        keybord_ = MCKeyboard::create();
//        addChild (keybord_);
#endif /* MacOSX、Windows和Linux控制响应 */
        setTouchEnabled(true);
//        setControlMode(MCBattleControlMode);
        
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
    joypad_->setDelegate(aDelegate);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    keybord_->setDelegate(aDelegate);
#endif /* MacOSX、Windows和Linux控制响应 */
}

MCControlMode
MCControllerLayer::getControlMode()
{
    return 0;
}

void
MCControllerLayer::setControlMode(MCControlMode aControlMode)
{
    if (aControlMode == MCBattleControlMode) {
        joypad_->setVisible(false);
        joypad_->setTouchEnabled(false);
    } else {
        joypad_->setTouchEnabled(true);
        joypad_->setVisible(true);
    }
    controlMode_ = aControlMode;
}

void
MCControllerLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {
    CCLayer::ccTouchesBegan(pTouches, pEvent);
#warning debug
//    if (actionMenu_ && actionMenu_->getActionMenuType() == MCFloatMenu) {
//        actionMenu_->openMenu(NULL);
//    }
}
