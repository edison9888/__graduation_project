//
//  MCTaskAccessor.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskAccessor__
#define __Military_Confrontation__MCTaskAccessor__

#include "MCTask.h"

class MCTaskAccessor {
public:
    MCTaskAccessor();
    ~MCTaskAccessor();
    
    bool loadTasks(const char *aFilePath);
    
    MCTask *taskWithObjectId(mc_object_id_t anObjectId);
    
private:
    CCDictionary *tasks_;
};

#endif /* defined(__Military_Confrontation__MCTaskAccessor__) */
