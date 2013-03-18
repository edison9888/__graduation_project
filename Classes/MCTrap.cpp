//
//  MCTrap.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTrap.h"

bool
MCTrap::init(mc_object_id_t anObjectId)
{
    if (MCEffectiveItem::init()) {
        setID(anObjectId);
        return true;
    }
    
    return false;
}

MCTrap *
MCTrap::create(mc_object_id_t anObjectId)
{
    MCTrap *trap = new MCTrap;
    
    if (trap && trap->init(anObjectId)) {
        trap->autorelease();
    } else {
        CC_SAFE_DELETE(trap);
        trap = NULL;
    }
    
    return trap;
}

CCObject *
MCTrap::copy()
{
    MCTrap *trap = new MCTrap;
    
    trap->setID(id_);
    trap->setTag(tag_);
    trap->setName(name_);
    trap->setDescription(description_);
    trap->setItemType(itemType_);
    trap->effect = effect;
    
    return trap;
}
