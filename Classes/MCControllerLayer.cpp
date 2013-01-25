//
//  MCControllerLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCControllerLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "MCKeyboardDispatcher.h"
#endif /* MacOSX控制响应 */

bool
MCControllerLayer::init()
{
    if (CCLayer::init()) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CCSprite *bg = CCSprite::create ("joystick-background.png");
        CCSprite *control = CCSprite::create ("joystick-control.png");
        
        joypad_ = MCJoypad::create();
        addChild (joypad_);
        
        joypad_->setJoystick(MCJoystick::create(bg, control));
#endif /* Android和iOS控制响应 */
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        keybord_ = MCKeyboard::create();
        addChild (keybord_);
        
        CCSprite *bg = CCSprite::create ("joystick-background.png");
        CCSprite *control = CCSprite::create ("joystick-control.png");
        
        joypad_ = MCJoypad::create();
        addChild (joypad_);
        
        joypad_->setJoystick(MCJoystick::create(bg, control));
#endif /* MacOSX、Windows和Linux控制响应 */
        
        return true;
    }
    
    return false;
}

MCControllerDelegate * 
MCControllerLayer::getDelegate()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return joypad_->getDelegate();
#endif /* Android和iOS控制响应 */
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    return keybord_->getDelegate();
    return joypad_->getDelegate();
#endif /* MacOSX、Windows和Linux控制响应 */
}

void
MCControllerLayer::setDelegate(MCControllerDelegate* aDelegate)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    joypad_->setDelegate(aDelegate);
#endif /* Android和iOS控制响应 */
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    keybord_->setDelegate(aDelegate);
    joypad_->setDelegate(aDelegate);
#endif /* MacOSX、Windows和Linux控制响应 */
}
