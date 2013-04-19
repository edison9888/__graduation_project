//
//  MCMercenaryAI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-16.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMercenaryAI__
#define __Military_Confrontation__MCMercenaryAI__

#include "MCAI.h"

class MCMercenaryAI : public MCAI {
public:
    void changingState();
    
    CREATE_FUNC(MCMercenaryAI);
};

class MCMercenaryAIStateMachineDelegate : public MCAIStateMachineDelegate {
public:
    /**
     * 激活某状态
     */
    virtual void activate(MCAIState anAIState);
    
    /**
     * 跟随状态下回调
     */
    virtual void performWhenFollowingState() {}
    
    /**
     * 逃跑状态下回调
     */
    virtual void performWhenFleeState() {}
};

#endif /* defined(__Military_Confrontation__MCMercenaryAI__) */
