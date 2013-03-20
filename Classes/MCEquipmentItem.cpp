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
    
    equipmentItem->setID(id_);
    equipmentItem->setTag(tag_);
    equipmentItem->setName(name_);
    equipmentItem->setDescription(description_);
    equipmentItem->setItemType(itemType_);
    equipmentItem->setIcon(icon_);
    equipmentItem->equipment_ = equipment_;
    equipmentItem->setOre(ore_);
    
    return equipmentItem;
}

