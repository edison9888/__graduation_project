//
//  MCJoypadControllerLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCJoypadControllerLayer__
#define __Military_Confrontation__MCJoypadControllerLayer__

#include "MCControllerDelegate.h"
#include "MCControllerLayer.h"

#include "MCJoypad.h"

class MCJoypadControllerLayer : public MCControllerLayer {
    
public:
    bool init();
    
    CREATE_FUNC(MCJoypadControllerLayer);
    
    bool isEnabled();
    void setEnabled(bool var);
    
    MCJoypadControllerDelegate *getJoypadDelegate();
    void setJoypadDelegate(MCJoypadControllerDelegate *aJoypadDelegate);
    
protected:
    void activate(CCObject *aSender); /* 行动 */
    
    MCJoypad *joypad_;
    CCMenu *actionButton_;
};

#endif /* defined(__Military_Confrontation__MCJoypadControllerLayer__) */
