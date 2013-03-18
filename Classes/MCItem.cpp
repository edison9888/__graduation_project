//
//  MCItem.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCItem.h"

bool
MCItem::init()
{
    id_ = NULLObjectId;
    tag_ = CC_INVALID_INDEX;
    name_ = NULL;
    description_ = NULL;
    itemType_ = MCUnknownItem;
    
    return true;
}
