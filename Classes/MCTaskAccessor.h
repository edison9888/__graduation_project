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
    
    /**
     * 从数据包加载任务
     */
    bool loadTasks(const char *aFilePath);
    
    /**
     * 储存数据
     */
    void saveData();
    
    /**
     * 从存档读取数据
     */
    void loadData();
    
    MCTask *taskWithObjectId(mc_object_id_t anObjectId);
    
private:
    CC_SYNTHESIZE_READONLY(CCDictionary *, tasks_, Tasks);
};

#endif /* defined(__Military_Confrontation__MCTaskAccessor__) */
