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
MCViewportLayer::loadEntrances(CCArray *entrances)
{
    this->entrances->addObjectsFromArray(entrances);
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
        CCRect aabb;
        CCPoint dp;
        
        /* AABB */
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
        /* draw viewport */
        if (role->getAI()) {
            MCOBB vpOBB = role->getAI()->getVision()->getOBB();
            CCRect aabb = vpOBB.getAABB();
            aabb.origin = ccpAdd(aabb.origin, mapOffset);
            CCPoint dp = ccp(aabb.origin.x + vpOBB.width,
                             aabb.origin.y + vpOBB.height);
            ccDrawSolidRect(aabb.origin,
                            dp,
                            ccc4f(0.9, 0.2, 0.1, 0.4));
        }
    }
#if (MC_COLLISION_USE_OBB == 1)
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
#else
        /* entrances */
        CCARRAY_FOREACH(entrances, obj) {
            MCEntrance *entrance = (MCEntrance *) obj;
            
            /* AABB */
            CCRect aabb = entrance->getFrame();
            aabb.origin = ccpAdd(aabb.origin, mapOffset);
            CCPoint dp = ccp(aabb.origin.x + aabb.size.width,
                             aabb.origin.y + aabb.size.height);
            ccDrawSolidRect(aabb.origin,
                            dp,
                            ccc4f(0.3, 0.9, 0.3, 0.4));
        }
        /* semis */
        CCARRAY_FOREACH(semis, obj) {
            MCSemiTransparent *semi = (MCSemiTransparent *) obj;
            
            /* AABB */
            CCRect aabb = semi->getFrame();
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
            
            /* AABB */
            CCRect aabb = barrier->getFrame();
            aabb.origin = ccpAdd(aabb.origin, mapOffset);
            CCPoint dp = ccp(aabb.origin.x + aabb.size.width,
                             aabb.origin.y + aabb.size.height);
            ccDrawSolidRect(aabb.origin,
                            dp,
                            ccc4f(0.9, 0.1, 0.1, 0.4));
        }
#endif
    }
}