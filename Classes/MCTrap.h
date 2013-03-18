//
//  MCTrap.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTrap__
#define __Military_Confrontation__MCTrap__

#include "MCEffectiveItem.h"

class MCTrap : public MCEffectiveItem {
public:
    bool init(mc_object_id_t anObjectId);
    
    static MCTrap *create(mc_object_id_t anObjectId);
    
    CCObject *copy();
};

#endif /* defined(__Military_Confrontation__MCTrap__) */
