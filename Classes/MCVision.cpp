//
//  MCVision.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-4.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCVision.h"

MCVision::~MCVision()
{
    
}

bool
MCVision::init(MCRoleEntity *aRoleEntity)
{
    do {
        obb_ = aRoleEntity->getOBB();
        unitLength_ = obb_.width < obb_.height
                        ? obb_.width
                        : obb_.height;
        roleEntity_ = aRoleEntity;
        
        return true;
    } while (0);
    
    return false;
}

MCVision *
MCVision::create(MCRoleEntity *aRoleEntity)
{
    MCVision *vision = new MCVision;
    
    vision->init(aRoleEntity);
    
    return vision;
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
MCVision::collideWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap)
{
    MCOBB obb = getOBB();
    return obb.collidesWith(aRoleEntity->getOBB());
}

bool
MCVision::collideWith(MCRoleEntity *aRoleEntity)
{
    MCOBB obb = getOBB();
    return obb.collidesWith(aRoleEntity->getOBB());
}

const MCOBB &
MCVision::getOBB()
{
    MCFacade facade = roleEntity_->getFacade();
    float dw;
    float dh;
    MCOBB roleOBB = roleEntity_->getOBB();
    
    obb_.setup(roleOBB);
    if (facade == MCFacingUp) {
        dw = 5 * obb_.width;
        dh = 4 * obb_.height;
        obb_.center.y += roleOBB.extents.height + dh / 2;
    } else if (facade == MCFacingDown) {
        dw = 5 * obb_.width;
        dh = 4 * obb_.height;
        obb_.center.y -= roleOBB.extents.height + dh / 2;
    } else if (facade == MCFacingLeft) {
        dw = 4 * obb_.width;
        dh = 5 * obb_.height;
        obb_.center.x -= roleOBB.extents.width + dw / 2;
    } else if (facade == MCFacingRight) {
        dw = 4 * obb_.width;
        dh = 5 * obb_.height;
        obb_.center.x += roleOBB.extents.width + dw / 2;
    } else {
        dw = 0;
        dh = 0;
    }
    obb_.width = dw;
    obb_.height = dh;
    obb_.extents.width = dw / 2;
    obb_.extents.height = dh / 2;
    
    return obb_;
}
