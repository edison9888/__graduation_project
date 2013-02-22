//
//  MCTask.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTask__
#define __Military_Confrontation__MCTask__

#include "MCObject.h"
#include "JsonBox.h"

/* 任务类型 */
enum {
    MCMainlineTask      = MCMakeEnum(0), /* 主线任务 */
    MCSideQuest         = MCMakeEnum(1), /* 支线任务 */
    MCMercenaryMission  = MCMakeEnum(2), /* 佣兵任务 */
    MCArenaTask         = MCMakeEnum(3)  /* 竞技场任务 */
};
typedef mc_enum_t MCTaskType;

extern const char *MCMainlineTaskName;
extern const char *MCSideQuestName;
extern const char *MCMercenaryMissionName;
extern const char *MCArenaTaskName;

const char *nameForMCTaskType(MCTaskType aTaskType);

/* 任务类别 */
enum {
    MCMoveToTask        = MCMakeEnum(0), /* 移动型任务 */
    MCCollectionTask    = MCMakeEnum(1), /* 收集型任务 */
    MCHuntTask          = MCMakeEnum(2), /* 狩猎型任务 */
};
typedef mc_enum_t MCTaskCategory;

/* 任务上下文 */
class MCTaskContext {
    friend class MCTask;
    
    CC_SYNTHESIZE_READONLY(mc_index_t, completed_, Completed); /* 任务进度 */
};

/* 移动型任务上下文 */
class MCMoveToTaskContext : public MCTaskContext {
public:
    CCArray *steps_;
};

/* 收集型任务上下文 */
class MCCollectionTaskContext : public MCTaskContext {
public:
    mc_object_id_t o_id_;
    mc_index_t need_;
};

/* 狩猎型任务上下文 */
class MCHuntTaskContext : public MCCollectionTaskContext {
};

class MCTask : public MCObject {
public:
    friend class MCTaskAccessor;
    
    inline bool isDone() {
        return isDone_;
    }
    
    void loadTask(JsonBox::Object &aTask);
    
private:
    void loadMoveToTaskContext(JsonBox::Object &aTaskContext);
    void loadCollectionTaskContext(JsonBox::Object &aTaskContext);
    void loadHuntTaskContext(JsonBox::Object &aTaskContext);
    
    bool isDone_; /* 任务是否已完成 */

    CC_SYNTHESIZE_READONLY(MCTaskType, taskType_, TaskType); /* 任务类型 */
    CC_SYNTHESIZE_READONLY(MCTaskCategory, taskCategory_, TaskCategory); /* 任务类别 */
    CC_SYNTHESIZE_READONLY(MCTaskContext *, taskContext_, TaskContext); /* 任务上下文 */
};

#endif /* defined(__Military_Confrontation__MCTask__) */
