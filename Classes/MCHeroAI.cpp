//
//  MCHeroAI.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCHeroAI.h"

void
MCHeroAI::changingState()
{
    if (activating_) {
        return;
    }
    
    /* 3秒不动切换到休息状态 */
    time_t lastActivationTime = lastActivationTime_;
    time_t now = time(NULL);
    
    /* 停3秒 */
    if (now - lastActivationTime > 3) {
        AIState_ = MCRestingState;
        lastActivationTime_ = now;
    }
    
    MCAI::changingState();
}
