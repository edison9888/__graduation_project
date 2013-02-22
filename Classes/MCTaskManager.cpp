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
MCTaskAccessor::loadTasks(MCTaskType aTaskType, const char *aFilePath)
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
                task->taskType_ = aTaskType;
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
        mainlineTaskAccessor_->loadTasks(MCMainlineTask,
                                         fileUtils->fullPathFromRelativePath(__mainline_tasks_package_file_path));
        
        /* 加载支线任务 */
        sideQuestAccessor_ = new MCTaskAccessor;
        sideQuestAccessor_->loadTasks(MCSideQuest,
                                      fileUtils->fullPathFromRelativePath(__side_quests_package_file_path));
        
        /* 加载佣兵任务 */
        mercenaryMissionAccessor_ = new MCTaskAccessor;
        mercenaryMissionAccessor_->loadTasks(MCMercenaryMission,
                                             fileUtils->fullPathFromRelativePath(__mercenary_missions_package_file_path));
        
        /* 加载竞技场任务 */
        arenaTaskAccessor_ = new MCTaskAccessor;
        arenaTaskAccessor_->loadTasks(MCArenaTask,
                                      fileUtils->fullPathFromRelativePath(__arena_tasks_package_file_path));
        
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
        case MCArenaTask:
            task = arenaTaskAccessor_->taskWithObjectId(anObjectId);
            break;
    }
    
    return task;
}
