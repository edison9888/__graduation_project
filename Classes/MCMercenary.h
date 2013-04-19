//
//  MCMercenary.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMercenary__
#define __Military_Confrontation__MCMercenary__

#include "MCEnemy.h"
#include "MCEquipmentProperty.h"
#include "MCDice.h"
#include "MCMercenaryAI.h"

typedef mc_ushort_t mc_cost_t;

class MCMercenaryManager;

/* 佣兵 */
class MCMercenary : public MCEnemy, public MCMercenaryAIStateMachineDelegate {
    friend class MCMercenaryManager;
public:
    ~MCMercenary();
    
    bool init();
    
    static MCMercenary *create(mc_object_id_t anObjectId);
    
    /* MCAIStateMachineDelegate */
    /**
     * 空闲状态下回调
     */
    void performWhenIdleState();
    
    /**
     * 战斗状态下回调
     */
    void performWhenCombatantStatus() { MCRole::performWhenCombatantStatus(); }
    
    /**
     * 休息状态下回调
     */
    void performWhenRestingState() { MCRole::performWhenRestingState(); }
    
    /**
     * 攻击状态下回调
     */
    void performWhenAttackState() { MCRole::performWhenAttackState(); }
    
    /**
     * 死亡状态下回调
     */
    void performWhenDeathState() { MCRole::performWhenDeathState(); }
    
    /* MCMercenaryAIStateMachineDelegate */
    /**
     * 逃跑状态下回调
     */
    void performWhenFleeState();
    
    MCRoleEntity *getEntity();
    
    CCObject *copy();
    
    CC_SYNTHESIZE(mc_hp_t, dying_, Dying); /* 生命垂危线 */
    CC_SYNTHESIZE_READONLY(mc_cost_t, cost_, Cost); /* 雇佣所需费用 */
};

#endif /* defined(__Military_Confrontation__MCMercenary__) */
