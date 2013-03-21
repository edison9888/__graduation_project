//
//  MCTeam.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTeam.h"
#include "MCHero.h"

#pragma mark *** MCTeam ***

static MCTeam *__shared_team = NULL;

MCTeam::MCTeam()
{
    maxSize_ = 5;
    teams_ = CCArray::createWithCapacity(maxSize_);
    teams_->retain();
}

MCTeam::~MCTeam()
{
    CC_SAFE_RELEASE(teams_);
}

MCTeam *
MCTeam::sharedTeam()
{
    if (__shared_team) {
        __shared_team = new MCTeam;
    }
    
    return __shared_team;
}

CCArray *
MCTeam::getRoles()
{
    return teams_;
}

MCRole *
MCTeam::roleAtIndex(mc_index_t anIndex)
{
    if (anIndex < teams_->count()) {
        return (MCRole *) teams_->objectAtIndex(anIndex);
    }
    
    return NULL;
}

bool
MCTeam::hasRole(MCRole *aRole)
{
    CCObject *obj;
    MCRole *role;
    mc_object_id_t aRoleId = aRole->getID();
    CCARRAY_FOREACH(teams_, obj) {
        role = (MCRole *)obj;
        if (MCObjectIdIsEqualsTo(role->getID(), aRoleId)) {
            return true;
        }
    }
    
    return false;
}

void
MCTeam::addRole(MCRole *aRole)
{
    if (!hasRole(aRole)) {
        teams_->addObject(aRole);
    }
}

void
MCTeam::removeRole(MCRole *aRole)
{
    teams_->removeObject(aRole);
}

mc_size_t
MCTeam::size()
{
    return teams_->count();
}
