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
nameForMCTaskType(MCTaskType aTaskType)
{
    switch (aTaskType) {
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
    switch (taskCategory_) {
        case MCMoveToTask:
            loadMoveToTaskContext(taskContext);
            break;
        case MCCollectionTask:
            loadCollectionTaskContext(taskContext);
            break;
        case MCHuntTask:
            loadHuntTaskContext(taskContext);
            break;
    }
}

void
MCTask::loadMoveToTaskContext(JsonBox::Object &aTaskContext)
{
    MCMoveToTaskContext *taskContext = new MCMoveToTaskContext;
    taskContext_ = taskContext;
    
    /* taskContext["completed"] Number */
    taskContext_->completed_ = aTaskContext["completed"].getInt();
    
    /* taskContext["steps"] Array */
}

void
MCTask::loadCollectionTaskContext(JsonBox::Object &aTaskContext)
{
    MCCollectionTaskContext *taskContext = new MCCollectionTaskContext;
    taskContext_ = taskContext;
    
    /* taskContext["completed"] Number */
    taskContext->completed_ = aTaskContext["completed"].getInt();
    
    /* taskContext["id"] String */
    const char *c_str_o_id = aTaskContext["id"].getString().c_str();
    taskContext->o_id_.class_ = c_str_o_id[0];
    taskContext->o_id_.sub_class_ = c_str_o_id[1];
    taskContext->o_id_.index_ = c_str_o_id[2];
    taskContext->o_id_.sub_index_ = c_str_o_id[3];
    
    /* taskContext["need"] Number */
    taskContext->need_ = aTaskContext["need"].getInt();
}

void
MCTask::loadHuntTaskContext(JsonBox::Object &aTaskContext)
{
    MCHuntTaskContext *taskContext = new MCHuntTaskContext;
    taskContext_ = taskContext;
    
    /* taskContext["completed"] Number */
    taskContext->completed_ = aTaskContext["completed"].getInt();
    
    /* taskContext["id"] String */
    const char *c_str_o_id = aTaskContext["id"].getString().c_str();
    taskContext->o_id_.class_ = c_str_o_id[0];
    taskContext->o_id_.sub_class_ = c_str_o_id[1];
    taskContext->o_id_.index_ = c_str_o_id[2];
    taskContext->o_id_.sub_index_ = c_str_o_id[3];
    
    /* taskContext["need"] Number */
    taskContext->need_ = aTaskContext["need"].getInt();
}
