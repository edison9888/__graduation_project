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
    currentTask_(NULL) {};
    
public:
    ~MCTaskManager();
    
    static MCTaskManager *sharedTaskManager();
    
    MCTask *taskWithObjectId(mc_object_id_t anObjectId);
    
    /**
     * 储存数据
     */
    void saveData();
    
    /**
     * 从存档读取数据
     */
    void loadData();
    
    /**
     * 任务列表
     */
    CCDictionary *tasks();
    
    /**
     * 接受一个任务，成功接受返回true，否则返回false
     */
    bool acceptTask(MCTask *task);
    
    /**
     * 以任务ID接受一个任务，成功接受返回true，否则返回false
     */
    bool acceptTaskWithObjectId(mc_object_id_t anObjectId);
    
    /**
     * 放弃一个任务，成功放弃返回true，否则返回false
     */
    void abortTask(MCTask *task);
    
    /**
     * 以任务ID放弃一个任务，成功放弃返回true，否则返回false
     */
    void abortTaskWithObjectId(mc_object_id_t anObjectId);
    
    /**
     * 完成一个任务，成功完成返回true，否则返回false
     */
    void taskDidComplete(MCTask *task);
    
    /**
     * 以任务ID完成一个任务，成功完成返回true，否则返回false
     */
    void taskDidCompleteWithObjectId(mc_object_id_t anObjectId);
    
private:
    /**
     * 从数据包加载任务 
     */
    bool loadTasks();
    
private:
    MCTaskAccessor *taskAccessor_;        /* 任务访问器 */
    
    MCTask *currentTask_;    /* 当前接受的任务 */
};

#endif /* defined(__Military_Confrontation__MCTaskManager__) */
