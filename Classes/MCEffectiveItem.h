//
//  MCEffectiveItem.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEffectiveItem__
#define __Military_Confrontation__MCEffectiveItem__

#include "MCItem.h"
#include "MCEffect.h"

class MCEffectiveItem : public MCItem {
public:
    bool init();
    
    CREATE_FUNC(MCEffectiveItem);
    
    MCEffect effect;
};

#endif /* defined(__Military_Confrontation__MCEffectiveItem__) */
