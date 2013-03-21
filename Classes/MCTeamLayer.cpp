//
//  MCTeamLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTeamLayer.h"


MCTeamLayer::~MCTeamLayer()
{
    CC_SAFE_RELEASE(selecredRoles_);
    CC_SAFE_RELEASE(group_);
}

bool
MCTeamLayer::init()
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
MCTeamLayer::selectAll()
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
MCTeamLayer::unselectAll()
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
MCTeamLayer::selectRole(MCRole *aRole)
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    MCRole *role;
    mc_object_id_t aRoleId = aRole->getID();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (MCObjectIdIsEqualsTo(role->getID(), aRoleId)) {
            info->selected();
            selecredRoles_->addObject(role);
            break;
        }
    }
}

void
MCTeamLayer::unselectRole(MCRole *aRole)
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    MCRole *role;
    mc_object_id_t aRoleId = aRole->getID();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (MCObjectIdIsEqualsTo(role->getID(), aRoleId)) {
            info->unselected();
            selecredRoles_->removeObject(role);
            break;
        }
    }
}

MCRoleBaseInfo *
MCTeamLayer::infoForRole(MCRole *aRole)
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    MCRole *role;
    mc_object_id_t aRoleId = aRole->getID();
    
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (MCObjectIdIsEqualsTo(role->getID(), aRoleId)) {
            return info;
        }
    }
    
    return NULL;
}

bool
MCTeamLayer::hasRole(MCRole *aRole)
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    MCRole *role;
    mc_object_id_t aRoleId = aRole->getID();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (MCObjectIdIsEqualsTo(role->getID(), aRoleId)) {
            return true;
        }
    }
    
    return false;
}

void
MCTeamLayer::addRole(MCRole *aRole)
{
    if (!hasRole(aRole)) {
        MCRoleBaseInfo *info = MCRoleBaseInfo::create(aRole);
        group_->addRoleBaseInfo(info);
    }
}

void
MCTeamLayer::removeRole(MCRole *aRole)
{
    MCRoleBaseInfo *info = infoForRole(aRole);
    if (info) {
        group_->removeRoleBaseInfo(info);
    }
}

mc_size_t
MCTeamLayer::size()
{
    return group_->size();
}
