//
//  MCTask.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTask.h"

const char *kMCSideQuestName = "支线任务";
const char *kMCGuildQuestName = "公会任务";

const char *
nameForMCTaskType(MCTaskCategory aTaskCategory)
{
    switch (aTaskCategory) {
        case MCSideQuest:
            return kMCSideQuestName;
        case MCGuildQuest:
            return kMCGuildQuestName;
            
        default:
            return NULL;
    }
}

void
MCTask::loadTask(JsonBox::Object &aTask)
{
    JsonBox::Array taskContent;
    
    /* task["done"] Number */
        //load from another file
//    isDone_ = false;
    
    /* task["category"] Number */
    taskCategory_ = aTask["category"].getInt();
    
    /* task["name"] String */
    setName(CCString::create(aTask["name"].getString()));
    
    /* task["description"] String */
    setDescription(CCString::create(aTask["description"].getString()));
    
    /* task["content"] Array */
    taskContent = aTask["content"].getArray();
}
