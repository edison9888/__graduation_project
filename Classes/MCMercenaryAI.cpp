//
//  MCMercenaryAI.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-16.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCMercenaryAI.h"

void
MCMercenaryAI::update(float dt)
{
    MCAI::update(dt);
}

#pragma mark -
#pragma mark *** MCMercenaryAIStateMachineDelegate ***

void
MCMercenaryAIStateMachineDelegate::activate(MCAIState anAIState)
{
    CCLog("MCMercenaryAIStateMachineDelegate::activate");
    if (anAIState == MCFollowingState) {
        performWhenFollowingState();
    } else if (anAIState == MCFleeState) {
        performWhenFleeState();
    }
    MCAIStateMachineDelegate::activate(anAIState);
}

