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

class MCFlag;

enum {
    MCTaskUnknownStatus = 0,
    MCTaskDone          = MCMakeEnum(0),
    MCTaskActiviting    = MCMakeEnum(1),
    MCTaskUncompleted   = MCMakeEnum(2)
};
typedef mc_enum_t MCTaskStatus;

class MCTaskContext;

class MCTask : public MCObject {
    friend class MCTaskAccessor;
public:
    MCTask();
    ~MCTask();
    
private:
    void loadTaskContent(JsonBox::Object &aTaskContent);
    
    CC_SYNTHESIZE_READONLY(CCArray *, bonus_, Bonus);                    /* 任务奖励 */
    CC_SYNTHESIZE_READONLY(MCTaskContext *, taskContext_, TaskContext);  /* 任务内容 */
    CC_PROPERTY(MCTaskStatus, taskStatus_, TaskStatus);                  /* 任务状态 */
    CC_SYNTHESIZE(MCFlag *, flag_, Flag);                                /* 开启标签 */
};

#endif /* defined(__Military_Confrontation__MCTask__) */
