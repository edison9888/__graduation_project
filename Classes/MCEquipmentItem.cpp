//
//  MCEquipmentItem.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEquipmentItem.h"

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
        equipment_->autorelease();
        equipment_->retain();
        
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

CCObject *
MCEquipmentItem::copy()
{
    MCEquipmentItem *equipmentItem = new MCEquipmentItem;
    
    equipmentItem->attackCheck_ = -1;
    equipmentItem->ac_ = -1;
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

