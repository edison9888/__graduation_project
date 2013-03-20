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

MCTask::MCTask()
{
    bonus_ = CCArray::create();
    bonus_->retain();
}

MCTask::~MCTask()
{
    CC_SAFE_RELEASE(bonus_);
}

void
MCTask::loadTaskContent(JsonBox::Object &aTaskContent)
{
    
    /* task["name"] String */
    setName(CCString::create(aTaskContent["name"].getString()));
    
    /* task["description"] String */
    setDescription(CCString::create(aTaskContent["description"].getString()));
    
    /* flag["description"] String */
    const char *s_f_id = aTaskContent["flag"].getString().c_str();
    mc_object_id_t f_id = {
        s_f_id[0],
        s_f_id[1],
        s_f_id[2],
        s_f_id[3]
    };
    setFlag(MCFlagManager::sharedFlagManager()->flagForObjectId(f_id));
    
    /* task["bonus"] Object */
    JsonBox::Object objects = aTaskContent["bonus"].getObject();
    for (JsonBox::Object::iterator iterator = objects.begin(); iterator != objects.end(); ++iterator) {
        const char *s_b_id = iterator->first.c_str();
        mc_object_id_t b_id = {
            s_b_id[0],
            s_b_id[1],
            s_b_id[2],
            s_b_id[3]
        };
        MCTaskBonus *bonus = new MCTaskBonus;
        bonus->autorelease();
        bonus->objectID = b_id;
        bonus->count = iterator->second.getInt();
        bonus_->addObject(bonus);
    }
    
    /* task["context"] Object */objects = aTaskContent["context"].getObject();
    for (JsonBox::Object::iterator iterator = objects.begin(); iterator != objects.end(); ++iterator) {
        const char *s_b_id = iterator->first.c_str();
        mc_object_id_t b_id = {
            s_b_id[0],
            s_b_id[1],
            s_b_id[2],
            s_b_id[3]
        };
        MCTaskTarget *target = new MCTaskTarget;
        target->autorelease();
        target->objectID = b_id;
        target->count = iterator->second.getInt();
        bonus_->addObject(target);
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
    } else {
        flag_->setState(MCOffState);
    }
}
