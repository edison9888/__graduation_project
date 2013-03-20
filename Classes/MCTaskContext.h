//
//  MCTaskContext.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-4.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskContext__
#define __Military_Confrontation__MCTaskContext__

#include "MCTask.h"

/* 任务上下文 */
class MCActionContext : public MCObject {
    friend class MCTask;
    
    CC_SYNTHESIZE_READONLY(CCDictionary *, targets_, Targets); /* 目标的字典，key为目标的ID，value为目标的数量 */
};

#endif /* defined(__Military_Confrontation__MCTaskContext__) */
