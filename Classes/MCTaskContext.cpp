//
//  MCTaskContext.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTaskContext.h"

MCTaskContext::MCTaskContext()
{
    trapWide_ = new MCBackpackItem;
    trapDamage_ = new MCBackpackItem;
    healthPotion_ = new MCBackpackItem;
    physicalPotion_ = new MCBackpackItem;
    
    hp_ = 0;
    pp_ = 0;
    damageBonus_ = 0;
    armorCheckPenalty_ = 0;
    special_ = MCNoSpecial;
}

MCTaskContext::~MCTaskContext()
{
    delete trapWide_;
    delete trapDamage_;
    delete healthPotion_;
    delete physicalPotion_;
}
