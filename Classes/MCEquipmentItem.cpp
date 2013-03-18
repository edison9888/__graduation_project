//
//  MCEquipmentItem.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEquipmentItem.h"

bool
MCEquipmentItem::init()
{
    if (MCItem::init()) {
        equipment.type = MCUnknownEquipment;
        
        return true;
    }
    
    return false;
}

bool
MCEquipmentItem::init(MCEquipmentType aEquipmentType)
{
    if (MCItem::init()) {
        equipment.type = aEquipmentType;
        
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
