//
//  MCMercenary.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCMercenary.h"


MCMercenary::~MCMercenary()
{
    
}

bool
MCMercenary::init()
{
    setRoleRace(MCTerrans);
    
    return true;
}

MCMercenary *
MCMercenary::create(mc_object_id_t anObjectId)
{
    MCMercenary *mercenary = new MCMercenary;
    
    if (mercenary && mercenary->init()) {
        mercenary->setID(anObjectId);
        mercenary->autorelease();
    } else {
        CC_SAFE_DELETE(mercenary);
        mercenary = NULL;
    }
    
    return mercenary;
}

CCObject *
MCMercenary::copy()
{
    MCMercenary *mercenary = new MCMercenary;
    
    /* 浅copy是也 */
    mercenary->name_ = name_;
    mercenary->description_ = description_;
    
    mercenary->roleRace_ = roleRace_;
    mercenary->hp_ = hp_;
    mercenary->pp_ = pp_;
    mercenary->roleState_ = roleState_;
    mercenary->face_ = face_;
    mercenary->spriteSheet_ = spriteSheet_;
    mercenary->ai_ = ai_;
    
    return mercenary;
}