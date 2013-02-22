//
//  MCControllerLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCControllerLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "MCKeyboardDispatcher.h"
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
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        keybord_ = MCKeyboard::create();
        addChild (keybord_);
#endif /* MacOSX、Windows和Linux控制响应 */
        
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
    keybord_->setDelegate(aDelegate);
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
    
}
