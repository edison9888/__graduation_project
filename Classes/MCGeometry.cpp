//
//  MCGeometry.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCGeometry.h"


MCPoint::MCPoint(void)
{
    setPoint(0.0f, 0.0f);
}

MCPoint::MCPoint(int x, int y)
{
    setPoint(x, y);
}

MCPoint::MCPoint(const MCPoint& other)
{
    setPoint(other.x, other.y);
}

MCPoint& MCPoint::operator= (const MCPoint& other)
{
    setPoint(other.x, other.y);
    return *this;
}

void MCPoint::setPoint(int x, int y)
{
    this->x = x;
    this->y = y;
}

CCObject* MCPoint::copyWithZone(CCZone* pZone)
{
    MCPoint* pRet = new MCPoint();
    pRet->setPoint(this->x, this->y);
    return pRet;
}

bool MCPoint::equals(const MCPoint& target) const
{
    return ((x == target.x) && (y == target.y));
}

bool MCPoint::MCPointEqualToPoint(const MCPoint& point1, const MCPoint& point2)
{
    return point1.equals(point2);
}

    // implementation of MCSize

MCSize::MCSize(void)
{
    setSize(0.0f, 0.0f);
}

MCSize::MCSize(int width, int height)
{
    setSize(width, height);
}

MCSize::MCSize(const MCSize& other)
{
    setSize(other.width, other.height);
}

MCSize& MCSize::operator= (const MCSize& other)
{
    setSize(other.width, other.height);
    return *this;
}

void MCSize::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

CCObject* MCSize::copyWithZone(CCZone* pZone)
{
    MCSize* pRet = new MCSize();
    pRet->setSize(this->width, this->width);
    return pRet;
}

bool MCSize::equals(const MCSize& target) const
{
    return ((width == target.width) && (height == target.height));
}


bool MCSize::MCSizeEqualToSize(const MCSize& size1, const MCSize& size2)
{
    return size1.equals(size2);
}

    // implementation of MCRect

MCRect::MCRect(void)
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

MCRect::MCRect(int x, int y, int width, int height)
{
    setRect(x, y, width, height);
}

MCRect::MCRect(const MCRect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

MCRect& MCRect::operator= (const MCRect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    return *this;
}

void MCRect::setRect(int x, int y, int width, int height)
{
        // Only support that, the width and height > 0
    CCAssert(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");
    
    origin.x = x;
    origin.y = y;
    
    size.width = width;
    size.height = height;
}

CCObject* MCRect::copyWithZone(CCZone* pZone)
{
    MCRect* pRet = new MCRect();
    pRet->setRect(this->origin.x, this->origin.y, this->size.width, this->size.height);
    return pRet;
}

bool MCRect::equals(const MCRect& rect) const
{
    return (origin.equals(rect.origin) &&
            size.equals(rect.size));
}

int MCRect::getMaxX() const
{
    return (int)(origin.x + size.width);
}

int MCRect::getMidX() const
{
    return (int)(origin.x + size.width / 2.0);
}

int MCRect::getMinX() const
{
    return origin.x;
}

int MCRect::getMaxY() const
{
    return origin.y + size.height;
}

int MCRect::getMidY() const
{
    return (int)(origin.y + size.height / 2.0);
}

int MCRect::getMinY() const
{
    return origin.y;
}

bool MCRect::containsPoint(const MCPoint& point) const
{
    bool bRet = false;
    
    if (point.x >= getMinX() && point.x <= getMaxX()
        && point.y >= getMinY() && point.y <= getMaxY())
    {
        bRet = true;
    }
    
    return bRet;
}

bool MCRect::intersectsRect(const MCRect& rect) const
{
    return !(     getMaxX() < rect.getMinX() ||
             rect.getMaxX() <      getMinX() ||
             getMaxY() < rect.getMinY() ||
             rect.getMaxY() <      getMinY());
}

bool MCRect::MCRectEqualToRect(const MCRect& rect1, const MCRect& rect2)
{
    return rect1.equals(rect2);
}

bool MCRect::MCRectContainsPoint(const MCRect& rect, const MCPoint& point)
{
    return rect.containsPoint(point);
}

bool MCRect::MCRectIntersectsRect(const MCRect& rectA, const MCRect& rectB)
{
    /*
     return !(MCRectGetMaxX(rectA) < MCRectGetMinX(rectB)||
     MCRectGetMaxX(rectB) < MCRectGetMinX(rectA)||
     MCRectGetMaxY(rectA) < MCRectGetMinY(rectB)||
     MCRectGetMaxY(rectB) < MCRectGetMinY(rectA));
     */
    return rectA.intersectsRect(rectB);
}
