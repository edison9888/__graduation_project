//
//  MCNPC.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCNPC.h"

MCNPC::~MCNPC()
{
    CC_SAFE_DELETE(viewport_);
}

bool
MCNPC::init()
{
    setRoleType(MCNPCType);
#warning debug
    loadSpriteSheet("spritesheets/n-13");
    
    viewport_ = MCViewport::create(getEntity());
    
    return true;
}

MCNPC *
MCNPC::create(mc_object_id_t anObjectId)
{
    MCNPC *npc = new MCNPC;
    
    if (npc && npc->init()) {
        npc->autorelease();
        /* 加载NPC数据 */
    } else {
        CC_SAFE_DELETE(npc);
        npc = NULL;
    }
    
    return npc;
}
