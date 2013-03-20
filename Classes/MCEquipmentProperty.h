//
//  MCEquipmentProperty.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-20.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef Military_Confrontation_MCEquipmentProperty_h
#define Military_Confrontation_MCEquipmentProperty_h

#include "MCType.h"

typedef mc_ushort_t mc_damage_t;
typedef float mc_critical_hit_t;
typedef mc_short_t mc_dexterity_t;
typedef mc_short_t mc_armor_check_penalty_t;
typedef mc_ushort_t mc_price_t;
typedef mc_ushort_t mc_distance_t; /* 攻击距离 */

enum {
    MCPuncture  = MCMakeEnum(0), /* 穿刺 */
    MCSlash     = MCMakeEnum(1), /* 挥砍 */
    MCHammer    = MCMakeEnum(2)  /* 钝击 */
};
typedef mc_enum_t MCDamageType;

#endif
