//
//  MCTaskManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskManager__
#define __Military_Confrontation__MCTaskManager__

#include "MCTask.h"

class MCTaskAccessor {
public:
    MCTaskAccessor();
    ~MCTaskAccessor();
    
    bool loadTasks(MCTaskType aTaskType, const char *aFilePath);
    
    MCTask *taskWithObjectId(mc_object_id_t anObjectId);
    
private:
    MCTaskType taskType_;
    CCDictionary *tasks_;
};

class MCTaskManager {
private:
    MCTaskManager() {};
    
public:
    ~MCTaskManager();
    
    static MCTaskManager *sharedTaskManager();
    
    bool loadTasks();
    
    MCTask *taskWithObjectId(mc_object_id_t anObjectId);
    
private:
    MCTaskAccessor *mainlineTaskAccessor_;      /* 主线任务访问器 */
    MCTaskAccessor *sideQuestAccessor_;         /* 支线任务访问器 */
    MCTaskAccessor *mercenaryMissionAccessor_;  /* 佣兵任务访问器 */
    MCTaskAccessor *arenaTaskAccessor_;         /* 竞技场任务访问器 */
};

#endif /* defined(__Military_Confrontation__MCTaskManager__) */
