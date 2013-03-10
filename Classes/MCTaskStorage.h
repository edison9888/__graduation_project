//
//  MCTaskStorage.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskStorage__
#define __Military_Confrontation__MCTaskStorage__

#include "MCTaskStatus.h"

class MCTaskStorage {
private:
    MCTaskStorage();
    
public:
    ~MCTaskStorage();
    static MCTaskStorage *sharedTaskStorage();
    
    MCTaskStatus *statusForTaskWithObjectId(mc_object_id_t anObjectId);
    
    void save();
    void load();
    
private:
    CCDictionary *tasks_;
};

#endif /* defined(__Military_Confrontation__MCTaskStorage__) */
