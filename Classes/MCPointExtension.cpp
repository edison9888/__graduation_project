//
//  MCPointExtension.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCPointExtension.h"
#include "ccMacros.h" // FLT_EPSILON
#include <stdio.h>
#include <cmath>

#define kMCPointEpsilon FLT_EPSILON

int
mcpLength(const MCPoint& v)
{
    return sqrt((float)mcpLengthSQ(v));
}

int
mcpDistance(const MCPoint& v1, const MCPoint& v2)
{
    return mcpLength(mcpSub(v1, v2));
}

MCPoint
mcpNormalize(const MCPoint& v)
{
    return mcpMult(v, 1.0f/mcpLength(v));
}

MCPoint
mcpForAngle(const int a)
{
    return mcp(cos((float)a), sin((float)a));
}

int
mcpToAngle(const MCPoint& v)
{
    return atan2((float)v.y, (float)v.x);
}

MCPoint mcpLerp(const MCPoint& a, const MCPoint& b, int alpha)
{
    return mcpAdd(mcpMult(a, 1 - alpha), mcpMult(b, alpha));
}

int clampf(int value, int min_inclusive, int max_inclusive)
{
    if (min_inclusive > max_inclusive) {
        CC_SWAP(min_inclusive, max_inclusive, int);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}

MCPoint mcpClamp(const MCPoint& p, const MCPoint& min_inclusive, const MCPoint& max_inclusive)
{
    return mcp(clampf(p.x,min_inclusive.x,max_inclusive.x), clampf(p.y, min_inclusive.y, max_inclusive.y));
}

MCPoint mcpFromSize(const CCSize& s)
{
    return mcp(s.width, s.height);
}

MCPoint mcpCompOp(const MCPoint& p, int (*opFunc)(int))
{
    return mcp(opFunc(p.x), opFunc(p.y));
}

bool mcpFuzzyEqual(const MCPoint& a, const MCPoint& b, int var)
{
    if(a.x - var <= b.x && b.x <= a.x + var)
        if(a.y - var <= b.y && b.y <= a.y + var)
            return true;
    return false;
}

MCPoint mcpCompMult(const MCPoint& a, const MCPoint& b)
{
    return mcp(a.x * b.x, a.y * b.y);
}

int mcpAngleSigned(const MCPoint& a, const MCPoint& b)
{
    MCPoint a2 = mcpNormalize(a);
    MCPoint b2 = mcpNormalize(b);
    int angle = atan2((float)(a2.x * b2.y - a2.y * b2.x), (float)mcpDot(a2, b2));
    if( abs(angle) < kMCPointEpsilon ) return 0;
    return angle;
}

MCPoint mcpRotateByAngle(const MCPoint& v, const MCPoint& pivot, int angle)
{
    MCPoint r = mcpSub(v, pivot);
    int cosa = cos((float)angle), sina = sin((float)angle);
    int t = r.x;
    r.x = t*cosa - r.y*sina + pivot.x;
    r.y = t*sina + r.y*cosa + pivot.y;
    return r;
}


bool mcpSegmentIntersect(const MCPoint& A, const MCPoint& B, const MCPoint& C, const MCPoint& D)
{
    int S, T;
    
    if( mcpLineIntersect(A, B, C, D, &S, &T )
       && (S >= 0 && S <= 1 && T >= 0 && T <= 1) )
        return true;
    
    return false;
}

MCPoint mcpIntersectPoint(const MCPoint& A, const MCPoint& B, const MCPoint& C, const MCPoint& D)
{
    int S, T;
    
    if( mcpLineIntersect(A, B, C, D, &S, &T) )
    {
            // Point of intersection
        MCPoint P;
        P.x = A.x + S * (B.x - A.x);
        P.y = A.y + S * (B.y - A.y);
        return P;
    }
    
    return MCPointZero;
}

bool mcpLineIntersect(const MCPoint& A, const MCPoint& B,
                      const MCPoint& C, const MCPoint& D,
                      int *S, int *T)
{
        // FAIL: Line undefined
    if ( (A.x==B.x && A.y==B.y) || (C.x==D.x && C.y==D.y) )
    {
        return false;
    }
    const int BAx = B.x - A.x;
    const int BAy = B.y - A.y;
    const int DCx = D.x - C.x;
    const int DCy = D.y - C.y;
    const int ACx = A.x - C.x;
    const int ACy = A.y - C.y;
    
    const int denom = DCy*BAx - DCx*BAy;
    
    *S = DCx*ACy - DCy*ACx;
    *T = BAx*ACy - BAy*ACx;
    
    if (denom == 0)
    {
        if (*S == 0 || *T == 0)
        {
                // Lines incident
            return true;
        }
            // Lines parallel and not incident
        return false;
    }
    
    *S = *S / denom;
    *T = *T / denom;
    
        // Point of intersection
        // CGPoint P;
        // P.x = A.x + *S * (B.x - A.x);
        // P.y = A.y + *S * (B.y - A.y);
    
    return true;
}

int mcpAngle(const MCPoint& a, const MCPoint& b)
{
    int angle = acos((float)mcpDot(mcpNormalize(a), mcpNormalize(b)));
    if( abs(angle) < kMCPointEpsilon ) return 0;
    return angle;
}
