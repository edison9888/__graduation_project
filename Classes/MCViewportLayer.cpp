//
//  MCViewportLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#if MC_DEBUG_NON_VISUAL_OBJECTS == 1
#include "MCViewportLayer.h"
#include "MCHero.h"
#include "MCEntrance.h"
#include "MCBarrier.h"
#include "MCSkill.h"
#include "MCMezzanine.h"

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
MCViewportLayer::loadMezzanine(MCMezzanine *mezzanine)
{
    this->mezzanine = mezzanine;
}

void
MCViewportLayer::draw(void)
{
    CCLayer::draw();
    
    bool MC_SHOW_OBJECTS = true;
    bool MC_SHOW_VISIONS = true;
    bool MC_SHOW_ENTRANCES = false;
    bool MC_SHOW_SEMITRANSPARENTS = false;
    bool MC_SHOW_BARRIERS = false;
    bool MC_SHOW_SKILLS = true;
    
    if (1) {
        CCObject *obj;
        MCRoleEntity *roleEntity;
        MCRole *role;
        CCPoint mapOffset = map->getPosition();
        
        /* objects */
        if (MC_SHOW_OBJECTS) {
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
                if (MC_SHOW_VISIONS) {
                    if (role->getAI()) {
#if (MC_COLLISION_USE_OBB == 1)
                        MCOBB vpOBB = role->getAI()->getVision()->getOBB();
                        CCRect aabb = vpOBB.getAABB();
                        aabb.origin = ccpAdd(aabb.origin, mapOffset);
                        CCPoint dp = ccp(aabb.origin.x + vpOBB.width,
                                         aabb.origin.y + vpOBB.height);
#else
                        CCRect aabb = role->getAI()->getVision()->getFrame();
                        aabb.origin = ccpAdd(aabb.origin, mapOffset);
                        CCPoint dp = ccp(aabb.origin.x + aabb.size.width,
                                         aabb.origin.y + aabb.size.height);
#endif
                        ccDrawSolidRect(aabb.origin,
                                        dp,
                                        ccc4f(0.9, 0.2, 0.1, 0.4));
                    }
                }
            }
        }
#if (MC_COLLISION_USE_OBB == 1)
        /* entrances */
        if (MC_SHOW_ENTRANCES) {
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
        }
        
        /* semis */
        if (MC_SHOW_SEMITRANSPARENTS) {
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
        }
        
        /* barriers */
        if (MC_SHOW_BARRIERS) {
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
#else
        /* entrances */
        if (MC_SHOW_ENTRANCES) {
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
        }
        
        /* semis */
        if (MC_SHOW_SEMITRANSPARENTS) {
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
        }
        
        /* barriers */
        if (MC_SHOW_BARRIERS) {
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
        }
#endif
        
        /* skills */
        if (MC_SHOW_SKILLS) {
            CCPoint points[4];
            CCArray *skills = mezzanine->getSkills();
            CCARRAY_FOREACH(skills, obj) {
                MCSkill *skill = (MCSkill *) obj;
                
                MCOBB obb = skill->obb;
                CCPoint origin = obb.getOrigin();
                
                origin = ccpAdd(origin, mapOffset);
                obb.center = ccpAdd(obb.center, mapOffset);
                
                points[0] = ccpRotateByAngle(origin, obb.center, obb.rotation);
                points[1] = ccpRotateByAngle(ccpAdd(origin, ccp(obb.width, 0)), obb.center, obb.rotation);
                points[2] = ccpRotateByAngle(ccpAdd(origin, ccp(obb.width, obb.height)), obb.center, obb.rotation);
                points[3] = ccpRotateByAngle(ccpAdd(origin, ccp(0, obb.height)), obb.center, obb.rotation);
                
                ccDrawSolidPoly(points, 4, ccc4f(0.1, 0.9, 0.4, 0.4));
            }
        }
    }
}
#endif /* MC_DEBUG_NON_VISUAL_OBJECTS */
