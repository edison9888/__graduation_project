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
class MCEffect;

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
    /* 实际上并无消除状态的效果 */
//    MCRoleState          negative_state; /* 会消除的状态 */
    mc_lasting_time_t    lasting_time;   /* 效果时间 */
    
    mc_radius_t radius; /* 效果区域半径 */
    
    MCEffect *effect; /* 效果 */
};

#endif /* defined(__Military_Confrontation__MCEffectiveItem__) */
