//
//  MCEffectiveItem.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEffectiveItem.h"

bool
MCEffectiveItem::init()
{
    if (MCItem::init()) {
        effect.hp = 0;
        effect.pp = 0;
        effect.positive_state = 0;
        effect.negative_state = 0;
        
        return true;
    }
    
    return false;
}
