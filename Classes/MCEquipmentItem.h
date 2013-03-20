//
//  MCEquipmentItem.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEquipmentItem__
#define __Military_Confrontation__MCEquipmentItem__

#include "MCItem.h"
#include "MCEquipment.h"
#include "MCOre.h"

class MCItemManager;

class MCEquipmentItem : public MCItem {
    friend class MCItemManager;
public:
    bool init(MCEquipmentType aEquipmentType);
    
    static MCEquipmentItem *create(MCEquipmentType aEquipmentType);
    
    CCObject *copy();
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCEquipment, equipment_, Equipment);
    CC_SYNTHESIZE(MCOre *, ore_, Ore); /* 装备所用矿石 */
};

#endif /* defined(__Military_Confrontation__MCEquipmentItem__) */
