//
//  MCTaskManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTaskManager.h"

    //warning: delete .json
const char *__task_package_file_path = "T000.tpkg.json";

static MCTaskManager *__shared_task_manager = NULL;

MCTaskManager::~MCTaskManager()
{
    CC_SAFE_DELETE(taskAccessor_);
}

MCTaskManager *
MCTaskManager::sharedTaskManager()
{
    if (__shared_task_manager == NULL) {
        __shared_task_manager = new MCTaskManager;
        __shared_task_manager->loadTasks();
    }
    
    return __shared_task_manager;
}

MCTask *
MCTaskManager::taskWithObjectId(mc_object_id_t anObjectId)
{
    MCTask *task = taskAccessor_->taskWithObjectId(anObjectId);
    
    return task;
}

/**
 * 储存数据
 */
void
MCTaskManager::saveData()
{
    taskAccessor_->saveData();
}

/**
 * 从存档读取数据
 */
void
MCTaskManager::loadData()
{
    taskAccessor_->loadData();
}

/**
 * 任务列表
 */
CCDictionary *
MCTaskManager::tasks()
{
    return taskAccessor_->getTasks();
}

/**
 * 接受一个任务，成功接受返回true，否则返回false
 */
bool
MCTaskManager::acceptTask(MCTask *task)
{
    MCTaskStatus status = task->getTaskStatus();
    if (status == MCTaskUncompleted) {
        currentTask_ = task;
        task->setTaskStatus(MCTaskActiviting);
        return true;
    }
    
    return false;
}

/**
 * 以任务ID接受一个任务，成功接受返回true，否则返回false
 */
bool
MCTaskManager::acceptTaskWithObjectId(mc_object_id_t anObjectId)
{
    return acceptTask(taskWithObjectId(anObjectId));
}

/**
 * 放弃一个任务，成功放弃返回true，否则返回false
 */
void
MCTaskManager::abortTask(MCTask *task)
{
    currentTask_ = NULL;
    task->setTaskStatus(MCTaskUncompleted);
}

/**
 * 以任务ID放弃一个任务，成功放弃返回true，否则返回false
 */
void
MCTaskManager::abortTaskWithObjectId(mc_object_id_t anObjectId)
{
    abortTask(taskWithObjectId(anObjectId));
}

/**
 * 完成一个任务，成功完成返回true，否则返回false
 */
void
MCTaskManager::taskDidComplete(MCTask *task)
{
    task->setTaskStatus(MCTaskDone);
}

/**
 * 以任务ID完成一个任务，成功完成返回true，否则返回false
 */
void
MCTaskManager::taskDidCompleteWithObjectId(mc_object_id_t anObjectId)
{
    taskDidComplete(taskWithObjectId(anObjectId));
}

/**
 * 从数据包加载任务
 */
bool
MCTaskManager::loadTasks()
{
    bool result = false;
    
    do {
        CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
        
        /* 加载任务 */
        taskAccessor_ = new MCTaskAccessor;
        taskAccessor_->loadTasks(fileUtils->fullPathFromRelativePath(__task_package_file_path));
        
        result = true;
    } while (0);
    
    return result;
}
