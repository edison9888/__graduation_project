//
//  MCOBB.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCOBB.h"
#include <math.h>

bool
MCOBB::init(const CCRect &aBoundingBox, float rotation)
{
    setup(aBoundingBox, rotation);
    
    return true;
}

MCOBB *
MCOBB::create(const CCRect &aBoundingBox, float rotation)
{
    MCOBB *obb = new MCOBB;
    
    obb->init(aBoundingBox, rotation);
    
    return obb;
}

void
MCOBB::setup(const CCRect &aBoundingBox, float rotation)
{
    center_ = ccp(aBoundingBox.getMidX(),
                  aBoundingBox.getMinY());
    extents_.width = aBoundingBox.size.width / 2;
    extents_.height = aBoundingBox.size.height / 2;
    axes_[0] = ccp(cosf(rotation), sinf(rotation));
    axes_[1] = ccp(-1 * sinf(rotation), cosf(rotation));
    width_ = aBoundingBox.size.width;
    height_ = aBoundingBox.size.height;
    rotation_ = rotation;
}

void
MCOBB::setup(const CCPoint &aCenterPoint, float width, float height, float rotation)
{
    center_.x = aCenterPoint.x;
    center_.y = aCenterPoint.y;
    extents_.width = width / 2;
    extents_.height = height / 2;
    axes_[0] = ccp(cosf(rotation), sinf(rotation));
    axes_[1] = ccp(-1 * sinf(rotation), cosf(rotation));
    width_ = width;
    height_ = height;
    rotation_ = rotation;
}

bool
MCOBB::collidesWith(MCOBB *anOBB)
{
    CCPoint nv = ccpSub(center_, anOBB->center_);
    CCPoint axisA1 = axes_[0];
    CCPoint axisA2 = axes_[1];
    CCPoint axisB1 = anOBB->axes_[0];
    CCPoint axisB2 = anOBB->axes_[1];
    
    if (getProjectionRadius(axisA1) + anOBB->getProjectionRadius(axisA1) <= fabsf(ccpDot(nv, axisA1))) {
        return false;
    }
    if (getProjectionRadius(axisA2) + anOBB->getProjectionRadius(axisA2) <= fabsf(ccpDot(nv, axisA2))) {
        return false;
    }
    if (getProjectionRadius(axisB1) + anOBB->getProjectionRadius(axisB1) <= fabsf(ccpDot(nv, axisB1))) {
        return false;
    }
    if (getProjectionRadius(axisB2) + anOBB->getProjectionRadius(axisB2) <= fabsf(ccpDot(nv, axisB2))) {
        return false;
    }
    
    return true;
}

float
MCOBB::getProjectionRadius(const CCPoint anAxis) {
    return extents_.width * fabsf(ccpDot(anAxis, axes_[0])) + extents_.height * fabsf(ccpDot(anAxis, axes_[1]));
}