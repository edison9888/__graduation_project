//
//  MCRegion.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCRegion.h"

bool
MCRegion::hasScene(mc_object_id_t aSceneId)
{
    std::vector<mc_object_id_t>::iterator iterator;
    
    for (iterator = regions.begin();
         iterator != regions.end();
         ++iterator) {
        mc_object_id_t id = *iterator;
        if (MCObjectIdIsEqualsTo(id, aSceneId)) {
            return true;
        }
    }
    
    return false;
}