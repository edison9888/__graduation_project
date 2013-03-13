//
//  MCBarrier.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCBarrier.h"
#include "MCRole.h"

const char *kMCTypeBarrier = "barrier";

bool
MCBarrier::init(const CCRect &aRect, MCBarrierType aBarrierType)
{
    rect_ = CCRect(aRect);
    type_ = aBarrierType;
    
    return true;
}

MCBarrier *
MCBarrier::create(const CCRect &aRect, MCBarrierType aBarrierType)
{
    MCBarrier *pRet = new MCBarrier();
    if (pRet && pRet->init(aRect, aBarrierType)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
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
MCBarrier::collidesWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap)
{
    MCRole *role = aRoleEntity->getPrototype();
    MCRoleRace race = role->getRoleRace();
    CCRect bounds = aRoleEntity->getAABB();
    bounds.origin = ccpAdd(bounds.origin, anOffsetAtMap);
    
    if (rect_.intersectsRect(bounds)) {
        return MCHasFlag(type_, race); /* 此障碍物不允许改人物种族经过 */
    }
    
    return false;
}
