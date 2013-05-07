//
//  MCTeamLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTeamLayer.h"
#include "MCActionBar.h"
#include "MCShadow.h"
#include "MCEffect.h"

static const char *kMCTeamLayerItemOpenedFilepath = "UI/tl_opened.png";
static const char *kMCTeamLayerItemOpenedSelectedFilepath = "UI/tl_opened_selected.png";
static const char *kMCTeamLayerItemClosedFilepath = "UI/tl_closed.png";
static const char *kMCTeamLayerItemClosedSelectedFilepath = "UI/tl_closed_selected.png";

static const float kMCActionDuration = 0.2f;

MCTeamLayer::~MCTeamLayer()
{
#if MC_SELECT_ALL_SUPPORT == 1
    CC_SAFE_RELEASE(selectedRoles_);
#else
    CC_SAFE_RELEASE(selectedRole_);
#endif
}

bool
MCTeamLayer::init()
{
    if (CCLayer::init()) {
        group_ = MCRoleBaseInfoGroup::create();
        addChild(group_);
        frameSize_ = CCSizeMake(group_->width_, group_->height_);
        
        CCMenuItemImage *opened = CCMenuItemImage::create(kMCTeamLayerItemOpenedFilepath,
                                                          kMCTeamLayerItemOpenedSelectedFilepath);
        CCMenuItemImage *closed = CCMenuItemImage::create(kMCTeamLayerItemClosedFilepath,
                                                          kMCTeamLayerItemClosedSelectedFilepath);
        CCMenuItemToggle *toggleButton = CCMenuItemToggle::createWithTarget(this,
                                                                            menu_selector(MCTeamLayer::toggle),
                                                                            opened, closed, NULL);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint anchorPoint = ccp(0.0f, 1.0f);
        CCMenu *menu = CCMenu::createWithItem(toggleButton);
        addChild(menu);
        toggleButton->setAnchorPoint(anchorPoint);
        menu->setAnchorPoint(anchorPoint);
        menu->setPosition(ccp(0, winSize.height - frameSize_.height));
        toggleButton_ = menu;
        
#if MC_SELECT_ALL_SUPPORT == 1
        selectedRoles_ = CCArray::create();
        selectedRoles_->retain();
#else
        selectedRole_ = NULL;
#endif

        return true;
    }
    
    return false;
}

#if MC_SELECT_ALL_SUPPORT == 1
void
MCTeamLayer::selectAll()
{
    CCObject *obj;
    MCRoleBaseInfo *info;
    selectedRoles_->removeAllObjects();
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        info->selected();
        info->getRole()->getEntity()->getShadow()->selected();
        selectedRoles_->addObject(info->getRole());
    }
}
#endif

void
MCTeamLayer::unselectAll()
{
    CCObject *obj;
    MCRoleBaseInfo *info;
#if MC_SELECT_ALL_SUPPORT == 1
    selectedRoles_->removeAllObjects();
#endif
    CCARRAY_FOREACH(group_->infoList_, obj) {
        info = (MCRoleBaseInfo *)obj;
        info->unselected();
        info->getRole()->getEntity()->getShadow()->unselected();
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
            role->getEntity()->getShadow()->selected();
#if MC_SELECT_ALL_SUPPORT == 1
            selectedRoles_->addObject(role);
#else
            selectedRole_ = role;
#endif
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
            role->getEntity()->getShadow()->unselected();
#if MC_SELECT_ALL_SUPPORT == 1
            selectedRoles_->removeObject(role);
#else
            selectedRole_ = NULL;
#endif
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

const CCSize &
MCTeamLayer::getFrameSize() {
    return frameSize_;
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
    if (!anActionBarItem
        || anActionBarItem->getBackpackItem()->item->getID().class_ != 'P') { /* 不是药水 */
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

void
MCTeamLayer::selectedRolesUseActionBarItem(MCActionBarItem *anActionBarItem)
{
    CCObject *obj = NULL;
    CCArray *rolesInfo = group_->infoList_;
    
    CCARRAY_FOREACH(rolesInfo, obj) {
        MCRoleBaseInfo *info = dynamic_cast<MCRoleBaseInfo *>(obj);
        if (info && info->isSelected()) {
            if (! info->useActionBarItem(anActionBarItem)) {
                break; /* 已经不够了，不必继续下去 */
            }
            /* 物品效果 */
            dynamic_cast<MCEffectiveItem *>(anActionBarItem->getBackpackItem()->item)->effect->attach(this, info->getRole());
        }
    }
}

void
MCTeamLayer::toggle()
{
    CCPoint offset = ccp(frameSize_.width * 2, 0);
    if (group_->getPositionX() >= 0) { /* 将要隐藏 */
        offset.x = -offset.x;
    }
    group_->runAction(CCMoveBy::create(kMCActionDuration, offset));
}

