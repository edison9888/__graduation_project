//
//  MCItem.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCItem__
#define __Military_Confrontation__MCItem__

#include "MCObject.h"

typedef mc_enum_t MCItemType;

class MCItem : public MCObject {
public:
    /* 物品类型 */
    enum {
        MCUnknownItem   = 0,             /* 未知物品 */
        MCNormalItem    = MCMakeEnum(0), /* 普通物品 */
        MCEquipmentItem = MCMakeEnum(1)  /* 装备物品 */
    };
    
    bool init();
    
    CC_SYNTHESIZE(MCItemType, itemType_, ItemType); /* 物品类型 */
};

#endif /* defined(__Military_Confrontation__MCItem__) */
