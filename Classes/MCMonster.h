//
//  MCMonster.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMonster__
#define __Military_Confrontation__MCMonster__

#include "MCRole.h"

/* 基础怪物 */
class MCMonster : public MCRole {
public:
    bool init();
    bool init(MCRoleRace aRoleRace);
    
    static MCMonster *create(mc_object_id_t anObjectId);
};

#endif /* defined(__Military_Confrontation__MCMonster__) */
