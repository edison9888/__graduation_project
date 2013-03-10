//
//  MCFlagManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-10.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCFlagManager__
#define __Military_Confrontation__MCFlagManager__

#include "MCFlag.h"

class MCFlagManager {
private:
    MCFlagManager();
    
public:
    ~MCFlagManager();
    
    static MCFlagManager *sharedFlagManager();
    
    MCFlag *createFlag(mc_object_id_t anObjectId);
    
    MCFlagState flagStateForObjectId(mc_object_id_t anObjectId);
    
private:
    /**
     * load all flags
     */
    void loadAllFlags();
};

#endif /* defined(__Military_Confrontation__MCFlagManager__) */
