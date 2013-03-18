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

enum {
	MCUnknownEquipment = 0,				/* 未知装备类型 */
	MCWeapon           = MCMakeEnum(0),	/* 武器 */
	MCArmor            = MCMakeEnum(1)	/* 防具 */
};
typedef mc_enum_t MCEquipmentType;

typedef struct {
} mc_weapon_t;

typedef struct {
} mc_armor_t;

struct MCEquipment {
	union {
		mc_weapon_t weapon;
		mc_armor_t armor;
	} equipment;
	MCEquipmentType type;
};

#endif /* defined(__Military_Confrontation__MCEquipment__) */
