//
//  MCOBB.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCOBB__
#define __Military_Confrontation__MCOBB__

#include <cocos2d.h>
USING_NS_CC;

class MCOBB {
public:
    MCOBB();
    MCOBB(const MCOBB &anOBB);
    
    static void convertAABBToOBB(const CCRect &anAABB, MCOBB &obb);
    
    void setup(const CCRect &aBoundingBox, float rotation);
    void setup(const CCPoint &aCenterPoint, float width, float height, float rotation);
    void setup(const CCPoint &anOffset);
    void setup(const MCOBB &anOBB);
    
    bool collidesWith(const MCOBB &anOBB);
    
    float getProjectionRadius(const CCPoint &anAxis) const;
    
    CCPoint getOrigin() const;
    CCRect getAABB() const;
    
public:
    CCPoint center;
    CCSize  extents;
    CCPoint axes[2];
    float width;
    float height;
    float rotation;
};

#endif /* defined(__Military_Confrontation__MCOBB__) */
