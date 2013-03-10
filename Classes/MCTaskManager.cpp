//
//  MCTaskManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTaskManager.h"

const char *__mainline_tasks_package_file_path = "tasks/1.tpkg";
const char *__side_quests_package_file_path = "tasks/2.tpkg";
const char *__mercenary_missions_package_file_path = "tasks/3.tpkg";
const char *__arena_tasks_package_file_path = "tasks/4.tpkg";

static MCTaskManager *__shared_task_manager = NULL;

MCTaskManager::~MCTaskManager()
{
    CC_SAFE_DELETE(mainlineTaskAccessor_);
    CC_SAFE_DELETE(sideQuestAccessor_);
    CC_SAFE_DELETE(mercenaryMissionAccessor_);
    CC_SAFE_DELETE(arenaTaskAccessor_);
}

MCTaskManager *
MCTaskManager::sharedTaskManager()
{
    if (__shared_task_manager == NULL) {
        __shared_task_manager = new MCTaskManager;
    }
    
    return __shared_task_manager;
}

bool
MCTaskManager::loadTasks()
{
    bool result = false;
    
    do {
        CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
        /* 加载主线任务 */
        mainlineTaskAccessor_ = new MCTaskAccessor;
        mainlineTaskAccessor_->loadTasks(fileUtils->fullPathFromRelativePath(__mainline_tasks_package_file_path));
        
        /* 加载支线任务 */
        sideQuestAccessor_ = new MCTaskAccessor;
        sideQuestAccessor_->loadTasks(fileUtils->fullPathFromRelativePath(__side_quests_package_file_path));
        
        /* 加载佣兵任务 */
        mercenaryMissionAccessor_ = new MCTaskAccessor;
        mercenaryMissionAccessor_->loadTasks(fileUtils->fullPathFromRelativePath(__mercenary_missions_package_file_path));
        
        result = true;
    } while (0);
    
    return result;
}

MCTask *
MCTaskManager::taskWithObjectId(mc_object_id_t anObjectId)
{
    MCTask *task = NULL;
    switch (anObjectId.sub_class_ - '0') {
        case MCMainlineTask:
            task = mainlineTaskAccessor_->taskWithObjectId(anObjectId);
            break;
        case MCSideQuest:
            task = sideQuestAccessor_->taskWithObjectId(anObjectId);
            break;
        case MCMercenaryMission:
            task = mercenaryMissionAccessor_->taskWithObjectId(anObjectId);
            break;
    }
    
    return task;
}

/**
 * 当前接受的任务列表
 */
CCArray *
MCTaskManager::currentTasks()
{
    if (currentTasks_ == NULL) {
        /* load current tasks */
    }
    return currentTasks_;
}

/**
 * 未完成的任务列表
 */
CCArray *
MCTaskManager::unfinishedTasks()
{
    if (unfinishedTasks_ == NULL) {
        /* load unfinished tasks */
    }
    return unfinishedTasks_;
}

/**
 * 接受一个任务，成功接受返回true，否则返回false
 */
bool
MCTaskManager::acceptTask(MCTask *task)
{
    CCArray *unfinishedTasks = this->unfinishedTasks();
    CCArray *currentTasks = this->currentTasks();
    if (unfinishedTasks->containsObject(task) /* 在未完成任务列表里 */
        && !currentTasks->containsObject(task)) { /* 且不在已接受任务列表里 */
        currentTasks->addObject(task);
        /* 刷新任务面板 */
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
