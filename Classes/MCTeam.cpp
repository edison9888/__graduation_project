//
//  MCTeam.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTeam.h"


#pragma mark -
#pragma mark *** MCTeam ***

MCTeam::~MCTeam()
{
    CC_SAFE_RELEASE(selecredRoles_);
    CC_SAFE_RELEASE(group_);
}

bool
MCTeam::init()
{
    if (CCLayer::init()) {
        group_ = MCRoleBaseInfoGroup::create();
        group_->retain();
        selecredRoles_ = CCArray::create();
        selecredRoles_->retain();
        isMultiSeletionMode_ = false;
        return true;
    }
    
    return false;
}

void
MCTeam::selectAll()
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    selecredRoles_->removeAllObjects();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        info->setSelected(true);
        selecredRoles_->addObject(info->getRole());
    }
}

void
MCTeam::unselectAll()
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    selecredRoles_->removeAllObjects();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        info->setSelected(false);
    }
}

void
MCTeam::selectRole(MCRole *aRole)
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    MCRole *role;
    mc_object_id_t aRoleId = aRole->getID();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (MCObjectIdIsEqualsTo(role->getID(), aRoleId) {
            info->selected();
            selecredRoles_->addObject(role);
            break;
        }
    }
}
  
void      
MCTeam::unselectRole(MCRole *aRole)
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    MCRole *role;
    mc_object_id_t aRoleId = aRole->getID();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (MCObjectIdIsEqualsTo(role->getID(), aRoleId) {
            info->unselected();
            selecredRoles_->removeObject(role);
            break;
        }
    }
}

MCRoleBaseInfo *
MCTeam::infoForRole(MCRole *aRole)
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    MCRole *role;
    mc_object_id_t aRoleId = aRole->getID();
    
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (MCObjectIdIsEqualsTo(role->getID(), aRoleId) {
            return info;
        }
    }
    
    return NULL;
}

bool
MCTeam::hasRole(MCRole *aRole)
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    MCRole *role;
    mc_object_id_t aRoleId = aRole->getID();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (MCObjectIdIsEqualsTo(role->getID(), aRoleId) {
            return true;
        }
    }
    
    return false;
}

void
MCTeam::addRole(MCRole *aRole)
{
    if (!hasRole(aRole)) {
        MCRoleBaseInfo *info = MCRoleBaseInfo::create(aRole);
        group_->addRoleBaseInfo(info);
    }
}

void
MCTeam::removeRole(MCRole *aRole)
{
    MCRoleBaseInfo *info = infoForRole(aRole);
    if (info) {
        group_->removeRoleBaseInfo(info);
    }
}

mc_size_t
MCTeam::size()
{
    return group_->size();
}
