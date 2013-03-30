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
}

MCTaskContext::~MCTaskContext()
{
    delete trapWide_;
    delete trapDamage_;
    delete healthPotion_;
    delete physicalPotion_;
}
