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

typedef mc_short_t mc_radius_t;

class MCItemManager;

class MCEffectiveItem : public MCItem {
    friend class MCItemManager;
public:
    ~MCEffectiveItem();
    bool init();
    
    CREATE_FUNC(MCEffectiveItem);
    
    CCObject *copy();
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCEffect, effect_, Effect);
    CC_SYNTHESIZE(mc_radius_t, radius_, Radius); /* 效果区域半径 */
    CC_SYNTHESIZE(CCString *, path_, Path); /* 效果路径 */
};

#endif /* defined(__Military_Confrontation__MCEffectiveItem__) */
