//
//  MCTaskManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskManager__
#define __Military_Confrontation__MCTaskManager__

#include "MCTaskAccessor.h"

class MCTaskManager {
private:
    MCTaskManager() :
    currentTasks_(NULL),
    unfinishedTasks_(NULL)
    {};
    
public:
    ~MCTaskManager();
    
    static MCTaskManager *sharedTaskManager();
    
    bool loadTasks();
    
    MCTask *taskWithObjectId(mc_object_id_t anObjectId);
    
    /**
     * 当前接受的任务列表
     */
    CCArray *currentTasks();
    
    /**
     * 未完成的任务列表
     */
    CCArray *unfinishedTasks();
    
    /**
     * 接受一个任务，成功接受返回true，否则返回false
     */
    bool acceptTask(MCTask *task);
    
    /**
     * 以任务ID接受一个任务，成功接受返回true，否则返回false
     */
    bool acceptTaskWithObjectId(mc_object_id_t anObjectId);
    
private:
    MCTaskAccessor *sideQuestAccessor_;         /* 支线任务访问器 */
    MCTaskAccessor *guileQuestAccessor_;        /* 公会任务访问器 */
    
    CCArray *currentTasks_;    /* 当前接受的任务列表 */
    CCArray *unfinishedTasks_; /* 未完成的任务列表 */
};

#endif /* defined(__Military_Confrontation__MCTaskManager__) */
