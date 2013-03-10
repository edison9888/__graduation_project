//
//  MCTaskAccessor.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTaskAccessor.h"


MCTaskAccessor::MCTaskAccessor()
{
    tasks_ = CCDictionary::create();
    tasks_->retain();
}

MCTaskAccessor::~MCTaskAccessor()
{
    CC_SAFE_RELEASE(tasks_);
}

bool
MCTaskAccessor::loadTasks(const char *aFilePath)
{
    bool result = false;
    
    do {
        JsonBox::Value in;
        JsonBox::Object o;
        JsonBox::Object tasksDict;
        JsonBox::Object::iterator oIter;
        
        in.loadFromFile(aFilePath);
        CC_BREAK_IF(! in.isObject());
        
        tasksDict = in.getObject();
        for (oIter = tasksDict.begin(); oIter != tasksDict.end(); ++oIter) {
            std::string t_id = oIter->first;
            const char *s_t_id = t_id.c_str();
            mc_object_id_t o_id = {
                s_t_id[0],
                s_t_id[1],
                s_t_id[2],
                s_t_id[3]
            };
            MCTask *task = new MCTask;
            if (task) {
                task->setID(o_id);
                o = oIter->second.getObject();
                task->loadTask(o);
                    //                task->taskType_ = aTaskType;
                tasks_->setObject(task, MCObjectIdToDickKey(o_id));
            }
        }
        
        result = true;
    } while (0);
    
    return result;
}

MCTask *
MCTaskAccessor::taskWithObjectId(mc_object_id_t anObjectId)
{
    return (MCTask *) tasks_->objectForKey(MCObjectIdToDickKey(anObjectId));
}
