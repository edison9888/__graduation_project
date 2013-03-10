//
//  MCTask.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTask.h"

const char *MCMainlineTaskName      = "主线任务";
const char *MCSideQuestName         = "支线任务";
const char *MCMercenaryMissionName  = "佣兵任务";
const char *MCArenaTaskName         = "竞技场任务";

const char *
nameForMCTaskType(MCTaskCategory aTaskCategory)
{
    switch (aTaskCategory) {
        case MCMainlineTask:
            return MCMainlineTaskName;
        case MCSideQuest:
            return MCSideQuestName;
        case MCMercenaryMission:
            return MCMercenaryMissionName;
        case MCArenaTask:
            return MCArenaTaskName;
            
        default:
            return NULL;
    }
}

void
MCTask::loadTask(JsonBox::Object &aTask)
{
    JsonBox::Object taskContext;
    
    /* task["done"] Number */
        //load from another file
//    isDone_ = false;
    
    /* task["category"] Number */
    taskCategory_ = aTask["category"].getInt();
    
    /* task["name"] String */
    setName(CCString::create(aTask["name"].getString()));
    
    /* task["description"] String */
    setDescription(CCString::create(aTask["description"].getString()));
    
    /* task["task"] Object */
    taskContext = aTask["task"].getObject();
}
