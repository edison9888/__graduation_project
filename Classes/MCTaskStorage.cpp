//
//  MCTaskStorage.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTaskStorage.h"

static MCTaskStorage *__shared_task_storage = NULL;

MCTaskStorage::MCTaskStorage()
{
    tasks_ = CCDictionary::create();
    tasks_->retain();
}

MCTaskStorage::~MCTaskStorage()
{
    CC_SAFE_RELEASE(tasks_);
}

MCTaskStorage *
MCTaskStorage::sharedTaskStorage()
{
    if (__shared_task_storage == NULL) {
        __shared_task_storage = new MCTaskStorage;
    }
    
    return __shared_task_storage;
}

MCTaskStatus *
MCTaskStorage::statusForTaskWithObjectId(mc_object_id_t anObjectId)
{
    return (MCTaskStatus *) tasks_->objectForKey(MCObjectIdToDickKey(anObjectId));
}

void
MCTaskStorage::save()
{
    
}
void
MCTaskStorage::load()
{
    /* 检查是否有任务记录 */
}
