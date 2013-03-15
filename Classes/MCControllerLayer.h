//
//  MCControllerLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCControllerLayer__
#define __Military_Confrontation__MCControllerLayer__

#include "MCControllerDelegate.h"

#include "MCJoypad.h"

class MCActionMenu;

/* 控制器模式 */
enum {
    MCNormalControlMode = MCMakeEnum(0), /* 通常模式 */
    MCBattleControlMode = MCMakeEnum(1), /* 战斗模式 */
};
typedef mc_enum_t MCControlMode;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include "MCKeyboardboard.h"
#endif /* MacOSX、Windows和Linux控制响应 */

class MCControllerLayer : public CCLayer {
    
public:
    bool init();
    
    CREATE_FUNC(MCControllerLayer);

    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    
private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    MCKeyboard *keybord_;
#endif /* MacOSX、Windows和Linux控制响应 */
    MCJoypad *joypad_;
    
    CC_PROPERTY(MCControllerDelegate *, delegate_, Delegate);
    CC_PROPERTY(MCControlMode, controlMode_, ControlMode);
    CC_SYNTHESIZE(MCActionMenu *, actionMenu_, ActionMenu);
};

#endif /* defined(__Military_Confrontation__MCControllerLayer__) */
