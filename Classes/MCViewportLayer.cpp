//
//  MCViewportLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCViewportLayer.h"
#include "MCHero.h"

bool
MCViewportLayer::init()
{
    if (CCLayer::init()) {
        roles = CCArray::create();
        roles->retain();
        roles->addObject(MCHero::sharedHero()->getEntity());
        return true;
    }
    
    return false;
}

void
MCViewportLayer::draw(void)
{
    CCLayer::draw();
    CCObject *obj;
    MCRoleEntity *roleEntity;
    MCRole *role;
//    CCRect bounds;
    CCARRAY_FOREACH(roles, obj) {
        roleEntity = (MCRoleEntity *) obj;
        role = roleEntity->getPrototype();
//        bounds = roleEntity->getBounds();
//        bounds.origin = ccpAdd(bounds.origin, roleEntity->getPosition());
        MCViewport *vp = role->getViewport()->getDebugViewport();
        CCPoint dp = ccp(vp->boundingBox_.origin.x + vp->boundingBox_.size.width,
                         vp->boundingBox_.origin.y + vp->boundingBox_.size.height);
        ccDrawSolidRect(vp->boundingBox_.origin,
                        dp,
                        ccc4f(0.9, 0.2, 0.1, 0.4));
//        ccDrawFilledCircle(role->getViewport()->center_,
//                           role->getViewport()->radius_,
//                           0, 2 * M_PI,
//                           100);
//        ccDrawFilledCircle(c, vp->radius_, 0, 2*M_PI, 1000);
    }
}