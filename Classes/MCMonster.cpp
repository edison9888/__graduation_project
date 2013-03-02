//
//  MCMonster.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCMonster.h"

bool
MCMonster::init()
{
    setRoleType(MCEnemy);
    
    return true;
}

bool
MCMonster::init(MCRoleRace aRoleRace)
{
    setRoleRace(aRoleRace);
    
    return true;
}

MCMonster *
MCMonster::create(mc_object_id_t anObjectId)
{
    MCMonster *monster = new MCMonster;
    
    if (monster && monster->init()) {
        monster->autorelease();
        /* 加载monster数据 */
        /* 检测怪物种族 */
    } else {
        CC_SAFE_DELETE(monster);
        monster = NULL;
    }
    
    return monster;
}
