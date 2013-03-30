//
//  MCTaskManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTaskManager.h"
#include "MCFlagManager.h"

const char *kMCTaskPackageFilepath = "T000.jpkg";
const char *kMCTaskDidFinishNotification = "kMCTaskDidFinishNotification";

static MCTaskManager *__shared_task_manager = NULL;

MCTaskManager::~MCTaskManager()
{
    CC_SAFE_DELETE(taskAccessor_); /* 非CCObject子类 */
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
    MCTask *task = (MCTask *) taskAccessor_->taskWithObjectId(anObjectId)->copy();
    
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
        MCFlagManager::sharedFlagManager()->setTaskStarted(true);
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
    MCFlagManager::sharedFlagManager()->setTaskStarted(false);
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
        taskAccessor_->loadTasks(fileUtils->fullPathFromRelativePath(kMCTaskPackageFilepath));
        
        result = true;
    } while (0);
    
    return result;
}

void
MCTaskManager::startCurrentTask()
{
    if (currentTask_) {
        CCNotificationCenter *notificationCenter = CCNotificationCenter::sharedNotificationCenter();
        notificationCenter->addObserver(currentTask_,
                                        callfuncO_selector(MCTaskManager::taskDidFinish),
                                        kMCTaskDidFinishNotification,
                                        currentTask_);
        /* 建立任务上下文 */
        currentTask_->generateTaskContext();
    }
}

void
MCTaskManager::taskDidFinish(CCObject *obj)
{
    CCNotificationCenter *notificatinCenter = CCNotificationCenter::sharedNotificationCenter();
    MCTask *task = dynamic_cast<MCTask *>(obj);
    
    /* 清点任务 */
    task->setTaskStatus(MCTaskDone);
    
    notificatinCenter->removeObserver(task, kMCTaskDidFinishNotification);
    delete task; /* 由于是copy出来的，所以要清理下 */
    MCFlagManager::sharedFlagManager()->setTaskStarted(false);
}
