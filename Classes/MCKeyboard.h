//
//  MCKeyboard.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCKeyboard__
#define __Military_Confrontation__MCKeyboard__

#include "MCControllerDelegate.h"
#include "MCKeyboardDispatcher.h"

enum {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    MCKeyUp     = 126,
    MCKeyDown   = 125,
    MCKeyLeft   = 123,
    MCKeyRight  = 124
#endif //CC_PLATFORM_MAC Mac OS X的KeyCode
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    MCKeyUp     = 38,
    MCKeyDown   = 40,
    MCKeyLeft   = 37,
    MCKeyRight  = 39
#endif //CC_PLATFORM_WIN32 Windows的KeyCode
};
typedef mc_enum_t MCKeyCode;

class MCKeyboard : public CCNode, public MCKayboardDelegate
{
public:
    bool init();
    CREATE_FUNC(MCKeyboard);
    
protected:
    void keyPressed(unsigned short keycode);
    void keyReleased(unsigned short keycode);
    
    CC_SYNTHESIZE(MCControllerDelegate *, delegate_, Delegate);
};

#endif /* defined(__Military_Confrontation__MCKeyboard__) */
