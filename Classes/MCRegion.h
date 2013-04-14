//
//  MCRegion.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRegion__
#define __Military_Confrontation__MCRegion__

#include "MCObject.h"
#include <vector>

enum {
    MCVillage   = MCMakeEnum(0),
    MCCity      = MCMakeEnum(1),
    MCForest    = MCMakeEnum(2),
    MCDesert    = MCMakeEnum(3),
    MCGlacier   = MCMakeEnum(4),
    MCDungeon   = MCMakeEnum(5)
};
typedef mc_enum_t MCSceneRegion;

class MCRegion : public MCObject {
public:
    std::vector<mc_object_id_t> regions;
    
    bool hasScene(mc_object_id_t aSceneId);
    
    CC_SYNTHESIZE(MCSceneRegion, sceneRegion_, SceneRegion);
};

#endif /* defined(__Military_Confrontation__MCRegion__) */
