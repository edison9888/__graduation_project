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

MCNPC *
MCNPC::create(mc_object_id_t anObjectId)
{
    MCNPC *npc = new MCNPC;
    
    if (npc && npc->init()) {
        npc->setID(anObjectId);
        npc->autorelease();
        /* 加载NPC数据 */
    } else {
        CC_SAFE_DELETE(npc);
        npc = NULL;
    }
    
    return npc;
}

CCObject *
MCNPC::copy()
{
    MCNPC *npc = new MCNPC;
    
    /* 浅copy是也 */
    npc->name_ = name_;
    npc->description_ = description_;
    
    npc->roleRace_ = roleRace_;
    npc->hp_ = hp_;
    npc->pp_ = pp_;
    npc->roleState_ = roleState_;
    npc->face_ = face_;
    npc->spriteSheet_ = spriteSheet_;
#warning should design dialogue system
    
    return npc;
}
