//
//  MCTestActionMenu.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTestActionMenu.h"

bool
MCTestActionMenu::init()
{
    if (MCTestLayer::initWithColor(ccc4(64, 128, 240, 255))) {
        setTouchEnabled(true);
        return true;
    }
    
    return false;
}
