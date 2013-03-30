//
//  MCBattleControllerLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBattleControllerLayer__
#define __Military_Confrontation__MCBattleControllerLayer__

#include "MCControllerDelegate.h"
#include "MCControllerLayer.h"

class MCBattleController;

class MCBattleControllerLayer : public MCControllerLayer {
    
public:
    MCBattleControllerLayer();
    ~MCBattleControllerLayer();
    
    bool init();
    
    CREATE_FUNC(MCBattleControllerLayer);
    
    bool isEnable();
    void setEnable(bool var);
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    
protected:
    
private:
    MCBattleController *controller_;
    
    CC_PROPERTY(MCBattleControllerDelegate *, delegate_, Delegate);
};

#endif /* defined(__Military_Confrontation__MCBattleControllerLayer__) */
