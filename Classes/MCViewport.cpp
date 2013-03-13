//
//  MCViewport.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCViewport.h"
#include "MCRole.h"

MCViewport::~MCViewport()
{
    /* deprecated */
//    CC_SAFE_DELETE(line1_);
//    CC_SAFE_DELETE(line2_);
}

bool
MCViewport::init(MCRoleEntity *aRoleEntity)
{
    do {
        CCRect bounds = aRoleEntity->getAABB();
        unitLength_ = bounds.size.width < bounds.size.height
                        ? bounds.size.width
                        : bounds.size.height;
        roleEntity_ = aRoleEntity;
        /* deprecated */
//        line1_ = new MCLine;
//        line2_ = new MCLine;
        
        return true;
    } while (0);
    
    return false;
}

MCViewport *
MCViewport::create(MCRoleEntity *aRoleEntity)
{
    MCViewport *viewport = new MCViewport;
    
    viewport->init(aRoleEntity);
    
    return viewport;
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
MCViewport::collideWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap)
{
    MCFacade facade = aRoleEntity->getFacade();
    CCRect bounds = aRoleEntity->getAABB();
    CCRect selfBounds = roleEntity_->getAABB();
    CCPoint boundsCenterPoint = ccp(bounds.getMidX(),
                                    bounds.getMidY());
    float boundsHalfWidth = bounds.size.width / 2;
    float boundsHalfHeight = bounds.size.height / 2;
    
    
    
    return false;
}
