//
//  MCJoypadDelegate.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCJoypadDelegate__
#define __Military_Confrontation__MCJoypadDelegate__

#include <cocos2d.h>

USING_NS_CC;

NS_CC_BEGIN

namespace joypad {
    
    enum {
        kJoystickDirectionUp = 1,
        kJoystickDirectionDown,
        kJoystickDirectionLeft,
        kJoystickDirectionRight,
        kJoystickDirectionUpLeft,
        kJoystickDirectionUpRight,
        kJoystickDirectionDownLeft,
        kJoystickDirectionDownRight
    };
    typedef unsigned int MCJoystickDirection;
    
    class MCJoystickDelegate : CCObject {
        virtual void joystickDidMoveToUp(CCObject *object) = 0;
        virtual void joystickDidMoveToDown(CCObject *object) = 0;
        virtual void joystickDidMoveToLeft(CCObject *object) = 0;
        virtual void joystickDidMoveToRight(CCObject *object) = 0;
    };
}

NS_CC_END



#endif /* defined(__Military_Confrontation__MCJoypadDelegate__) */
