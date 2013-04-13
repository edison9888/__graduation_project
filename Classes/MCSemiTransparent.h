//
//  MCSemiTransparent.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSemiTransparent__
#define __Military_Confrontation__MCSemiTransparent__

#include "MCRoleEntity.h"

extern const char *kMCTypeSemiTransparent;

class MCOBB;

class MCSemiTransparent : public CCObject {
public:
    bool init(const CCRect &aRect);
    
    static MCSemiTransparent* create(const CCRect &aRect);
    
    bool collidesWith(const CCRect &aTargetRect);

    /**
     * 检测是否与人物碰撞
     *
     * aRoleEntity(in):   人物实体
     * anOffsetAtMap(in): 人物在地图上的位置
     *
     * 返回值你懂的
     */
//    bool collidesWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap);
    
#if (MC_COLLISION_USE_OBB == 1)
    bool collidesWith(const MCOBB &anOBB);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCOBB, obb_, OBB);
#else
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCRect, frame_, Frame);
#endif
};

#endif /* defined(__Military_Confrontation__MCSemiTransparent__) */
