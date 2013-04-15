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
#include "MCJoypadControllerLayer.h"

class MCBattleController;

class MCBattleControllerLayer : public MCJoypadControllerLayer {
    
public:
    MCBattleControllerLayer();
    ~MCBattleControllerLayer();
    
    bool init();
    
    CREATE_FUNC(MCBattleControllerLayer);
    
    bool isEnabled();
    void setEnabled(bool var);
    
    bool getJoypadEnable();
    void setJoypadEnable(bool var);
    
protected:
    void activate(CCObject *aSender); /* 在这里是切换为Joypad控制的按钮 */
    
    MCBattleController *controller_;
    
    CC_PROPERTY(MCBattleControllerDelegate *, delegate_, Delegate);
};

#endif /* defined(__Military_Confrontation__MCBattleControllerLayer__) */
