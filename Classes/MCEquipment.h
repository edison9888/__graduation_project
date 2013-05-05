//
//  MCEquipment.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEquipment__
#define __Military_Confrontation__MCEquipment__

#include "MCType.h"
#include "MCDice.h"
#include "MCEquipmentProperty.h"
#include "MCRoleProperty.h"
#include "MCEffect.h"

typedef mc_enum_t MCEquipmentType;

class MCEquipment : public CCObject {
public:
    enum {
        MCUnknownEquipment = 0,				/* 未知装备类型 */
        MCWeapon           = MCMakeEnum(0),	/* 武器 */
        MCArmor            = MCMakeEnum(1)	/* 防具 */
    };
public:
    MCEquipment();
    
	MCEquipmentType type;     /* 装备类型 */
    mc_dexterity_t dexterity; /* 敏捷调整值 */
};

class MCWeapon : public MCEquipment {
public:
    MCWeapon();
    ~MCWeapon();
    
    CCObject *copy();
    
    mc_pp_t consume; /* 攻击体力消耗 */
    mc_dice_unit_t damage; /* 伤害值 */
    mc_critical_hit_t criticalHit; /* 重击倍数 */
    MCDiceRange criticalHitVisible; /* 可视区域内重击范围 */
    MCDiceRange criticalHitInvisible; /* 非可视区域内重击范围 */
    mc_distance_t distance; /* 攻击距离，单位为资源缩放值*32像素 */
    MCRoleState effect; /* 附带效果 */
    MCDiceRange effectCheck; /* 效果判定 */
    
    MCEffect *attackEffect; /* 攻击效果 */
};


class MCArmor : public MCEquipment {
public:
    MCArmor();
    
    CCObject *copy();
    
    mc_dexterity_t defense; /* 防御加值 */
    mc_armor_check_penalty_t armorCheckPenalty; /* 防具检定减值 */
};

#endif /* defined(__Military_Confrontation__MCEquipment__) */
