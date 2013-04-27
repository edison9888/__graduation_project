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
    
    bool isEnabled();
    void setEnabled(bool var);
    
    bool isJoypadEnable();
    void setJoypadEnable(bool var);
    
    MCBattleControllerDelegate *getDelegate();
    void setDelegate(MCBattleControllerDelegate *aJoypadDelegate);

    void onEnter();
    void onExit();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    CREATE_FUNC(MCBattleControllerLayer);
    
protected:
    /**
     * 在这里是切换为Joypad控制的按钮
     * 仅能在选中一人的情况下使用摇杆
     */
    void activate(CCObject *aSender);
    
    MCBattleController *controller_;
};

#endif /* defined(__Military_Confrontation__MCBattleControllerLayer__) */
