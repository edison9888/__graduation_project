//
//  MCEnemy.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEnemy.h"

bool
MCEnemy::init(MCRoleRace aRoleRace)
{
    setRoleRace(aRoleRace);
    
    return true;
}

MCEnemy *
MCEnemy::create(mc_object_id_t anObjectId)
{
    MCEnemy *enemy = new MCEnemy;
    
    if (enemy && enemy->init(MCTerrestrial)) {
        enemy->setID(anObjectId);
        enemy->autorelease();
        /* 加载enemy数据 */
        /* 检测怪物种族 */
    } else {
        CC_SAFE_DELETE(enemy);
        enemy = NULL;
    }
    
    return enemy;
}

CCObject *
MCEnemy::copy()
{
    MCEnemy *enemy = new MCEnemy;
    
    /* 浅copy是也 */
    enemy->name_ = name_;
    enemy->description_ = description_;
    
    enemy->roleRace_ = roleRace_;
    enemy->hp_ = hp_;
    enemy->pp_ = pp_;
    enemy->roleState_ = roleState_;
    enemy->face_ = face_;
    enemy->spriteSheet_ = spriteSheet_;
    enemy->ai_ = ai_;
    
    return enemy;
}
