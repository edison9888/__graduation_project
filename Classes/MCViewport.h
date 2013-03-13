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

class MCViewport {
public:
    ~MCViewport();
    
    bool init(MCRoleEntity *aRoleEntity);
    
    static MCViewport *create(MCRoleEntity *aRoleEntity);
    
#warning for debug
    MCViewport *getDebugViewport() {
        MCViewport *vp = MCViewport::create(roleEntity_);
        MCFacade facade = roleEntity_->getFacade();
        CCRect bounds = roleEntity_->getAABB();
        
        bounds.origin = ccpAdd(bounds.origin, roleEntity_->getPosition());
        if (facade == MCFacingUp) {
            vp->boundingBox_.origin = ccpAdd(bounds.origin, ccp(-2 * bounds.size.width, bounds.size.height));
            vp->boundingBox_.size = CCSizeMake(5 * bounds.size.width, 4 * bounds.size.height);
        } else if (facade == MCFacingDown) {
            vp->boundingBox_.origin = ccpAdd(bounds.origin, ccp(-2 * bounds.size.width, -4 * bounds.size.height));
            vp->boundingBox_.size = CCSizeMake(5 * bounds.size.width, 4 * bounds.size.height);
        } else if (facade == MCFacingLeft) {
            vp->boundingBox_.origin = ccpAdd(bounds.origin, ccp(-5 * bounds.size.height, -1.5 * bounds.size.width));
            vp->boundingBox_.size = CCSizeMake(5 * bounds.size.height, 4 * bounds.size.width);
        } else if (facade == MCFacingRight) {
            vp->boundingBox_.origin = ccpAdd(bounds.origin, ccp(bounds.size.height * 2, -1.5 * bounds.size.width));
            vp->boundingBox_.size = CCSizeMake(5 * bounds.size.height, 4 * bounds.size.width);
        }
        
        return vp;
    }
    
    /**
     * 检测是否与人物碰撞
     *
     * aRoleEntity(in):   人物实体
     * anOffsetAtMap(in): 人物在地图上的位置
     *
     * 返回值你懂的
     */
    bool collideWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap);
    
#warning private is right
//private:
public:
    MCRoleEntity *roleEntity_;
    CCRect       boundingBox_;
    float        unitLength_;
    /* deprecated */
//    MCLine       *line1_;
//    MCLine       *line2_;
};

#endif /* defined(__Military_Confrontation__MCViewport__) */
