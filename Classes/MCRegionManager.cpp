//
//  MCRegionManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCRegionManager.h"
#include "JsonBox.h"
#include "MCRegion.h"

static const char *kMCRegionFilepath = "R000.jpkg";

/* 森林 */
static const mc_object_id_t kMCForestRegionId = {'M', '1', '0', '0'};
/* 沙漠 */
static const mc_object_id_t kMCDesertRegionId = {'M', '2', '0', '0'};
/* 冰川 */
static const mc_object_id_t kMCGlacierRegionId = {'M', '4', '0', '0'};
/* 地下城 */
static const mc_object_id_t kMCDungeonRegionId = {'M', '3', '0', '0'};

static MCRegionManager *__shared_region_manager = NULL;

MCRegionManager::MCRegionManager()
{
    regions_ = CCDictionary::create();
    regions_->retain();
}

MCRegionManager::~MCRegionManager()
{
    CC_SAFE_RELEASE(regions_);
}

MCRegionManager *
MCRegionManager::sharedRegionManager()
{
    if (__shared_region_manager == NULL) {
        __shared_region_manager = new MCRegionManager;
        __shared_region_manager->loadRegions();
    }
    
    return __shared_region_manager;
}

void
MCRegionManager::loadRegions()
{
    JsonBox::Value v;
    JsonBox::Object regions;
    JsonBox::Object::iterator regionsIterator;
    CCString *ccstring;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCRegionFilepath);
    if (pstrFileContent) {
        v.loadFromString(pstrFileContent->getCString());
    }
#else
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCRegionFilepath).c_str());
#endif
    
    regions = v.getObject();
    
    for (regionsIterator = regions.begin();
         regionsIterator != regions.end();
         ++regionsIterator) {
        const char *c_str_r_id = regionsIterator->first.c_str();
        mc_object_id_t r_id = {
            c_str_r_id[0],
            c_str_r_id[1],
            c_str_r_id[2],
            c_str_r_id[3]
        };
        JsonBox::Object regionsObject = regionsIterator->second.getObject();
        JsonBox::Array subRegionsArray = regionsObject["sub-regions"].getArray();
        JsonBox::Array::iterator subRegionsArrayIterator;
        MCRegion *region = new MCRegion;
        region->autorelease();
        
        region->setID(r_id);
        region->setSceneRegion(regionsObject["region"].getInt());
        for (subRegionsArrayIterator = subRegionsArray.begin();
             subRegionsArrayIterator != subRegionsArray.end();
             ++subRegionsArrayIterator) {
            const char *c_str_sub_r_id = subRegionsArrayIterator->getString().c_str();
            mc_object_id_t sub_r_id = {
                c_str_sub_r_id[0],
                c_str_sub_r_id[1],
                c_str_sub_r_id[2],
                c_str_sub_r_id[3]
            };
            region->regions.push_back(sub_r_id);
        }
        ccstring = CCString::create(regionsObject["name"].getString());
        region->setName(ccstring);
        ccstring->retain();
        
        regions_->setObject(region, MCObjectIdToDickKey(r_id));
    }
}

MCRegion *
MCRegionManager::regionForObjectId(mc_object_id_t anObjectId)
{
    return dynamic_cast<MCRegion *>(regions_->objectForKey(MCObjectIdToDickKey(anObjectId)));
}

MCRegion *
MCRegionManager::forestRegion()
{
    return regionForObjectId(kMCForestRegionId);
}

MCRegion *
MCRegionManager::desertRegion()
{
    return regionForObjectId(kMCDesertRegionId);
}

MCRegion *
MCRegionManager::glacierRegion()
{
    return regionForObjectId(kMCGlacierRegionId);
}

MCRegion *
MCRegionManager::dungeonRegion()
{
    return regionForObjectId(kMCDungeonRegionId);
}
