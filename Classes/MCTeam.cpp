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
    if (__shared_team == NULL) {
        __shared_team = new MCTeam;
        __shared_team->addRole(MCHero::sharedHero());
    }
    
    return __shared_team;
}

/* 恢复人物状态 */
void
MCTeam::restoreRoles()
{
    CCObject *obj;
    CCARRAY_FOREACH(teams_, obj) {
        dynamic_cast<MCRole *>(obj)->restoreState();
    }
}

/* hero失踪之谜！折中解决方案~ */
void
MCTeam::ensureHero()
{
    MCHero *hero = MCHero::sharedHero();
    
    if (!hasRole(hero)) {
        teams_->insertObject(hero, 0);
    }
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
        return dynamic_cast<MCRole *>(teams_->objectAtIndex(anIndex));
    }
    
    return NULL;
}

bool
MCTeam::hasRole(MCRole *aRole)
{
    CCObject *obj;
    MCRole *role;
    CCARRAY_FOREACH(teams_, obj) {
        role = dynamic_cast<MCRole *>(obj);
        if (role == aRole) {
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
