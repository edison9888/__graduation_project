//
//  MCJoypad.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCJoypad__
#define __Military_Confrontation__MCJoypad__

#include "MCJoystick.h"
#include "MCControllerDelegate.h"

class MCJoypad : public CCLayer
{
public:
    ~MCJoypad();
    bool init();
    
    CREATE_FUNC(MCJoypad);
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    void onEnter();
    void onExit();
    void update(float fDelta);
    
private:
    bool isValidControl_;
    CCTouch *controlTouch_;
    
    CC_PROPERTY(MCJoystick *, joystick_, Joystick)
    CC_SYNTHESIZE_PASS_BY_REF(CCPoint, joystickPosition_, JoystickPosition)
    CC_SYNTHESIZE(CCPoint *, joystickDelta_, JoystickDelta)
    CC_SYNTHESIZE(MCControllerDelegate *, delegate_, Delegate);
};

#endif /* defined(__Military_Confrontation__MCJoypad__) */
