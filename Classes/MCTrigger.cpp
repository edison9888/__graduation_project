//
//  MCTrigger.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-7.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTrigger.h"

void
MCTrigger::trigger()
{
    script_->run();
}

MCTrigger *
MCTriggerMaker::createTrigger(MCTriggerType aTriggerType, MCScript *aScript)
{
    MCTrigger *trigger;
    
    trigger = new MCTrigger;
    trigger->autorelease();
    trigger->triggerType_ = aTriggerType;
    trigger->script_ = aScript;
    
    return trigger;
}
