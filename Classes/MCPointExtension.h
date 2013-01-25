//
//  MCPointExtension.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCPointExtension__
#define __Military_Confrontation__MCPointExtension__

/**
 @file
 MCPoint extensions based on Chipmunk's cpVect file.
 These extensions work both with MCPoint and cpVect.
 
 The "mcp" prefix means: "CoCos2d Point"
 
 Examples:
 - mcpAdd( mcp(1,1), mcp(2,2) ); // preferred cocos2d way
 - mcpAdd( MCPointMake(1,1), MCPointMake(2,2) ); // also ok but more verbose
 
 - cpvadd( cpv(1,1), cpv(2,2) ); // way of the chipmunk
 - mcpAdd( cpv(1,1), cpv(2,2) ); // mixing chipmunk and cocos2d (avoid)
 - cpvadd( MCPointMake(1,1), MCPointMake(2,2) ); // mixing chipmunk and CG (avoid)
 */

#include "MCGeometry.h"
#include <math.h>

/**
 * @addtogroup data_structures
 * @{
 */

/** Helper macro that creates a MCPoint
 @return MCPoint
 @since v0.7.2
 */
#define mcp(__X__,__Y__) MCPointMake((int)(__X__), (int)(__Y__))

/** Returns opposite of point.
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpNeg(const MCPoint& v)
{
    return mcp(-v.x, -v.y);
}

/** Calculates sum of two points.
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpAdd(const MCPoint& v1, const MCPoint& v2)
{
    return mcp(v1.x + v2.x, v1.y + v2.y);
}

/** Calculates difference of two points.
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpSub(const MCPoint& v1, const MCPoint& v2)
{
    return mcp(v1.x - v2.x, v1.y - v2.y);
}

/** Returns point multiplied by given factor.
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpMult(const MCPoint& v, const int s)
{
    return mcp(v.x*s, v.y*s);
}

/** Calculates midpoint between two points.
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpMidpoint(const MCPoint& v1, const MCPoint& v2)
{
    return mcpMult(mcpAdd(v1, v2), 0.5f);
}

/** Calculates dot product of two points.
 @return int
 @since v0.7.2
 */
static inline int
mcpDot(const MCPoint& v1, const MCPoint& v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

/** Calculates cross product of two points.
 @return int
 @since v0.7.2
 */
static inline int
mcpCross(const MCPoint& v1, const MCPoint& v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}

/** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpPerp(const MCPoint& v)
{
    return mcp(-v.y, v.x);
}

/** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpRPerp(const MCPoint& v)
{
    return mcp(v.y, -v.x);
}

/** Calculates the projection of v1 over v2.
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpProject(const MCPoint& v1, const MCPoint& v2)
{
    return mcpMult(v2, mcpDot(v1, v2)/mcpDot(v2, v2));
}

/** Rotates two points.
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpRotate(const MCPoint& v1, const MCPoint& v2)
{
    return mcp(v1.x*v2.x - v1.y*v2.y, v1.x*v2.y + v1.y*v2.x);
}

/** Unrotates two points.
 @return MCPoint
 @since v0.7.2
 */
static inline MCPoint
mcpUnrotate(const MCPoint& v1, const MCPoint& v2)
{
    return mcp(v1.x*v2.x + v1.y*v2.y, v1.y*v2.x - v1.x*v2.y);
}

/** Calculates the square length of a MCPoint (not calling sqrt() )
 @return int
 @since v0.7.2
 */
static inline int
mcpLengthSQ(const MCPoint& v)
{
    return mcpDot(v, v);
}


/** Calculates the square distance between two points (not calling sqrt() )
 @return int
 @since v1.1
 */
static inline int
mcpDistanceSQ(const MCPoint p1, const MCPoint p2)
{
    return mcpLengthSQ(mcpSub(p1, p2));
}


/** Calculates distance between point an origin
 @return int
 @since v0.7.2
 */
int mcpLength(const MCPoint& v);

/** Calculates the distance between two points
 @return int
 @since v0.7.2
 */
int mcpDistance(const MCPoint& v1, const MCPoint& v2);

/** Returns point multiplied to a length of 1.
 @return MCPoint
 @since v0.7.2
 */
MCPoint mcpNormalize(const MCPoint& v);

/** Converts radians to a normalized vector.
 @return MCPoint
 @since v0.7.2
 */
MCPoint mcpForAngle(const int a);

/** Converts a vector to radians.
 @return int
 @since v0.7.2
 */
int mcpToAngle(const MCPoint& v);


/** Clamp a value between from and to.
 @since v0.99.1
 */
int clampf(int value, int min_inclusive, int max_inclusive);

/** Clamp a point between from and to.
 @since v0.99.1
 */
MCPoint mcpClamp(const MCPoint& p, const MCPoint& from, const MCPoint& to);

/** Quickly convert CCSize to a MCPoint
 @since v0.99.1
 */
MCPoint mcpFromSize(const CCSize& s);

/** Run a math operation function on each point component
 * absf, fllorf, ceilf, roundf
 * any function that has the signature: int func(int);
 * For example: let's try to take the floor of x,y
 * mcpCompOp(p,floorf);
 @since v0.99.1
 */
MCPoint mcpCompOp(const MCPoint& p, int (*opFunc)(int));

/** Linear Interpolation between two points a and b
 @returns
 alpha == 0 ? a
 alpha == 1 ? b
 otherwise a value between a..b
 @since v0.99.1
 */
MCPoint mcpLerp(const MCPoint& a, const MCPoint& b, int alpha);


/** @returns if points have fuzzy equality which means equal with some degree of variance.
 @since v0.99.1
 */
bool mcpFuzzyEqual(const MCPoint& a, const MCPoint& b, int variance);


/** Multiplies a and b components, a.x*b.x, a.y*b.y
 @returns a component-wise multiplication
 @since v0.99.1
 */
MCPoint mcpCompMult(const MCPoint& a, const MCPoint& b);

/** @returns the signed angle in radians between two vector directions
 @since v0.99.1
 */
int mcpAngleSigned(const MCPoint& a, const MCPoint& b);

/** @returns the angle in radians between two vector directions
 @since v0.99.1
 */
int mcpAngle(const MCPoint& a, const MCPoint& b);

/** Rotates a point counter clockwise by the angle around a pivot
 @param v is the point to rotate
 @param pivot is the pivot, naturally
 @param angle is the angle of rotation cw in radians
 @returns the rotated point
 @since v0.99.1
 */
MCPoint mcpRotateByAngle(const MCPoint& v, const MCPoint& pivot, int angle);

/** A general line-line intersection test
 @param p1
 is the startpoint for the first line P1 = (p1 - p2)
 @param p2
 is the endpoint for the first line P1 = (p1 - p2)
 @param p3
 is the startpoint for the second line P2 = (p3 - p4)
 @param p4
 is the endpoint for the second line P2 = (p3 - p4)
 @param s
 is the range for a hitpoint in P1 (pa = p1 + s*(p2 - p1))
 @param t
 is the range for a hitpoint in P3 (pa = p2 + t*(p4 - p3))
 @return bool
 indicating successful intersection of a line
 note that to truly test intersection for segments we have to make
 sure that s & t lie within [0..1] and for rays, make sure s & t > 0
 the hit point is        p3 + t * (p4 - p3);
 the hit point also is    p1 + s * (p2 - p1);
 @since v0.99.1
 */
bool mcpLineIntersect(const MCPoint& p1, const MCPoint& p2,
                      const MCPoint& p3, const MCPoint& p4,
                      int *s, int *t);

/*
 mcpSegmentIntersect returns YES if Segment A-B intersects with segment C-D
 @since v1.0.0
 */
bool mcpSegmentIntersect(const MCPoint& A, const MCPoint& B, const MCPoint& C, const MCPoint& D);

/*
 mcpIntersectPoint returns the intersection point of line A-B, C-D
 @since v1.0.0
 */
MCPoint mcpIntersectPoint(const MCPoint& A, const MCPoint& B, const MCPoint& C, const MCPoint& D);

#endif /* defined(__Military_Confrontation__MCPointExtension__) */
