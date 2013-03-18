//
//  MCViewportLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCViewportLayer.h"
#include "MCHero.h"
#include "MCEntrance.h"
#include "MCBarrier.h"

bool
MCViewportLayer::init()
{
    if (CCLayer::init()) {
        objects = CCArray::create();
        objects->retain();
        entrances = CCArray::create();
        entrances->retain();
        semis = CCArray::create();
        semis->retain();
        barriers = CCArray::create();
        barriers->retain();
        objects->addObject(MCHero::sharedHero());
        return true;
    }
    
    return false;
}

void
MCViewportLayer::loadObjects(CCArray *objects)
{
    this->objects->addObjectsFromArray(objects);
}

void
MCViewportLayer::loadBarriers(CCArray *barriers)
{
    this->barriers->addObjectsFromArray(barriers);
}

void
MCViewportLayer::loadSemis(CCArray *semis)
{
    this->semis->addObjectsFromArray(semis);
}

void
MCViewportLayer::draw(void)
{
    CCLayer::draw();
    if (!1) {
    CCObject *obj;
    MCRoleEntity *roleEntity;
    MCRole *role;
    CCPoint mapOffset = map->getPosition();
    CCARRAY_FOREACH(objects, obj) {
        role = (MCRole *) obj;
        roleEntity = role->getEntity();
        MCOBB obb = roleEntity->getOBB();
        
//        /* draw viewport */
//        MCOBB vpOBB = role->getViewport()->getOBB();
//        CCRect aabb = CCRectMake(obb.center.x - obb.extents.width,
//                                 obb.center.y - obb.extents.height,
//                                 obb.width,
//                                 obb.height);
//        aabb.origin = ccpAdd(aabb.origin, mapOffset);
//        CCPoint dp = ccp(aabb.origin.x + obb.width,
//                         aabb.origin.y + obb.height);
//        CCPointLog(dp);
//        ccDrawSolidRect(aabb.origin,
//                        dp,
//                        ccc4f(0.9, 0.2, 0.1, 0.4));
        /* AABB */
        CCRect aabb;
        CCPoint dp;
        aabb = CCRectMake(obb.center.x - obb.extents.width,
                          obb.center.y - obb.extents.height,
                          obb.width,
                          obb.height);
        aabb.origin = ccpAdd(aabb.origin, mapOffset);
        dp = ccp(aabb.origin.x + aabb.size.width,
                 aabb.origin.y + aabb.size.height);
        ccDrawSolidRect(aabb.origin,
                        dp,
                        ccc4f(0.6, 0.6, 0.6, 0.4));
    }
    
    /* entrances */
    CCARRAY_FOREACH(entrances, obj) {
        MCEntrance *entrance = (MCEntrance *) obj;
        MCOBB obb = entrance->getOBB();
        
        /* AABB */
        CCRect aabb = CCRectMake(obb.center.x - obb.extents.width,
                                 obb.center.y - obb.extents.height,
                                 obb.width,
                                 obb.height);
        aabb.origin = ccpAdd(aabb.origin, mapOffset);
        CCRectLog(aabb);
        CCPoint dp = ccp(aabb.origin.x + aabb.size.width,
                         aabb.origin.y + aabb.size.height);
        ccDrawSolidRect(aabb.origin,
                        dp,
                        ccc4f(0.3, 0.9, 0.3, 0.4));
    }
        /* semis */
        CCARRAY_FOREACH(semis, obj) {
            MCSemiTransparent *semi = (MCSemiTransparent *) obj;
            MCOBB obb = semi->getOBB();
            
            /* AABB */
            CCRect aabb = CCRectMake(obb.center.x - obb.extents.width,
                                     obb.center.y - obb.extents.height,
                                     obb.width,
                                     obb.height);
            aabb.origin = ccpAdd(aabb.origin, mapOffset);
            CCPoint dp = ccp(aabb.origin.x + aabb.size.width,
                             aabb.origin.y + aabb.size.height);
            ccDrawSolidRect(aabb.origin,
                            dp,
                            ccc4f(0.2, 0.4, 0.9, 0.4));
        }
        
        /* barriers */
        CCARRAY_FOREACH(barriers, obj) {
            MCBarrier *barrier = (MCBarrier *) obj;
            MCOBB obb = barrier->getOBB();
            
            /* AABB */
            CCRect aabb = CCRectMake(obb.center.x - obb.extents.width,
                                     obb.center.y - obb.extents.height,
                                     obb.width,
                                     obb.height);
            aabb.origin = ccpAdd(aabb.origin, mapOffset);
            CCPoint dp = ccp(aabb.origin.x + aabb.size.width,
                             aabb.origin.y + aabb.size.height);
            ccDrawSolidRect(aabb.origin,
                            dp,
                            ccc4f(0.9, 0.1, 0.1, 0.4));
        }
    }
}