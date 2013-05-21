//
//  MCTaskContext.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskContext__
#define __Military_Confrontation__MCTaskContext__

#include "MCTask.h"
#include "MCBackpack.h"

class MCTaskContext {
    friend class MCTask;
private:
    MCTaskContext();
public:
    ~MCTaskContext();
    
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, trapWide_, TrapWide);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, trapDamage_, TrapDamage);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, healthPotion_, HealthPotion);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, physicalPotion_, PhysicalPotion);
    
    CC_SYNTHESIZE_READONLY(mc_size_t, trapWideCount_, TrapWideCount);                 /* 大范围陷阱持有个数 */
    CC_SYNTHESIZE_READONLY(mc_size_t, trapDamageCount_, TrapDamageCount);             /* 大伤害陷阱持有个数 */
    CC_SYNTHESIZE_READONLY(mc_size_t, healthPotionCount_, HealthPotionCount);         /* 生命药水持有个数 */
    CC_SYNTHESIZE_READONLY(mc_size_t, physicalPotionCount_, PhysicalPotionCount);     /* 体力药水持有个数 */
    
    /* 累计 */
    CC_SYNTHESIZE_READONLY(mc_hp_t, hp_, HP); /* HP影响 */
    CC_SYNTHESIZE_READONLY(mc_pp_t, pp_, PP); /* PP影响 */
    CC_SYNTHESIZE_READONLY(mc_damage_t, damageBonus_, DamageBonus); /* 伤害值加成 */
    CC_SYNTHESIZE_READONLY(mc_armor_check_penalty_t, armorCheckPenalty_, ArmorCheckPenalty); /* 防具检定减值加成 */
    CC_SYNTHESIZE_READONLY(MCSpecial, special_, Special); /* 特殊效果 */
    
    CC_SYNTHESIZE_READONLY(MCTask *, task_, Task);
    CC_SYNTHESIZE_READONLY(MCRegion *, taskRegion_, TaskRegion);
};

#endif /* defined(__Military_Confrontation__MCTaskContext__) */
