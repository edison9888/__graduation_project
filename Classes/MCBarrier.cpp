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
    if (pRet && pRet->init(aRect, aBarrierType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool
MCBarrier::collideWith(const CCRect &aTargetRect)
{
    return rect_.intersectsRect(aTargetRect);
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
MCBarrier::collideWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap)
{
    MCRole *role = aRoleEntity->getPrototype();
    MCRoleRace race = role->getRoleRace();
    CCRect bounds = aRoleEntity->getBounds();
    bounds.origin = ccpAdd(bounds.origin, anOffsetAtMap);
    
    if (race > type_) { /* 人物种族级别比障碍物更高，直接无视障碍物 */
        return false;
    }
    
    return rect_.intersectsRect(bounds);
}
