//
//  MCShadow.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-1.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCShadow.h"
#include "MCRoleManager.h"

static const CCPoint kMCShadowAnchorPoint = ccp(0.5f, 0.75f);
static const CCPoint kMCSelectedShadowAnchorPoint = ccp(0.5f, 0.5f);

MCShadow::MCShadow()
{
    role_ = NULL;
    shadow_ = NULL;
    selectedShadow_ = NULL;
}

MCShadow::~MCShadow()
{
    CC_SAFE_RELEASE(shadow_);
    CC_SAFE_RELEASE(selectedShadow_);
}

bool
MCShadow::init()
{
    if (CCSprite::init()) {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        shadow_ = cache->spriteFrameByName(kMCShadow);
        selectedShadow_ = cache->spriteFrameByName(kMCSelectedShadow);
        
        unselected();
        
        setVisible(false);
        
        return true;
    }
    
    return false;
}

void
MCShadow::bind(MCRole *aRole)
{
    MCRoleEntity *entity = aRole->getEntity();
    CCPoint rolePosition = entity->getPosition();
    CCSize roleSize = entity->getContentSize();
    setPosition(ccp(rolePosition.x + roleSize.width / 2, rolePosition.y));
    setVisible(true);
    entity->setShadow(this);
    role_ = aRole;
}

MCRole *
MCShadow::unbind()
{
    MCRole *role = role_;
    
    setVisible(false);
    role_ = NULL;
    
    return role;
}

void
MCShadow::selected()
{
    setDisplayFrame(selectedShadow_);
    setAnchorPoint(kMCSelectedShadowAnchorPoint);
}

void
MCShadow::unselected()
{
    setDisplayFrame(shadow_);
    setAnchorPoint(kMCShadowAnchorPoint);
}

void
MCShadow::updatePosition()
{
    MCRoleEntity *entity = role_->getEntity();
    CCPoint rolePosition = entity->getPosition();
    CCSize roleSize = entity->getContentSize();
    setPosition(ccp(rolePosition.x + roleSize.width / 2, rolePosition.y));
}
