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
#include "MCEffect.h"

enum {
	MCUnknownEquipment = 0,				/* 未知装备类型 */
	MCWeapon           = MCMakeEnum(0),	/* 武器 */
	MCArmor            = MCMakeEnum(1)	/* 防具 */
};
typedef mc_enum_t MCEquipmentType;

typedef struct {
    mc_dexterity_t price; /* 升级基础价格 */
    mc_dice_unit_t damage; /* 伤害值 */
    MCDiceRange critical_hit_visible; /* 可视区域内重击范围 */
    MCDiceRange critical_hit_invisible; /* 非可视区域内重击范围 */
    mc_critical_hit_t critical_hit; /* 重击倍数 */
    mc_distance_t distance; /* 攻击距离，单位为资源缩放值*32像素 */
    MCDamageType damage_type; /* 伤害类型 */
    MCRoleState effect; /* 附带效果 */
    MCDiceRange effect_check; /* 效果判定 */
    mc_dexterity_t dexterity; /* 敏捷调整值 */
} mc_weapon_t;

typedef struct {
    mc_dexterity_t defense; /* 防御加值 */
    mc_dexterity_t dexterity; /* 敏捷调整值 */
    mc_armor_check_penalty_t armor_check_penalty; /* 防具检定减值 */
    mc_dexterity_t price; /* 升级基础价格 */
} mc_armor_t;

struct MCEquipment {
	union {
		mc_weapon_t weapon;
		mc_armor_t armor;
	} equipment;
	MCEquipmentType type;
};

#endif /* defined(__Military_Confrontation__MCEquipment__) */
