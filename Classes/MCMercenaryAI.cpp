//
//  MCMercenaryAI.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-16.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCMercenaryAI.h"

void
MCMercenaryAI::changingState()
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
    
    dynamic_cast<MCMercenaryAIStateMachineDelegate *>(AIStateMachineDelegate_)->activate(AIState_);
}

#pragma mark -
#pragma mark *** MCMercenaryAIStateMachineDelegate ***

void
MCMercenaryAIStateMachineDelegate::activate(MCAIState anAIState)
{
    if (anAIState == MCFleeState) {
        performWhenFleeState();
    }
    MCAIStateMachineDelegate::activate(anAIState);
}

