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
#include "MCRoleProperty.h"

typedef mc_short_t mc_hp_change_value_t;
typedef float mc_pp_change_value_t;
typedef float mc_lasting_time_t;

typedef mc_short_t mc_radius_t;

class MCItemManager;

class MCEffectiveItem : public MCItem {
    friend class MCItemManager;
public:
    ~MCEffectiveItem();
    bool init();
    
    CREATE_FUNC(MCEffectiveItem);
    
    CCObject *copy();
    
    mc_hp_change_value_t hp;             /* HP变化值 */
    mc_pp_change_value_t pp;             /* PP变化值 */
    MCRoleState          positive_state; /* 会增加的状态 */
    MCRoleState          negative_state; /* 会消除的状态 */
    mc_hp_change_value_t adjusted_hp;    /* 一次调度后hp的变化值 */
    mc_hp_change_value_t adjusted_pp;    /* 一次调度后pp的变化值 */
    mc_lasting_time_t    lasting_time;   /* 效果时间 */
    
    CC_SYNTHESIZE(mc_radius_t, radius_, Radius); /* 效果区域半径 */
    CC_SYNTHESIZE(CCString *, path_, Path); /* 效果路径 */
};

#endif /* defined(__Military_Confrontation__MCEffectiveItem__) */
