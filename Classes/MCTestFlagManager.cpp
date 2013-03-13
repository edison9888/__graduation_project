//
//  MCTestFlagManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTestFlagManager.h"
#include "MCFlagManager.h"

bool
MCTestFlagManager::init()
{
    if (MCTestLayer::initWithColor(ccc4(64, 64, 64, 255))) {
        MCFlagManager::sharedFlagManager();
        return true;
    }
    
    return false;
}
