//
//  MCMercenaryAI.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-16.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCMercenaryAI.h"

#include "MCSimpleGameSceneContextServer.h"

void
MCMercenaryAI::changingState()
{
    if (activating_) {
        return;
    }
    dynamic_cast<MCMercenaryAIStateMachineDelegate *>(AIStateMachineDelegate_)->activate(AIState_);
}

#pragma mark -
#pragma mark *** MCMercenaryAIStateMachineDelegate ***

void
MCMercenaryAIStateMachineDelegate::activate(MCAIState anAIState)
{
    if (anAIState == MCFollowingState) {
        performWhenFollowingState();
    } else if (anAIState == MCFleeState) {
        performWhenFleeState();
    }
    MCAIStateMachineDelegate::activate(anAIState);
}

