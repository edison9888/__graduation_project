//
//  MCTaskTarget.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTaskTarget.h"

CCObject *
MCTaskTarget::copy()
{
    MCTaskTarget *target = new MCTaskTarget;
    
    target->objectID = objectID;
    target->count = count;
    
    return target;
}
