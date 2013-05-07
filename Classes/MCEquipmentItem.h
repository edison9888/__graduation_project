//
//  MCEquipmentItem.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEquipmentItem__
#define __Military_Confrontation__MCEquipmentItem__

#include "MCItem.h"
#include "MCEquipment.h"
#include "MCOre.h"

class MCItemManager;

class MCEquipmentItem : public MCItem {
    friend class MCItemManager;
    friend class MCEquipmentManager;
public:
    ~MCEquipmentItem();
    bool init(MCEquipmentType aEquipmentType);
    
    static MCEquipmentItem *create(MCEquipmentType aEquipmentType);
    
    CCArray *skills();
    
    CCObject *copy();
    
    /* 战斗用接口 */
    /**
     * 若非武器则返回-1
     */
    CC_PROPERTY_READONLY(mc_dice_unit_t, attackCheck_, AttackCheck); /* attackCheck_实际上是武器敏捷调整值 */
    /**
     * 若非武器则返回-1
     */
    CC_PROPERTY_READONLY(mc_damage_t, damage_, Damage); /* damage_实际上是矿石的伤害加成 */
    /**
     * 若非武器则返回-1
     */
    CC_PROPERTY_READONLY(mc_critical_hit_t, criticalHit_, CriticalHit);
    /**
     * 若非防具则返回-1
     */
    CC_PROPERTY_READONLY(mc_ac_t, ac_, AC);
    /**
     * 若非防具则返回-1
     */
    CC_PROPERTY_READONLY(mc_armor_check_penalty_t, armorCheckPenalty_, ArmorCheckPenalty);
    
    /**
     * 若非武器则返回-1
     * 武器评分(没有加上体力状态评分)
     */
    CC_PROPERTY_READONLY(mc_score_t, damageScore_, DamageScore);
    
    CC_SYNTHESIZE_READONLY(MCEquipment *, equipment_, Equipment);
    CC_SYNTHESIZE(MCOre *, ore_, Ore); /* 装备所用矿石 */
};

#endif /* defined(__Military_Confrontation__MCEquipmentItem__) */
