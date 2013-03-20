//
//  MCOre.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-20.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCOre__
#define __Military_Confrontation__MCOre__

#include "MCObject.h"
#include "MCEquipmentProperty.h"

class MCOre : public MCObject {
    CC_SYNTHESIZE(mc_damage_t, damage_, Damage); /* 伤害调整值 */
    CC_SYNTHESIZE(mc_critical_hit_t, criticalHit_, CriticalHit); /* 重击调整值 */
    CC_SYNTHESIZE(mc_dexterity_t, weaponDexterity_, WeaponDexterity); /* 敏捷调整值(武器) */
    CC_SYNTHESIZE(mc_dexterity_t, armorDexterity_, ArmorDexterity); /* 敏捷调整值(防具) */
    CC_SYNTHESIZE(mc_armor_check_penalty_t, armorCheckPenalty_, ArmorCheckPenalty); /* 防具检定调整值 */
    CC_SYNTHESIZE(mc_price_t, price_, Price); /* 升级所需价格(马克) */
    CC_SYNTHESIZE(MCOre *, nextLevel, NextLevel); /* 下一级矿石 */
};

#endif /* defined(__Military_Confrontation__MCOre__) */
