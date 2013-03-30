//
//  MCEquipmentItem.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEquipmentItem.h"

bool
MCEquipmentItem::init(MCEquipmentType aEquipmentType)
{
    if (MCItem::init()) {
        equipment_.type = aEquipmentType;
        
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
    
    equipmentItem->id_ = id_;
    equipmentItem->tag_ = tag_;
    equipmentItem->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    equipmentItem->name_->retain();
    equipmentItem->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
    equipmentItem->description_->retain();
    equipmentItem->itemType_ = itemType_;
    equipmentItem->icon_ = icon_;
    equipmentItem->equipment_ = equipment_;
    equipmentItem->ore_ = ore_;
    
    return equipmentItem;
}

