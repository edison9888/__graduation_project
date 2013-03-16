//
//  MCEnemy.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEnemy__
#define __Military_Confrontation__MCEnemy__

#include "MCRole.h"

/* 基础怪物 */
class MCEnemy : public MCRole {
public:
    bool init(MCRoleRace aRoleRace);
    
    static MCEnemy *create(mc_object_id_t anObjectId);
    
    CCObject *copy();
};

#endif /* defined(__Military_Confrontation__MCEnemy__) */
