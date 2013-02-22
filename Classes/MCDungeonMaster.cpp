//
//  MCDungeonMaster.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCDungeonMaster.h"

static MCDungeonMaster *__shared_dungeon_master = NULL;

bool
MCDungeonMaster::_init()
{
    return true;
}

MCDungeonMaster *
MCDungeonMaster::sharedDungeonMaster()
{
    if (__shared_dungeon_master == NULL) {
        __shared_dungeon_master = new MCDungeonMaster;
        if (__shared_dungeon_master && __shared_dungeon_master->_init()) {
        } else {
            delete __shared_dungeon_master;
            __shared_dungeon_master = NULL;
        }
    }
    
    return __shared_dungeon_master;
}
