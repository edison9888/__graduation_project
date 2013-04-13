//
//  MCRegionManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRegionManager__
#define __Military_Confrontation__MCRegionManager__

#include "MCRegion.h"

class MCRegionManager {
private:
    MCRegionManager();
    
public:
    ~MCRegionManager();
    
    static MCRegionManager *sharedRegionManager();
    
    MCRegion *regionForObjectId(mc_object_id_t anObjectId);
    
    MCRegion *forestRegion();
    MCRegion *desertRegion();
    MCRegion *glacierRegion();
    MCRegion *dungeonRegion();
    
private:
    void loadRegions();
    
    CCDictionary *regions_;
};

#endif /* defined(__Military_Confrontation__MCRegionManager__) */
