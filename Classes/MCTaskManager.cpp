//
//  MCTaskManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTaskManager.h"
#include "MCFlagManager.h"
#include "MCRegion.h"
#include "MCBackpack.h"
#include "MCBase64.h"
#include "MCGameState.h"
#include "MCSceneController.h"
#include "MCTeam.h"
#include "MCTaskContext.h"
#include "MCToast.h"
#include "MCMercenaryManager.h"

using namespace std;

static const char *kMCTaskPackageFilepath = "T000.jpkg";
const char *kMCTaskDidFinishNotification = "kMCTaskDidFinishNotification";

#if MC_DEBUG_SAVEDATA == 1
static const char *kMCCurrentTaskKey = "current-task";
#else
static const char *kMCCurrentTaskKey = "Y3VycmVudC10YXNr"; /* current-task的BASE64编码 */
#endif

static mc_object_id_t kMCAreaBlockedTaskId = {
    'T', '2', '1', '0'
};

static MCTaskManager *__shared_task_manager = NULL;

MCTaskManager::~MCTaskManager()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kMCTaskDidFinishNotification);
    CC_SAFE_DELETE(taskAccessor_); /* 非CCObject子类 */
}

MCTaskManager *
MCTaskManager::sharedTaskManager()
{
    if (__shared_task_manager == NULL) {
        __shared_task_manager = new MCTaskManager;
        __shared_task_manager->loadTasks();
        
        CCNotificationCenter *notificationCenter = CCNotificationCenter::sharedNotificationCenter();
        notificationCenter->addObserver(__shared_task_manager,
                                        callfuncO_selector(MCTaskManager::taskDidFinish),
                                        kMCTaskDidFinishNotification,
                                        NULL);
    }
    
    return __shared_task_manager;
}

MCTask *
MCTaskManager::taskWithObjectId(mc_object_id_t anObjectId)
{
    MCTask *task = dynamic_cast<MCTask *>(protoTaskWithObjectId(anObjectId)->copy());
    
    return task;
}


MCTask *
MCTaskManager::protoTaskWithObjectId(mc_object_id_t anObjectId)
{
    return taskAccessor_->taskWithObjectId(anObjectId);
}

/**
 * 储存数据
 */
void
MCTaskManager::saveData()
{
    taskAccessor_->saveData();
    
    string data;
    if (currentTask_) {
        mc_byte_t b_o_id[5] = {0};
        const char *c_s_o_id = (const char *) b_o_id;
        mc_object_id_t o_id = currentTask_->getID();
        b_o_id[0] = o_id.class_;
        b_o_id[1] = o_id.sub_class_;
        b_o_id[2] = o_id.index_;
        b_o_id[3] = o_id.sub_index_;
        data.append(c_s_o_id);
    }
    
#if MC_DEBUG_SAVEDATA == 1
    const char *output = data.c_str();
#else
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
    CCUserDefault::sharedUserDefault()->setStringForKey(kMCCurrentTaskKey, output);
#if MC_DEBUG_SAVEDATA != 1
    delete []output;
#endif
}

/**
 * 从存档读取数据
 */
void
MCTaskManager::loadData()
{
    taskAccessor_->loadData();
    
    string data = CCUserDefault::sharedUserDefault()->getStringForKey(kMCCurrentTaskKey, "");
    if (MCGameState::sharedGameState()->isSaveFileExists() && data.size() > 0) {
#if MC_DEBUG_SAVEDATA == 1
        const char *output = data.c_str();
#else
        const char *input = data.c_str();
        char  *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
        data.assign(output);
        
        const char *c_s_o_id = data.c_str();
        mc_object_id_t o_id = {
            c_s_o_id[0],
            c_s_o_id[1],
            c_s_o_id[2],
            c_s_o_id[3]
        };
        currentTask_ = taskWithObjectId(o_id);
        currentTask_->setTaskStatus(MCTaskAccepted);
    }
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
int
MCTaskManager::acceptTask(MCTask *task)
{
    MCBackpack *backpack = MCBackpack::sharedBackpack();
//    MCTaskStatus status = task->getTaskStatus();
    
//    if (status == MCTaskUncompleted) {
        if (task->getCashPledge() > backpack->getMoney()) { /* 不够钱接受任务~~~~ */
            return kMCNotEnoughMoney;
        }
        currentTask_ = task;
        task->setTaskStatus(MCTaskAccepted);
        backpack->setMoney(backpack->getMoney() - task->getCashPledge());
        return kMCHandleSucceed;
//    }
    
    return kMCHandleFailured;
}

/**
 * 以任务ID接受一个任务，成功接受返回true，否则返回false
 */
int
MCTaskManager::acceptTaskWithObjectId(mc_object_id_t anObjectId)
{
    return acceptTask(taskWithObjectId(anObjectId));
}

/**
 * 放弃当前任务，成功放弃返回true，否则返回false
 */
void
MCTaskManager::abortCurrentTask()
{
    if (currentTask_) {
        currentTask_->setTaskStatus(MCTaskUncompleted);
        MCFlagManager *flagManager = MCFlagManager::sharedFlagManager();
        flagManager->setTaskStarted(false);
        flagManager->setSpawnFlagOff();
        currentTask_ = NULL;
    }
}

///**
// * 放弃一个任务，成功放弃返回true，否则返回false
// */
//void
//MCTaskManager::abortTask(MCTask *task)
//{
//    task->setTaskStatus(MCTaskUncompleted);
//    MCFlagManager::sharedFlagManager()->setTaskStarted(false);
//}
//
///**
// * 以任务ID放弃一个任务，成功放弃返回true，否则返回false
// */
//void
//MCTaskManager::abortTaskWithObjectId(mc_object_id_t anObjectId)
//{
//    abortTask(taskWithObjectId(anObjectId));
//}

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
        taskAccessor_->loadTasks(fileUtils->fullPathForFilename(kMCTaskPackageFilepath).c_str());
        
        result = true;
    } while (0);
    
    return result;
}

void
MCTaskManager::startCurrentTask()
{
    if (currentTask_) {
        /* 建立任务上下文 */
        currentTask_->generateTaskContext();
        currentTask_->setTaskStatus(MCTaskActiviting);
        MCFlagManager::sharedFlagManager()->setTaskStarted(true);
        
        /* 佣兵技能效果 */
        MCTaskContext *taskContext = currentTask_->getTaskContext();
        CCArray *team = MCTeam::sharedTeam()->getRoles();
        CCObject *obj;
        MCRole *role;
        CCARRAY_FOREACH(team, obj) {
            role = dynamic_cast<MCRole *>(obj);
            role->setMaxHP(role->getMaxHP() + taskContext->getHP());
            role->setHP(role->getMaxHP());
            role->setMaxPP(role->getMaxPP() + taskContext->getPP());
            role->setPP(role->getMaxPP());
            role->setAdditionalDamageBonus(taskContext->getDamageBonus());
            role->setAdditionalArmorCheckPenalty(taskContext->getArmorCheckPenalty());
        }
    }
}

CCArray *
MCTaskManager::tasksForRegion(MCRegion *aRegion)
{
    mc_object_id_t r_id = aRegion->getID();
    CCArray *array = CCArray::create();
    CCDictionary *tasks = taskAccessor_->getTasks();
    CCDictElement *elem;
    MCTask *task;
    
    array->retain();
    CCDICT_FOREACH(tasks, elem) {
        task = dynamic_cast<MCTask *>(elem->getObject());
        if (task->getID().sub_class_ == r_id.sub_class_) {
            array->addObject(task);
        }
    }
    
    return array;
}

void
MCTaskManager::taskDidFinish(CCObject *obj)
{
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    MCTask *task = dynamic_cast<MCTask *>(obj);
    MCTaskContext *taskContext = task->getTaskContext();
    MCBackpackItem *backpackItem;
    MCBackpackItem *trapWide = NULL;
    MCBackpackItem *trapDamage = NULL;
    mc_size_t used;
    mc_price_t bonus = task->getBonus();
    mc_object_id_t taskId = task->getID();
    MCToastLength toastLength = 0;
    
    task->setTaskStatus(MCTaskDone);
    /* 清点任务物品 */
    /**
     * 数量(used)=数量(after)-数量(before)
     * 结算背包物品数量=本身数量+赠送数量-数量(used)
     */
    backpackItem = backpack->getHealthPotion();
    used = taskContext->getHealthPotion()->count - taskContext->getHealthPotionCount();
    backpackItem->count = backpackItem->count + task->getHealthPotionBonus() - used;
    backpackItem = backpack->getPhysicalPotion();
    used = taskContext->getPhysicalPotion()->count - taskContext->getPhysicalPotionCount();
    backpackItem->count = backpackItem->count + task->getPhysicalPotionBonus() - used;
    switch (task->getTrapType()) {
        case MCFireballTrap:
            trapWide = backpack->getFireballTrapWide();
            trapDamage = backpack->getFireballTrapDamage();
            break;
        case MCCurseTrap:
            trapWide = backpack->getCurseTrapWide();
            trapDamage = backpack->getCurseTrapDamage();
            break;
        case MCParalysisTrap:
            trapWide = backpack->getParalysisTrapWide();
            trapDamage = backpack->getParalysisTrapDamage();
            break;
        case MCFogTrap:
            trapWide = backpack->getFogTrapWide();
            trapDamage = backpack->getFogTrapDamage();
            break;
        case MCFlashTrap:
            trapWide = backpack->getFlashTrapWide();
            trapDamage = backpack->getFlashTrapDamage();
            break;
        default:
            CCAssert(task->getTrapType() != MCUnknownTrap, "unknow trap");
            break;
    }
    used = taskContext->getTrapWide()->count - taskContext->getTrapWideCount();
    trapWide->count = trapWide->count + task->getTrapWideBonus() - used;
    used = taskContext->getTrapDamage()->count - taskContext->getTrapDamageCount();
    trapDamage->count = trapDamage->count + task->getTrapDamageBonus() - used;
    
    /* 解除佣兵技能效果 */
    CCArray *team = MCTeam::sharedTeam()->getRoles();
    MCRole *role;
    CCARRAY_FOREACH(team, obj) {
        role = dynamic_cast<MCRole *>(obj);
        role->setMaxHP(role->getMaxHP() - taskContext->getHP());
        role->setMaxPP(role->getMaxPP() - taskContext->getPP());
        role->setAdditionalDamageBonus(0);
        role->setAdditionalArmorCheckPenalty(0);
    }
    
    /* 恢复人物状态 */
    MCTeam::sharedTeam()->restoreRoles();
    
    /* 任务奖励 */
    MCToast *toast = MCToast::make(NULL,
                                   CCString::createWithFormat("任务已完成，获得%d马克的奖励！", bonus)->getCString());
    toast->show();
    backpack->setMoney(backpack->getMoney() + bonus);
    
    /* 写死吧= -!检测高级任务解锁，若T210完成则解锁 */
    if (MCObjectIdIsEqualsTo(taskId, kMCAreaBlockedTaskId)) {
        MCFlagManager::sharedFlagManager()->unblockArea();
        toastLength += MCLengthShort;
        MCToast::make(NULL, "高级任务已解锁!")->show();
    }
    
    currentTask_ = NULL;
    CC_SAFE_RELEASE(task); /* 由于是copy出来的，所以要清理下 */
    MCFlagManager::sharedFlagManager()->setTaskStarted(false);
    
    MCGameState::sharedGameState()->save();
    /* 回重生点 */
    MCSceneController::sharedSceneController()->loadSpawnScene(toast->getLength() * 1.5 + toastLength);
}
