//
//  MCGeometry.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCGeometry__
#define __Military_Confrontation__MCGeometry__

#include <cocos2d.h>
USING_NS_CC;

/**
 * @addtogroup data_structures
 * @{
 */

class MCPoint : public CCObject
{
public:
    int x;
    int y;
    
public:
    MCPoint();
    MCPoint(int x, int y);
    MCPoint(const MCPoint& other);
    MCPoint& operator= (const MCPoint& other);
    void setPoint(int x, int y);
    virtual CCObject* copyWithZone(CCZone* pZone);
    bool equals(const MCPoint& target) const;
    
public:
    /** @deprecated use MCPoint::equals(const MCPoint&) instead, like p1.equals(p2)
     */
    CC_DEPRECATED_ATTRIBUTE static bool MCPointEqualToPoint(const MCPoint& point1, const MCPoint& point2);
};

class MCSize : public CCObject
{
public:
    int width;
    int height;
    
public:
    MCSize();
    MCSize(int width, int height);
    MCSize(const MCSize& other);
    MCSize& operator= (const MCSize& other);
    void setSize(int width, int height);
    virtual CCObject* copyWithZone(CCZone* pZone);
    bool equals(const MCSize& target) const;
    
public:
    /** @deprecated use MCSize::equals(const MCSize&) instead, like size1.equals(size2) */
    CC_DEPRECATED_ATTRIBUTE static bool MCSizeEqualToSize(const MCSize& size1, const MCSize& size2);
};

class MCRect : public CCObject
{
public:
    MCPoint origin;
    MCSize  size;
    
public:
    MCRect();
    MCRect(int x, int y, int width, int height);
    MCRect(const MCRect& other);
    MCRect& operator= (const MCRect& other);
    void setRect(int x, int y, int width, int height);
    virtual CCObject* copyWithZone(CCZone* pZone);
    int getMinX() const; /// return the leftmost x-value of current rect
    int getMidX() const; /// return the midpoint x-value of current rect
    int getMaxX() const; /// return the rightmost x-value of current rect
    int getMinY() const; /// return the bottommost y-value of current rect
    int getMidY() const; /// return the midpoint y-value of current rect
    int getMaxY() const; /// return the topmost y-value of current rect
    bool equals(const MCRect& rect) const;
    bool containsPoint(const MCPoint& point) const;
    bool intersectsRect(const MCRect& rect) const;
    
public:
    /** @deprecated use MCRect::equals(const MCRect&) instead, like r1.equals(r2) */
    CC_DEPRECATED_ATTRIBUTE static bool MCRectEqualToRect(const MCRect& rect1, const MCRect& rect2);
    /** @deprecated use MCRect::containsPoint(const MCPoint&) instead, like rect.containsPoint(point) */
    CC_DEPRECATED_ATTRIBUTE static bool MCRectContainsPoint(const MCRect& rect, const MCPoint& point);
    /** @deprecated use MCRect::intersectsRect(const MCRect&) instead, like r1.intersectsRect(r2) */
    CC_DEPRECATED_ATTRIBUTE static bool MCRectIntersectsRect(const MCRect& rectA, const MCRect& rectB);
};


#define MCPointMake(x, y) MCPoint((int)(x), (int)(y))
#define MCSizeMake(width, height) MCSize((int)(width), (int)(height))
#define MCRectMake(x, y, width, height) MCRect((int)(x), (int)(y), (int)(width), (int)(height))

#define MCPointFromCCPoint(p) MCPoint((int)(p).x, (int)(p).y)
#define CCPointFromMCPoint(p) CCPoint((float)(p).x, (float)(p).y)
#define MCSizeFromCCSize(s) MCSize((int)(s).width, (int)(s).height)
#define MCRectFromCCRect(r) MCRect((int)(r).origin.x, (int)(r).origin.y, (int)(r).size.width, (int)(r).size.height)

#define MCPointLog(p) CCLog("%d::%s: (%d, %d)", __LINE__, #p, (p).x, (p).y)
#define MCSizeLog(s) CCLog("%d::%s: (%d, %d)", __LINE__, #s, (s).width, (s).height)

#define CCPointLog(p) CCLog("%d::%s: (%.0f, %.0f)", __LINE__, #p, (p).x, (p).y)
#define CCSizeLog(s) CCLog("%d::%s: (%.0f, %.0f)", __LINE__, #s, (s).width, (s).height)

const MCPoint MCPointZero = MCPointMake(0,0);

/* The "zero" size -- equivalent to MCSizeMake(0, 0). */
const MCSize MCSizeZero = MCSizeMake(0,0);

/* The "zero" rectangle -- equivalent to MCRectMake(0, 0, 0, 0). */
const MCRect MCRectZero = MCRectMake(0,0,0,0);

#endif /* defined(__Military_Confrontation__MCGeometry__) */
