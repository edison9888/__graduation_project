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
    obb_.setup(aRect, 0);
    
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
    MCOBB obb;
    
    MCOBB::convertAABBToOBB(aTargetRect, obb);
    
    return obb_.collidesWith(obb);
}


bool
MCSemiTransparent::collidesWith(const MCOBB &anOBB)
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
MCSemiTransparent::collidesWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap)
{
//    CCRect bounds = aRoleEntity->getAABB();
//    bounds.origin = ccpAdd(bounds.origin, anOffsetAtMap);
    
//    return rect_.intersectsRect(bounds);
//    MCOBB *obb = aRoleEntity->getOBB();
    
//    return obb_.collidesWith(obb);
    return false;
}
