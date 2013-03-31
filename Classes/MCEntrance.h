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
    
    bool collidesWith(const CCRect &aTargetRect);
    bool collidesWith(const MCOBB &anOBB);
    
    inline void setup(const CCRect &aRect) {
        obb_.setup(aRect, 0);
    }
    
    /**
     * 检测是否与人物碰撞
     *
     * aRoleEntity(in):   人物实体
     * anOffsetAtMap(in): 人物在地图上的位置
     *
     * 返回值你懂的
     */
    bool collidesWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap);
    
    CC_SYNTHESIZE(CCString *, destinaion_, Destination);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCOBB, obb_, OBB);
};

#endif /* defined(__Military_Confrontation__MCEntrance__) */
