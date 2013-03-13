//
//  MCSemiTransparent.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSemiTransparent.h"
#include "MCRole.h"

const char *kMCTypeSemiTransparent = "semi-transparent";

bool
MCSemiTransparent::init(const CCRect &aRect)
{
    rect_ = CCRect(aRect);
    
    return true;
}

MCSemiTransparent *
MCSemiTransparent::create(const CCRect &aRect)
{
    MCSemiTransparent *pRet = new MCSemiTransparent();
    if (pRet && pRet->init(aRect)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool
MCSemiTransparent::collidesWith(const CCRect &aTargetRect)
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
MCSemiTransparent::collidesWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap)
{
    CCRect bounds = aRoleEntity->getAABB();
    bounds.origin = ccpAdd(bounds.origin, anOffsetAtMap);
    
    return rect_.intersectsRect(bounds);
}
