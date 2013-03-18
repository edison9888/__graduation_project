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

class MCEquipmentItem : public MCItem {
public:
    bool init();
    bool init(MCEquipmentType aEquipmentType);
    
    CREATE_FUNC(MCEquipmentItem);
    static MCEquipmentItem *create(MCEquipmentType aEquipmentType);
    
    MCEquipment equipment;
};

#endif /* defined(__Military_Confrontation__MCEquipmentItem__) */
