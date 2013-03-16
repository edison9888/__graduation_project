//
//  MCEntrance.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEntrance.h"

const char *kMCTypeEntrance = "entrance";

MCEntrance::~MCEntrance()
{
    CC_SAFE_RELEASE(destinaion_);
}

void
MCEntrance::init(const CCRect &aRect)
{
    obb_.setup(aRect, 0);
}

MCEntrance *
MCEntrance::create(const CCRect &aRect)
{
    MCEntrance *pRet = new MCEntrance;
    if (pRet) {
        pRet->init(aRect);
        pRet->autorelease();
    } else {
        delete pRet;
        pRet = NULL;
    }
    return pRet;
}

bool
MCEntrance::collidesWith(const CCRect &aTargetRect)
{
    MCOBB obb;
    
    MCOBB::convertAABBToOBB(aTargetRect, obb);
    
    return obb_.collidesWith(obb);
}

bool
MCEntrance::collidesWith(const MCOBB &anOBB)
{
    return obb_.collidesWith(anOBB);
}

/**
 * 检测是否与人物碰撞
 *
 * aRoleEntity(in):   人物实体
 * anOffsetAtMap(in): 人物在地图上的位置
 *
 * 返回值你懂的
 */
bool
MCEntrance::collidesWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap)
{
//    CCRect bounds = aRoleEntity->getAABB();
//    bounds.origin = ccpAdd(bounds.origin, anOffsetAtMap);
    
//    return rect_.intersectsRect(bounds);
    return false;
}
