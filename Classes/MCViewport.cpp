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
//    CC_SAFE_RELEASE(line1_);
//    CC_SAFE_RELEASE(line2_);
}

bool
MCViewport::init(MCRoleEntity *aRoleEntity)
{
    do {
        obb_ = aRoleEntity->getOBB();
        unitLength_ = obb_.width < obb_.height
                        ? obb_.width
                        : obb_.height;
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
//    CCRect boundingBox = boundingBox_;
    MCOBB obb = obb_;
    MCOBB roleOBB = aRoleEntity->getOBB();
    
    return false;
}

const MCOBB &
MCViewport::getOBB()
{
    MCFacade facade = roleEntity_->getFacade();
    CCPoint dPoint = CCPointZero;
    float dw;
    float dh;
    
    obb_.setup(roleEntity_->getOBB());
    if (facade == MCFacingUp) {
        dPoint = ccp(-2 * obb_.width, obb_.height);
        dw = 5 * obb_.width;
        dh = 4 * obb_.height;
    } else if (facade == MCFacingDown) {
        dPoint = ccp(-2 * obb_.width, -4 * obb_.height);
        dw = 5 * obb_.width;
        dh = 4 * obb_.height;
    } else if (facade == MCFacingLeft) {
        dPoint = ccp(-5 * obb_.height, -1.5 * obb_.width);
        dw = 5 * obb_.height;
        dh = 4 * obb_.width;
    } else if (facade == MCFacingRight) {
        dPoint = ccp(2 * obb_.height, -1.5 * obb_.width);
        dw = 5 * obb_.height;
        dh = 4 * obb_.width;
    } else {
        dw = 0;
        dh = 0;
    }
    CCPointLog(obb_.center);
    obb_.center.x += dPoint.x;
    obb_.center.y += dPoint.y;
    CCPointLog(obb_.center);
    obb_.width = dw;
    obb_.height = dh;
    CCSizeLog(obb_);
    
    return obb_;
}

#include "MCScene.h"

MCViewport *
MCViewport::getDebugViewport() {
    MCViewport *vp = MCViewport::create(roleEntity_);
    MCFacade facade = roleEntity_->getFacade();
    const MCOBB &obb = roleEntity_->getOBB();
    CCRect aabb = CCRectMake(obb.center.x - obb.extents.width,
                             obb.center.y - obb.extents.height,
                             obb.width,
                             obb.height);
    MCSceneContext *currentContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    if (currentContext) {
        aabb.origin = ccpAdd(aabb.origin,
                               currentContext->getScene()->getMapOffset());
    }
    
    if (facade == MCFacingUp) {
        vp->boundingBox_.origin = ccpAdd(aabb.origin, ccp(-2 * aabb.size.width, aabb.size.height));
        vp->boundingBox_.size = CCSizeMake(5 * aabb.size.width, 4 * aabb.size.height);
    } else if (facade == MCFacingDown) {
        vp->boundingBox_.origin = ccpAdd(aabb.origin, ccp(-2 * aabb.size.width, -4 * aabb.size.height));
        vp->boundingBox_.size = CCSizeMake(5 * aabb.size.width, 4 * aabb.size.height);
    } else if (facade == MCFacingLeft) {
        vp->boundingBox_.origin = ccpAdd(aabb.origin, ccp(-5 * aabb.size.height, -1.5 * aabb.size.width));
        vp->boundingBox_.size = CCSizeMake(5 * aabb.size.height, 4 * aabb.size.width);
    } else if (facade == MCFacingRight) {
        vp->boundingBox_.origin = ccpAdd(aabb.origin, ccp(aabb.size.height * 2, -1.5 * aabb.size.width));
        vp->boundingBox_.size = CCSizeMake(5 * aabb.size.height, 4 * aabb.size.width);
    }
    
    return vp;
}
