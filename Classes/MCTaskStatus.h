//
//  MCTaskStatus.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-4.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskStatus__
#define __Military_Confrontation__MCTaskStatus__

#include "MCTask.h"

class MCTaskStatus : public CCObject {
public:
    bool isDone(); /* 任务是否已经完成 */
    
    CC_SYNTHESIZE_READONLY(MCTask *, task_, Task); /* 对应的任务 */
    CC_SYNTHESIZE(mc_size_t, progress_, Progress); /* 已完成的子任务数 */
};

class MCGiveTaskStatus : public MCTaskStatus {
public:
    CC_SYNTHESIZE_READONLY(CCDictionary *, items_, Items); /* 已给予的物品的字典，key为物品的ID，value为物品剩余需要给予的数量 */
};

class MCHuntTaskStatus : public MCTaskStatus {
public:
    CC_SYNTHESIZE_READONLY(CCDictionary *, targets_, Targets); /* 已完成狩猎的目标的字典，key为目标的ID，value为目标的完成数量 */
};

class MCCollectionTaskStatus : public MCTaskStatus {
public:
    CC_SYNTHESIZE_READONLY(CCDictionary *, targets_, Targets); /* 已完成收集/拯救目标的字典，key为目标的ID，value为目标的完成数量 */
};

#endif /* defined(__Military_Confrontation__MCTaskStatus__) */
