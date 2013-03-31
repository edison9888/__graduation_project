//
//  MCEnemy.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEnemy__
#define __Military_Confrontation__MCEnemy__

#include "MCNPC.h"
#include "MCEquipmentProperty.h"
#include "MCDice.h"

/* 基础怪物 */
class MCEnemy : public MCNPC {
    friend class MCRoleManager;
public:
    bool init(MCRoleRace aRoleRace);
    
    static MCEnemy *create(mc_object_id_t anObjectId);
    
    CCObject *copy();
    
    CC_SYNTHESIZE(mc_ac_t, ac_, AC); /* 防御等级 */
    CC_SYNTHESIZE(mc_armor_check_penalty_t, armorCheckPenalty_, ArmorCheckPenalty); /* 防具检定减值 */
    CC_SYNTHESIZE(MCDice *, damage_, Damage); /* 伤害值 */
    CC_SYNTHESIZE(mc_bonus_t, damageBonus_, DamageBonus); /* 伤害值加成 */
    CC_SYNTHESIZE(MCDiceRange, criticalHitVisible_, CriticalHitVisible); /* 可视区域重击范围 */
    CC_SYNTHESIZE(MCDiceRange, criticalHitInvisible_, CriticalHitInvisible); /* 非可视区域重击范围 */
    CC_SYNTHESIZE(mc_critical_hit_t, criticalHit_, CriticalHit); /* 重击倍数 */
    CC_SYNTHESIZE(mc_distance_t, distance_, Distance); /* 攻击距离 */
#warning 木有配置技能
    CC_SYNTHESIZE(MCRoleState, effect_, Effect); /* 附带效果 */
    CC_SYNTHESIZE(MCDiceRange, effectCheck_, EffectCheck); /* 附带效果判定 */
};

#endif /* defined(__Military_Confrontation__MCEnemy__) */
