//
//  MCTestFlagManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTestRoleManager.h"
#include "MCRoleManager.h"
#include "MCNPC.h"
#include "MCMonster.h"

bool
MCTestRoleManager::init()
{
    if (MCTestLayer::initWithColor(ccc4(64, 64, 64, 255))) {
        MCRoleManager::sharedRoleManager();
        
        mc_object_id_t oid = {
            'N', '0', '1', '8'
        };
        MCNPC *npc = MCRoleManager::sharedRoleManager()->NPCForObjectId(oid);
        npc->loadSpriteSheet();
        addChild(npc->getEntity()->getSpriteSheet(), 20000);
        npc->getEntity()->setPosition(ccp(100, 200));
        
        return true;
    }
    
    return false;
}
