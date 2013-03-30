//
//  MCTask.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTask__
#define __Military_Confrontation__MCTask__

#include "MCObject.h"
#include "JsonBox.h"
#include "MCEquipmentProperty.h"
#include "MCTrap.h"

class MCFlag;
class MCTaskContext;

enum {
    MCTaskUnknownStatus = 0,
    MCTaskDone          = MCMakeEnum(0),
    MCTaskActiviting    = MCMakeEnum(1),
    MCTaskUncompleted   = MCMakeEnum(2)
};
typedef mc_enum_t MCTaskStatus;

class MCTask : public MCObject {
    friend class MCTaskAccessor;
public:
    MCTask();
    ~MCTask();
    
    CCObject *copy();
    
    void generateTaskContext();
    
private:
    void loadTaskContent(JsonBox::Object &aTaskContent);
    
    CC_SYNTHESIZE_READONLY(MCTrapType, trapType_, TrapType);                          /* 陷阱类型 */
    CC_SYNTHESIZE_READONLY(mc_size_t, trapWideMaxSize_, TrapWideMaxSize);             /* 大范围陷阱限制个数 */
    CC_SYNTHESIZE_READONLY(mc_size_t, trapDamageMaxSize_, TrapDamageMaxSize);         /* 大伤害陷阱限制个数 */
    CC_SYNTHESIZE_READONLY(mc_size_t, healthPotionMaxSize_, HealthPotionMaxSize);     /* 生命药水限制个数 */
    CC_SYNTHESIZE_READONLY(mc_size_t, physicalPotionMaxSize_, PhysicalPotionMaxSize); /* 体力药水限制个数 */
    
    CC_SYNTHESIZE_READONLY(mc_size_t, trapWideBonus_, TrapWideBonus);                 /* 大范围陷阱赠送个数 */
    CC_SYNTHESIZE_READONLY(mc_size_t, trapDamageBonus_, TrapDamageBonus);             /* 大伤害陷阱赠送个数 */
    CC_SYNTHESIZE_READONLY(mc_size_t, healthPotionBonus_, HealthPotionBonus);         /* 生命药水赠送个数 */
    CC_SYNTHESIZE_READONLY(mc_size_t, physicalPotionBonus_, PhysicalPotionBonus);     /* 体力药水赠送个数 */
    
    CC_SYNTHESIZE_READONLY(mc_price_t, cashPledge_, CashPledge);        /* 任务押金 */
    CC_SYNTHESIZE_READONLY(mc_price_t, bonus_, Bonus);                  /* 任务奖励 */
    CC_SYNTHESIZE_READONLY(CCArray *, targets_, Targets);               /* 任务目标 */
    CC_PROPERTY(MCTaskStatus, taskStatus_, TaskStatus);                 /* 任务状态 */
    CC_SYNTHESIZE_READONLY(MCTaskContext *, taskContext_, TaskContext); /* 任务上下文 */
    CC_SYNTHESIZE(MCFlag *, flag_, Flag);                               /* 开启标签 */
};

#endif /* defined(__Military_Confrontation__MCTask__) */
