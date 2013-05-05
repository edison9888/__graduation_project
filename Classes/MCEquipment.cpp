//
//  MCEquipment.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEquipment.h"

#pragma mark *** MCEquipment ***

MCEquipment::MCEquipment()
{
    type = MCUnknownEquipment;
    dexterity = 0;
}

#pragma mark -
#pragma mark *** MCWeapon ***

MCWeapon::MCWeapon()
{
    type = MCEquipment::MCWeapon;
    damage = 0; /* 伤害值 */
    criticalHit = 0; /* 重击倍数 */
    /* 可视区域内重击范围 */
    criticalHitVisible.min = 0;
    criticalHitVisible.max = 0;
    criticalHitVisible.dice = 0;
    /* 非可视区域内重击范围 */
    criticalHitInvisible.min = 0;
    criticalHitInvisible.max = 0;
    criticalHitInvisible.dice = 0;
    distance = 0; /* 攻击距离，单位为资源缩放值*32像素 */
    effect = MCNormalState; /* 附带效果 */
    /* 效果判定 */
    effectCheck.min = 0;
    effectCheck.max = 0;
    effectCheck.dice = 0;
}

MCWeapon::~MCWeapon()
{
    CC_SAFE_RELEASE(attackEffect);
}

CCObject *
MCWeapon::copy()
{
    MCWeapon *weapon = new MCWeapon;
    
    weapon->type = type;
    weapon->consume = consume;
    weapon->dexterity = dexterity;
    weapon->damage = damage;
    weapon->criticalHit = criticalHit;
    weapon->criticalHitVisible = criticalHitVisible;
    weapon->criticalHitInvisible = criticalHitInvisible;
    weapon->distance = distance;
    weapon->effect = effect;
    weapon->effectCheck = effectCheck;
    weapon->attackEffect = dynamic_cast<MCEffect *>(attackEffect->copy());
    
    return weapon;
}

#pragma mark -
#pragma mark *** MCArmor ***

MCArmor::MCArmor()
{
    type = MCEquipment::MCArmor;
    defense = 0; /* 防御加值 */
    armorCheckPenalty = 0; /* 防具检定减值 */
}

CCObject *
MCArmor::copy()
{
    MCArmor *armor = new MCArmor;
    
    armor->type = type;
    armor->dexterity = dexterity;
    armor->defense = defense;
    armor->armorCheckPenalty = armorCheckPenalty;
    
    return armor;
}
