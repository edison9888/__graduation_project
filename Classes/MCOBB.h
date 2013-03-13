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
    bool init(const CCRect &aBoundingBox, float rotation);
    
    static MCOBB *create(const CCRect &aBoundingBox, float rotation);
    
    void setup(const CCRect &aBoundingBox, float rotation);
    void setup(const CCPoint &aCenterPoint, float width, float height, float rotation);
    
    bool collidesWith(MCOBB *anOBB);
    
    float getProjectionRadius(const CCPoint anAxis);
    
    
private:
    CCPoint center_;
    CCSize  extents_;
    CCPoint axes_[2];
    float width_;
    float height_;
    float rotation_;
};

#endif /* defined(__Military_Confrontation__MCOBB__) */
