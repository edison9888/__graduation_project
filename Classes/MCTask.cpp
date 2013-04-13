//
//  MCTask.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTask.h"
#include "MCFlagManager.h"
#include "MCTaskBonus.h"
#include "MCTaskTarget.h"
#include "MCTaskContext.h"
#include "MCRegionManager.h"

MCTask::MCTask()
{
    targets_ = CCArray::create();
    targets_->retain();
    taskContext_ = NULL;
}

MCTask::~MCTask()
{
    CC_SAFE_DELETE(taskContext_); /* 非CCObject子类 */
    CC_SAFE_RELEASE(targets_);
}

/*
 字段                 |  字段说明  | 解释
 ---------------------|-----------|---------
 name           | 任务名称      | 任务的 名称
 targets        | 任务目标      | 任务的目标字典，key为目标的ID，value为目标的数量
 trap-type      | 陷阱类型      | 值为1-5，分别为火球、诅咒、麻痹、迷雾、闪光
 trap-limited   | 陷阱限制      | 二元组，第一个值为大范围陷阱的限制个数，第二个值为大伤害陷阱的限制个数
 potion-limited | 药水限制      | 二元组，第一个值为生命药水的限制个数，第二个值为体力药水的限制个数
 trap-bonus     | 赠送的陷阱数量 | 二元组，第一个值为大范围陷阱的赠送个数，第二个值为大伤害陷阱的赠送个数
 potion-bonus   | 赠送的药水数量 | 二元组，第一个值为生命药水的赠送个数，第二个值为体力药水的赠送个数
 cash-pledge    | 任务押金      | 任务的押金，完成任务后退回，任务失败后不退回
 bonus          | 任务奖励      | 顺利完成任务的奖励
 flag           | 开启的标志ID  | 接受任务后打开的标志的ID
 description    | 任务描述      | 任务的描述
 */
void
MCTask::loadTaskContent(JsonBox::Object &aTaskContent)
{
    CCString *ccstring;
    
    /* task["name"] String */
    ccstring = CCString::create(aTaskContent["name"].getString());
    setName(ccstring);
    ccstring->retain();
    
    /* task["targets"] Object */
    JsonBox::Object targets = aTaskContent["targets"].getObject();
    JsonBox::Object::iterator targetsIterator;
    for (targetsIterator = targets.begin();
         targetsIterator != targets.end();
         ++targetsIterator) {
        const char * c_str_t_id = targetsIterator->first.c_str();
        mc_object_id_t t_id = {
            c_str_t_id[0],
            c_str_t_id[1],
            c_str_t_id[2],
            c_str_t_id[3]
        };
        MCTaskTarget *target = new MCTaskTarget;
        target->autorelease();
        target->objectID = t_id;
        target->count = targetsIterator->second.getInt();
        target->remaining = target->count;
        targets_->addObject(target);
    }
    
    /* task["trap-type"] Integer */
    trapType_ = aTaskContent["trap-type"].getInt();
    
    /* task["trap-limited"] Array */
    JsonBox::Array array = aTaskContent["trap-limited"].getArray();
    trapWideMaxSize_ = array.at(0).getInt();
    trapDamageMaxSize_ = array.at(1).getInt();
    
    /* task["potion-limited"] Array */
    array = aTaskContent["potion-limited"].getArray();
    healthPotionMaxSize_ = array.at(0).getInt();
    physicalPotionMaxSize_ = array.at(1).getInt();
    
    /* task["trap-limited"] Array */
    array = aTaskContent["trap-bonus"].getArray();
    trapWideBonus_ = array.at(0).getInt();
    trapDamageBonus_ = array.at(1).getInt();
    
    /* task["potion-limited"] Array */
    array = aTaskContent["potion-bonus"].getArray();
    healthPotionBonus_ = array.at(0).getInt();
    physicalPotionBonus_ = array.at(1).getInt();
    
    /* task["cash-pledge"] Integer */
    cashPledge_ = aTaskContent["cash-pledge"].getInt();
    
    /* task["bonus"] Integer */
    bonus_ = aTaskContent["bonus"].getInt();
    
    /* task["flag"] String */
    const char *s_f_id = aTaskContent["flag"].getString().c_str();
    mc_object_id_t f_id = {
        s_f_id[0],
        s_f_id[1],
        s_f_id[2],
        s_f_id[3]
    };
    setFlag(MCFlagManager::sharedFlagManager()->flagForObjectId(f_id));
    
    /* task["region"] String */
    const char *s_r_id = aTaskContent["region"].getString().c_str();
    mc_object_id_t r_id = {
        s_r_id[0],
        s_r_id[1],
        s_r_id[2],
        s_r_id[3]
    };
    setRegion(MCRegionManager::sharedRegionManager()->regionForObjectId(r_id));
        
    /* task["description"] String */
    ccstring = CCString::create(aTaskContent["description"].getString());
    setDescription(ccstring);
    ccstring->retain();
}

CCObject *
MCTask::copy()
{
    MCTask *task = new MCTask;
    
    task->id_ = id_;
    task->tag_ = tag_;
    task->name_ = name_;
    task->description_ = description_;
    
    task->trapType_ = trapType_;
    task->trapWideMaxSize_ = trapWideMaxSize_;
    task->trapDamageMaxSize_ = trapDamageMaxSize_;
    task->healthPotionMaxSize_ = healthPotionMaxSize_;
    task->physicalPotionMaxSize_ = physicalPotionMaxSize_;
    task->trapWideBonus_ = trapWideBonus_;
    task->trapDamageBonus_ = trapDamageBonus_;
    task->healthPotionBonus_ = healthPotionBonus_;
    task->physicalPotionBonus_ = physicalPotionBonus_;
    task->cashPledge_ = cashPledge_;
    task->bonus_ = bonus_;
    CCObject *obj;
    CCARRAY_FOREACH(targets_, obj) {
        task->targets_->addObject(dynamic_cast<MCTaskTarget *>(obj)->copy());
    }
    task->taskStatus_ = taskStatus_;
    task->flag_ = flag_;
    task->region_ = region_;
    task->proto_ = this;
    
    return task;
}

void
MCTask::generateTaskContext()
{
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    
    if (taskContext_ == NULL) {
        taskContext_ = new MCTaskContext;
        taskContext_->healthPotion_->count = backpack->getHealthPotion()->count;
        taskContext_->physicalPotion_->count = backpack->getPhysicalPotion()->count;
        switch (trapType_) {
            case MCFireballTrap:
                taskContext_->trapWide_->item = backpack->getFireballTrapWide()->item;
                taskContext_->trapWide_->count = backpack->getFireballTrapWide()->count;
                taskContext_->trapDamage_->item = backpack->getFireballTrapDamage()->item;
                taskContext_->trapDamage_->count = backpack->getFireballTrapDamage()->count;
                break;
            case MCCurseTrap:
                taskContext_->trapWide_->item = backpack->getCurseTrapWide()->item;
                taskContext_->trapWide_->count = backpack->getCurseTrapWide()->count;
                taskContext_->trapDamage_->item = backpack->getCurseTrapDamage()->item;
                taskContext_->trapDamage_->count = backpack->getCurseTrapDamage()->count;
                break;
            case MCParalysisTrap:
                taskContext_->trapWide_->item = backpack->getParalysisTrapWide()->item;
                taskContext_->trapWide_->count = backpack->getParalysisTrapWide()->count;
                taskContext_->trapDamage_->item = backpack->getParalysisTrapDamage()->item;
                taskContext_->trapDamage_->count = backpack->getParalysisTrapDamage()->count;
                break;
            case MCFogTrap:
                taskContext_->trapWide_->item = backpack->getFogTrapWide()->item;
                taskContext_->trapWide_->count = backpack->getFogTrapWide()->count;
                taskContext_->trapDamage_->item = backpack->getFogTrapDamage()->item;
                taskContext_->trapDamage_->count = backpack->getFogTrapDamage()->count;
                break;
            case MCFlashTrap:
                taskContext_->trapWide_->item = backpack->getFlashTrapWide()->item;
                taskContext_->trapWide_->count = backpack->getFlashTrapWide()->count;
                taskContext_->trapDamage_->item = backpack->getFlashTrapDamage()->item;
                taskContext_->trapDamage_->count = backpack->getFlashTrapDamage()->count;
                break;
            default:
                CCAssert(trapType_ != MCUnknownTrap, "unknow trap");
                break;
        }
        taskContext_->healthPotion_->item = backpack->getHealthPotion()->item;
        taskContext_->healthPotion_->count = backpack->getHealthPotion()->count;
        taskContext_->physicalPotion_->item = backpack->getPhysicalPotion()->item;
        taskContext_->physicalPotion_->count = backpack->getPhysicalPotion()->count;
        /* 限制数量 */
        taskContext_->trapWide_->count += trapWideBonus_;
        taskContext_->trapDamage_->count += trapDamageBonus_;
        taskContext_->healthPotion_->count += healthPotionBonus_;
        taskContext_->physicalPotion_->count += physicalPotionBonus_;
        if (taskContext_->trapWide_->count > trapWideMaxSize_) {
            taskContext_->trapWide_->count = trapWideMaxSize_;
        }
        if (taskContext_->trapDamage_->count > trapDamageMaxSize_) {
            taskContext_->trapDamage_->count = trapDamageMaxSize_;
        }
        if (taskContext_->healthPotion_->count > healthPotionMaxSize_) {
            taskContext_->healthPotion_->count = healthPotionMaxSize_;
        }
        if (taskContext_->physicalPotion_->count > physicalPotionMaxSize_) {
            taskContext_->physicalPotion_->count = physicalPotionMaxSize_;
        }
        /* 记录任务开始时的持有数 */
        taskContext_->trapWideCount_ = taskContext_->trapWide_->count;
        taskContext_->trapDamageCount_ = taskContext_->trapDamage_->count;
        taskContext_->healthPotionCount_ = taskContext_->healthPotion_->count;
        taskContext_->physicalPotionCount_ = taskContext_->physicalPotion_->count;
        
        taskContext_->task_ = this;
        taskContext_->taskRegion_ = region_;
    }
}

MCTaskStatus
MCTask::getTaskStatus()
{
    return taskStatus_;
}

void
MCTask::setTaskStatus(MCTaskStatus var)
{
    taskStatus_ = var;
    if (var == MCTaskActiviting) {
        flag_->setState(MCOnState);
    } else if (var == MCTaskDone && proto_) {
        proto_->setTaskStatus(MCTaskDone);
    } else {
        flag_->setState(MCOffState);
    }
}
