//
//  MCEquipmentItem.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEquipmentItem.h"
#include "MCSkillManager.h"

MCEquipmentItem::~MCEquipmentItem()
{
    CC_SAFE_RELEASE(equipment_);
}

bool
MCEquipmentItem::init(MCEquipmentType aEquipmentType)
{
    if (MCItem::init()) {
        if (aEquipmentType == MCEquipment::MCWeapon) {
            equipment_ = new MCWeapon;
        } else if (aEquipmentType == MCEquipment::MCArmor) {
            equipment_ = new MCArmor;
        } else {
            equipment_ = new MCEquipment;
        }
        
        damage_ = -1;
        armorCheckPenalty_ = -1;
        criticalHit_ = -1;

        attackCheck_ = -1;
        ac_ = -1;
        
        return true;
    }
    
    return false;
}

MCEquipmentItem *
MCEquipmentItem::create(MCEquipmentType aEquipmentType)
{
    MCEquipmentItem *equipmentItem = new MCEquipmentItem;
    
    if (equipmentItem && equipmentItem->init(aEquipmentType)) {
        equipmentItem->autorelease();
    } else {
        CC_SAFE_DELETE(equipmentItem);
        equipmentItem = NULL;
    }
    
    return equipmentItem;
}


CCArray *
MCEquipmentItem::skills()
{
    return MCSkillManager::sharedSkillManager()->skillsForSkillType(id_.sub_class_);
}

CCObject *
MCEquipmentItem::copy()
{
    MCEquipmentItem *equipmentItem = new MCEquipmentItem;
    
    equipmentItem->init(equipment_->type);
    equipmentItem->id_ = id_;
    equipmentItem->tag_ = tag_;
    equipmentItem->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    equipmentItem->name_->retain();
    if (description_ != NULL) {
        equipmentItem->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
        equipmentItem->description_->retain();
    }
    equipmentItem->price_ = price_;
    equipmentItem->itemType_ = itemType_;
    equipmentItem->icon_ = icon_;
    if (equipment_->type == MCEquipment::MCWeapon) {
        equipmentItem->equipment_ = dynamic_cast<MCEquipment *>(dynamic_cast<MCWeapon *>(equipment_)->copy());
    } else if (equipment_->type == MCEquipment::MCArmor) {
        equipmentItem->equipment_ = dynamic_cast<MCEquipment *>(dynamic_cast<MCArmor *>(equipment_)->copy());
    } else {
        CCAssert(equipment_->type != MCEquipment::MCUnknownEquipment, "MCUnknownEquipment");
    }
    equipmentItem->ore_ = ore_;
    
    return equipmentItem;
}

/**
 * 若非武器则返回-1
 */
mc_dice_unit_t
MCEquipmentItem::getAttackCheck()
{
    if (equipment_->type != MCEquipment::MCWeapon) {
        return -1;
    }
    if (attackCheck_ == -1) {
        attackCheck_ = ore_->getWeaponDexterity() + equipment_->dexterity;
    }
    
    return attackCheck_ + MCDiceMaker::sharedDiceMaker()->attackCheck();
}

/**
 * 若非武器则返回-1
 */
mc_damage_t
MCEquipmentItem::getDamage()
{
    if (equipment_->type != MCEquipment::MCWeapon) {
        return -1;
    }
    if (damage_ == -1) {
        damage_ = ore_->getDamage();
    }
    MCDice *damageDice = MCDiceMaker::sharedDiceMaker()->diceWithType(dynamic_cast<MCWeapon *>(equipment_)->damage);
    
    return damage_ + damageDice->roll();
}

/**
 * 若非武器则返回-1
 */
mc_critical_hit_t
MCEquipmentItem::getCriticalHit()
{
    if (equipment_->type != MCEquipment::MCWeapon) {
        return -1;
    }
    if (criticalHit_ == -1) {
        criticalHit_ = ore_->getCriticalHit() + dynamic_cast<MCWeapon *>(equipment_)->criticalHit;
    }
    
    return criticalHit_;
}

/**
 * 若非防具则返回-1
 */
mc_ac_t
MCEquipmentItem::getAC()
{
    if (equipment_->type != MCEquipment::MCArmor) {
        return -1;
    }
    if (ac_ == -1) {
        ac_ = ore_->getArmorDexterity() + dynamic_cast<MCArmor *>(equipment_)->defense + equipment_->dexterity;
    }
    
    return ac_;
}

/**
 * 若非防具则返回-1
 */
mc_ac_t
MCEquipmentItem::getArmorCheckPenalty()
{
    if (equipment_->type != MCEquipment::MCArmor) {
        return -1;
    }
    if (armorCheckPenalty_ == -1) {
        armorCheckPenalty_ = ore_->getArmorCheckPenalty() + dynamic_cast<MCArmor *>(equipment_)->armorCheckPenalty;
    }
    
    return armorCheckPenalty_;
}

