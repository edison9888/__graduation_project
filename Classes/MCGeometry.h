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

//class MCPositionProtocol {
//public:
//    virtual CCPoint *getPosition() const = 0;
//};
//
//class MCPosition : public CCPoint, public MCPositionProtocol {
//public:
//    bool init(const CCPoint &aPoint);
//    
//    static MCPosition *create(const CCPoint &aPoint);
//
//    CCPoint *getPosition() const;
//};

class MCLine {
public:
    MCLine() : p1(CCPointZero),
               p2(CCPointZero) {}
    
    MCLine(const CCPoint &p1, const CCPoint &p2) {
        this->p1.x = p1.x;
        this->p1.y = p1.y;
        this->p2.x = p2.x;
        this->p2.y = p2.y;
    }
    
public:
    CCPoint p1;
    CCPoint p2;
};

#define CCPointLog(p) CCLog("%s::%d::%s: (%.1f, %.1f)", __FILE__+85, __LINE__, #p, (p).x, (p).y)
#define CCSizeLog(s) CCLog("%s::%d::%s: (%.1f, %.1f)", __FILE__+85, __LINE__, #s, (s).width, (s).height)
#define CCRectLog(r) CCLog("%s::%d::%s: (%.1f, %.1f) - (%.1f, %.1f)", __FILE__+85, __LINE__, #r, \
                        (r).origin.x, (r).origin.y, (r).size.width, (r).size.height)

#endif /* defined(__Military_Confrontation__MCGeometry__) */
