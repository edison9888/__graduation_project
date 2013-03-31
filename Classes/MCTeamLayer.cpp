//
//  MCTeamLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTeamLayer.h"
#include "MCActionBar.h"

MCTeamLayer::~MCTeamLayer()
{
    CC_SAFE_RELEASE(selectedRoles_);
}

bool
MCTeamLayer::init()
{
    if (CCLayer::init()) {
        group_ = MCRoleBaseInfoGroup::create();
        addChild(group_);
        selectedRoles_ = CCArray::create();
        selectedRoles_->retain();
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
    selectedRoles_->removeAllObjects();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        info->setSelected(true);
        selectedRoles_->addObject(info->getRole());
    }
}

void
MCTeamLayer::unselectAll()
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    selectedRoles_->removeAllObjects();
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
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (role == aRole) {
            info->selected();
            selectedRoles_->addObject(role);
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
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (role == aRole) {
            info->unselected();
            selectedRoles_->removeObject(role);
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
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        role = info->getRole();
        if (role == aRole) {
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

MCRoleBaseInfo *
MCTeamLayer::roleBaseInfoForTouch(CCTouch *aTouch)
{
    CCPoint touchLocation = aTouch->getLocation();
    
    if (group_->size() > 0) {
        CCObject *obj = NULL;
        CCArray *rolesInfo = group_->infoList_;
        
        CCARRAY_FOREACH(rolesInfo, obj) {
            MCRoleBaseInfo *info = dynamic_cast<MCRoleBaseInfo *>(obj);
            if (info && info->isVisible()) {
                CCNode *faceBox = info->getFaceBox();
                CCPoint local = faceBox->convertToNodeSpace(touchLocation);
                CCSize s = faceBox->getContentSize();
                CCRect r = CCRectMake(0, 0, s.width, s.height);
                
                if (r.containsPoint(local)) {
                    return info;
                }
            }
        }
    }
    
    return NULL;
}


/**
 * 确实发生碰撞返回碰撞的人物，否则返回NULL
 */
MCRoleBaseInfo *
MCTeamLayer::collidesWithActionBarItem(MCActionBarItem *anActionBarItem)
{
    if (anActionBarItem->getBackpackItem()->item->getID().class_ != 'P') { /* 不是药水 */
        return NULL;
    }
    CCPoint p = anActionBarItem->getPosition();
    CCSize s = anActionBarItem->getContentSize();
    CCRect actionBarItemAABB = CCRectMake(p.x - s.width / 2, p.y, s.width, s.height); /* 计算锚点 */
    
    if (group_->size() > 0) {
        CCObject *obj = NULL;
        CCArray *rolesInfo = group_->infoList_;
        
        CCARRAY_FOREACH(rolesInfo, obj) {
            MCRoleBaseInfo *info = dynamic_cast<MCRoleBaseInfo *>(obj);
            if (info && info->isVisible()) {
                CCNode *faceBox = info->getFaceBox();
                CCPoint local = info->getPosition();
                CCSize s = faceBox->getContentSize();
                CCRect r = CCRectMake(local.x, local.y - s.height, s.width, s.height); /* local.y-s.width是因为锚点在左上角 */
                if (r.intersectsRect(actionBarItemAABB)) {
                    return info;
                }
            }
        }
    }
    
    return NULL;
}


/**
 * 名字有点蛋疼，其实意思是检测item和人物头像的碰撞，若有碰撞则半透明化
 */
void
MCTeamLayer::acceptActionBarItem(MCActionBarItem *anActionBarItem)
{
    if (anActionBarItem->getBackpackItem()->item->getID().class_ != 'P') { /* 不是药水 */
        return;
    }
    CCPoint p = anActionBarItem->getPosition();
    CCSize s = anActionBarItem->getContentSize();
    CCRect actionBarItemAABB = CCRectMake(p.x - s.width / 2, p.y, s.width, s.height); /* 计算锚点 */
    bool found = false;
    
    if (group_->size() > 0) {
        CCObject *obj = NULL;
        CCArray *rolesInfo = group_->infoList_;
        
        CCARRAY_FOREACH(rolesInfo, obj) {
            MCRoleBaseInfo *info = dynamic_cast<MCRoleBaseInfo *>(obj);
            if (info && info->isVisible()) {
                CCScale9Sprite *faceBox = info->getFaceBox();
                CCPoint local = info->getPosition();
                CCSize s = faceBox->getContentSize();
                CCRect r = CCRectMake(local.x, local.y - s.height, s.width, s.height); /* local.y-s.width是因为锚点在左上角 */
                if (r.intersectsRect(actionBarItemAABB) && !found) {
                    info->setOpacity(160);
                    found = true;
                } else {
                    info->setOpacity(255);
                }
            }
        }
    }
}
