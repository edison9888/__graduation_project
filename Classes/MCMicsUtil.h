//
//  MCMicsUtil.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMicsUtil__
#define __Military_Confrontation__MCMicsUtil__

#include "MCObject.h"

/**
 * a: point.y
 * b: point.x
 * tan(angle) = a / b
 * angle = atan (a / b)
 */
inline void
MCGetAngleForPoint(const CCPoint &aPoint,
                   float         &angle)
{
    float a = aPoint.y;
    float b = aPoint.x;
    float theta;
    
    a = fabsf (a);
    b = fabsf (b);
    theta = atan (a / b);
    
    if ((int) b == 0) //b不能等于0，否则atan不能计算
    {
        angle = 90.0f;
    }
    else
    {
        angle = theta * 180 / M_PI;
    }
}

/**
 * 输入精灵的碰撞矩形、位置和移动方向偏移量，输出参与碰撞的点的坐标和个数
 *
 * p3 -- p2
 * |     |
 * p0 -- p1
 *
 * boundsRect(in): 碰撞矩形
 * position(in):   位置
 * offset(in):     移动方向偏移量
 * point1(out):    碰撞点1
 * point2(out):    碰撞点2
 * point3(out):    碰撞点3
 *
 * 返回碰撞点个数
 */
inline mc_size_t
MCGetCheckPoints(const CCRect  &boundsRect,
                 const CCPoint &position,
                 const CCPoint &offset,
                 CCPoint       &point1,
                 CCPoint       &point2,
                 CCPoint       &point3)
{
    float angle;
    mc_size_t count = 2; /* 碰撞点个数 */
    CCPoint p0 = ccpAdd(position, boundsRect.origin);
    
    /* 每个方向分配60度角的空间 */
    MCGetAngleForPoint(offset, angle);
    point1.x = p0.x;
    point1.y = p0.y;
    point2.x = p0.x;
    point2.y = p0.y;
    if (angle < 22.5f) {
        if (offset.x > 0) { /* 向右，point1=p1, point2=p2 */
            /* tag: p1 */
            point1.x += boundsRect.size.width;
            /* tag: p2 */
            point2.x += boundsRect.size.width;
            point2.y += boundsRect.size.height;
        } else { /* 向左，point1=p0, point2=p3 */
            /* tag: p3 */
            point2.y += boundsRect.size.height;
        }
    } else if (angle < 67.5f) {
        point3.x = p0.x;
        point3.y = p0.y;
        if (offset.x > 0) {
            if (offset.y > 0) { /* 向右上，point1=p1, point2=p2, point3=p3 */
                point1.x += boundsRect.size.width;
                
                point2.x += boundsRect.size.width;
                point2.y += boundsRect.size.height;
                
                point3.y += boundsRect.size.height;
            } else { /* 向右下，point1=p0, point2=p1, point3=p2 */
                point2.x += boundsRect.size.width;
                
                point3.x += boundsRect.size.width;
                point3.y += boundsRect.size.height;
            }
        } else {
            if (offset.y > 0) { /* 向左上，point1=p0, point2=p2, point3=p3 */
                point2.x += boundsRect.size.width;
                point2.y += boundsRect.size.height;
                
                point3.y += boundsRect.size.height;
            } else { /* 向左下，point1=p0, point2=p1, point3=p3 */
                point2.x += boundsRect.size.width;
                
                point3.y += boundsRect.size.height;
            }
        }
        ++count;
    } else {
        if (offset.y > 0) { /* 向上，point1=p3, point2=p2 */
            point1.y += boundsRect.size.height;
            
            point2.x += boundsRect.size.width;
            point2.y += boundsRect.size.height;
        } else { /* 向下，point1=p0, point2=p1 */
            point2.x += boundsRect.size.width;
        }
    }

    return count;
}

#endif /* defined(__Military_Confrontation__MCMicsUtil__) */
