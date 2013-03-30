//
//  MCJoypadControllerLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCControllerLayer__
#define __Military_Confrontation__MCControllerLayer__

#include "MCControllerDelegate.h"

#include "MCJoypad.h"

class MCJoypadControllerLayer : public CCLayer {
    
public:
    bool init();
    
    CREATE_FUNC(MCJoypadControllerLayer);
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    
protected:
    void didSelectAll(CCObject *aSender);
    
private:
    MCJoypad *joypad_;
    
    CC_PROPERTY(MCJoypadControllerDelegate *, delegate_, Delegate);
};

#endif /* defined(__Military_Confrontation__MCControllerLayer__) */
