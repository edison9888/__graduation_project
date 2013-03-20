//
//  MCOreManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-20.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCOreManager__
#define __Military_Confrontation__MCOreManager__

#include "MCOre.h"

class MCOreManager {
private:
    MCOreManager();
    
public:
    ~MCOreManager();
    
    static MCOreManager *sharedOreManager();
    
    MCOre *oreForObjectId(mc_object_id_t anObjectId) const;
    
    MCOre *defaultOre() const;
    
private:
    
    /**
     * load all flags
     */
    void loadAllOres();
    
    CCDictionary *ores_;
};

#endif /* defined(__Military_Confrontation__MCOreManager__) */
