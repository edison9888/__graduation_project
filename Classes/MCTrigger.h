//
//  MCTrigger.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-7.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTrigger__
#define __Military_Confrontation__MCTrigger__

#include "MCScript.h"

enum {
    MCLocationTrigger = MCMakeEnum(0),                    /* 位置触发器 */
    MCFlagTrigger     = MCMakeEnum(1),                    /* 标志触发器 */
    MCCommonTrigger   = MCLocationTrigger & MCFlagTrigger /* 通用触发器 */
};
typedef mc_enum_t MCTriggerType;

class MCTrigger : public CCObject {
    friend class MCTriggerMaker;
public:
    void trigger();
    
    CC_SYNTHESIZE_READONLY(MCTriggerType, triggerType_, TriggerType);
    CC_SYNTHESIZE_READONLY(CCRect, area_, Area);
    
private:
    MCScript *script_;
};

class MCTriggerMaker {
public:
    static MCTrigger *createTrigger(MCTriggerType aTriggerType, MCScript *aScript);
};

#endif /* defined(__Military_Confrontation__MCTrigger__) */
