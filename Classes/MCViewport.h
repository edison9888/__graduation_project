//
//  MCViewport.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCViewport__
#define __Military_Confrontation__MCViewport__

#include "MCRoleEntity.h"
#include "MCGeometry.h"

class MCOBB;

class MCViewport {
public:
    ~MCViewport();
    
    bool init(MCRoleEntity *aRoleEntity);
    
    static MCViewport *create(MCRoleEntity *aRoleEntity);
    
        //warning: debug
    MCViewport *getDebugViewport();
    
    /**
     * 检测是否与人物碰撞
     *
     * aRoleEntity(in):   人物实体
     * anOffsetAtMap(in): 人物在地图上的位置
     *
     * 返回值你懂的
     */
    bool collideWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap);
    
        //warning: private is right
//private:
public:
    MCRoleEntity *roleEntity_;
    CCRect       boundingBox_;
    float        unitLength_;
    
    CC_PROPERTY_READONLY_PASS_BY_REF(MCOBB, obb_, OBB);
};

#endif /* defined(__Military_Confrontation__MCViewport__) */
