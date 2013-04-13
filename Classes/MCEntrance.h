//
//  MCEntrance.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEntrance__
#define __Military_Confrontation__MCEntrance__

#include "MCRoleEntity.h"

extern const char *kMCTypeEntrance;

class MCOBB;

class MCEntrance : public MCObject {
public:
    ~MCEntrance();
    
    void init(const CCRect &aRect);
    
    static MCEntrance* create(const CCRect &aRect);
    
    CC_DEPRECATED_ATTRIBUTE bool collidesWith(const CCRect &aTargetRect);
    bool contains(const CCRect &aTargetRect);
    
#if (MC_COLLISION_USE_OBB == 1)
    CC_DEPRECATED_ATTRIBUTE bool collidesWith(const MCOBB &anOBB);
    
    bool collidesWith(const MCOBB &anOBB);
    
    inline void setup(const CCRect &aRect) {
        obb_.setup(aRect, 0);
    }
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCOBB, obb_, OBB);
#else
    inline void setup(const CCRect &aRect) {
        frame_ = aRect;
    }
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCRect, frame_, Frame);
#endif
    
protected:
    CC_SYNTHESIZE(CCString *, destinaion_, Destination);
};

#endif /* defined(__Military_Confrontation__MCEntrance__) */
