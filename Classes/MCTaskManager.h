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
    
    MCTask *protoTaskWithObjectId(mc_object_id_t anObjectId);
    
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
    int acceptTask(MCTask *task);
    
    /**
     * 以任务ID接受一个任务，成功接受返回true，否则返回false
     */
    int acceptTaskWithObjectId(mc_object_id_t anObjectId);
    
    /**
     * 放弃当前任务，成功放弃返回true，否则返回false
     */
    void abortCurrentTask();
    
//    /**
//     * 放弃一个任务，成功放弃返回true，否则返回false
//     */
//    void abortTask(MCTask *task);
//    
//    /**
//     * 以任务ID放弃一个任务，成功放弃返回true，否则返回false
//     */
//    void abortTaskWithObjectId(mc_object_id_t anObjectId);
    
    void startCurrentTask();
    
    inline bool isTaskActiviting() {
        return currentTask_ && (currentTask_->getTaskStatus() == MCTaskActiviting);
    }
    
    CCArray *tasksForRegion(MCRegion *aRegion);
    
private:
    /**
     * 从数据包加载任务 
     */
    bool loadTasks();
    
    /**
     * 任务完成通知
     */
    void taskDidFinish(CCObject *obj);
    
private:
    MCTaskAccessor *taskAccessor_;        /* 任务访问器 */
    
    CC_SYNTHESIZE_READONLY(MCTask *, currentTask_, CurrentTask); /* 当前接受的任务 */
};

#endif /* defined(__Military_Confrontation__MCTaskManager__) */
