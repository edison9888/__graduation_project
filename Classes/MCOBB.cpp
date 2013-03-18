//
//  MCOBB.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCOBB.h"
#include <math.h>

MCOBB::MCOBB()
{
    center = CCPointZero;
    extents = CCSizeZero;
    axes[0] = CCPointZero;
    axes[1] = CCPointZero;
    width = 0;
    height = 0;
    rotation = 0;
}

MCOBB::MCOBB(const MCOBB &anOBB)
{
    center = anOBB.center;
    extents = anOBB.extents;
    axes[0] = anOBB.axes[0];
    axes[1] = anOBB.axes[1];
    width = anOBB.width;
    height = anOBB.height;
    rotation = anOBB.rotation;
}

void
MCOBB::convertAABBToOBB(const CCRect &anAABB, MCOBB &obb)
{
    obb.setup(anAABB, 0);
}

void
MCOBB::setup(const CCRect &aBoundingBox, float rotation)
{
    center = ccp(aBoundingBox.getMidX(),
                  aBoundingBox.getMidY());
    extents.width = aBoundingBox.size.width / 2;
    extents.height = aBoundingBox.size.height / 2;
    axes[0] = ccp(cosf(rotation), sinf(rotation));
    axes[1] = ccp(-1 * sinf(rotation), cosf(rotation));
    width = aBoundingBox.size.width;
    height = aBoundingBox.size.height;
    this->rotation = rotation;
}

void
MCOBB::setup(const CCPoint &aCenterPoint, float width, float height, float rotation)
{
    center.x = aCenterPoint.x;
    center.y = aCenterPoint.y;
    extents.width = width / 2;
    extents.height = height / 2;
    axes[0] = ccp(cosf(rotation), sinf(rotation));
    axes[1] = ccp(-1 * sinf(rotation), cosf(rotation));
    this->width = width;
    this->height = height;
    this->rotation = rotation;
}

void
MCOBB::setup(const CCPoint &anOffset)
{
    center.x += anOffset.x;
    center.y += anOffset.y;
}

void
MCOBB::setup(const MCOBB &anOBB)
{
    center = anOBB.center;
    extents = anOBB.extents;
    axes[0] = anOBB.axes[0];
    axes[1] = anOBB.axes[1];
    width = anOBB.width;
    height = anOBB.height;
    rotation = anOBB.rotation;
}

bool
MCOBB::collidesWith(const MCOBB &anOBB)
{
    CCPoint nv = ccpSub(center, anOBB.center);
    CCPoint axisA1 = axes[0];
    CCPoint axisA2 = axes[1];
    CCPoint axisB1 = anOBB.axes[0];
    CCPoint axisB2 = anOBB.axes[1];
    
    if (getProjectionRadius(axisA1) + anOBB.getProjectionRadius(axisA1) <= fabsf(ccpDot(nv, axisA1))) {
        return false;
    }
    if (getProjectionRadius(axisA2) + anOBB.getProjectionRadius(axisA2) <= fabsf(ccpDot(nv, axisA2))) {
        return false;
    }
    if (getProjectionRadius(axisB1) + anOBB.getProjectionRadius(axisB1) <= fabsf(ccpDot(nv, axisB1))) {
        return false;
    }
    if (getProjectionRadius(axisB2) + anOBB.getProjectionRadius(axisB2) <= fabsf(ccpDot(nv, axisB2))) {
        return false;
    }
    
    return true;
}

float
MCOBB::getProjectionRadius(const CCPoint &anAxis) const
{
    return extents.width * fabsf(ccpDot(anAxis, axes[0])) + extents.height * fabsf(ccpDot(anAxis, axes[1]));
}

CCPoint
MCOBB::getOrigin() const
{
    return ccp(center.x - extents.width, center.y - extents.height);
}
