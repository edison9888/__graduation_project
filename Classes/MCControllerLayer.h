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
#include "MCRoleBaseInfo.h"

class MCControllerLayer : public CCLayer {
    
public:
    bool init();
    
    CREATE_FUNC(MCControllerLayer);
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    
private:
    MCJoypad *joypad_;
    MCTeam *team_;
    
    CC_PROPERTY(MCControllerDelegate *, delegate_, Delegate);
};

#endif /* defined(__Military_Confrontation__MCControllerLayer__) */
