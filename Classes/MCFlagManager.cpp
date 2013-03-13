//
//  MCFlagManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-10.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "JsonBox.h"
#include "MCFlagManager.h"

static MCFlagManager *__shared_flag_manager = NULL;

const char *kMCFlagsFilepath = "flags.fpkg";

MCFlagManager::MCFlagManager()
{
    sourceFlags_ = CCDictionary::create();
    sourceFlags_->retain();
    flags_ = CCDictionary::create();
    flags_->retain();
}

MCFlagManager::~MCFlagManager()
{
    
}

MCFlagManager *
MCFlagManager::sharedFlagManager()
{
    if (__shared_flag_manager == NULL) {
        __shared_flag_manager = new MCFlagManager;
        __shared_flag_manager->loadAllFlags();
    }
    
    return __shared_flag_manager;
}

MCFlag *
MCFlagManager::flagForObjectId(mc_object_id_t anObjectId)
{
    int key = MCObjectIdToDickKey(anObjectId);
    MCFlag *flag;
    
    flag = (MCFlag *) flags_->objectForKey(key);
    if (flag == NULL) {
        flag = (MCFlag *) sourceFlags_->objectForKey(key);
    }
    
    return flag;
}

MCFlagState
MCFlagManager::flagStateForObjectId(mc_object_id_t anObjectId)
{
    MCFlag *flag = flagForObjectId(anObjectId);
    
    if (flag) {
        return flag->getState();
    }
    
    return MCInvalidState;
}

void
MCFlagManager::updateFlag(MCFlag *aFlag)
{
    flags_->setObject(aFlag, MCObjectIdToDickKey(aFlag->getID()));
}

/**
 * load all flags
 */
void
MCFlagManager::loadAllFlags()
{
    JsonBox::Value v;
    JsonBox::Object root;
    JsonBox::Object::iterator flagsIterator;
    JsonBox::Object flagValueRoot;
    const char *c_sitr_flag_d;
    mc_object_id_t flagId;
    MCFlag *flag;
    
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(kMCFlagsFilepath));
    root = v.getObject();
    
    for (flagsIterator = root.begin(); flagsIterator != root.end(); ++flagsIterator) {
        c_sitr_flag_d = flagsIterator->first.c_str();
        flagValueRoot = flagsIterator->second.getObject();
        flagId.class_ = c_sitr_flag_d[0];
        flagId.sub_class_ = c_sitr_flag_d[1];
        flagId.index_ = c_sitr_flag_d[2];
        flagId.sub_index_ = c_sitr_flag_d[3];
        
        flag = MCFlag::create(flagId, flagValueRoot);
        sourceFlags_->setObject(flag, MCObjectIdToDickKey(flagId));
    }
    
#warning load flags from CCUserDefault
}

/**
 * save all flags
 */
void
MCFlagManager::saveAllFlags()
{
#warning save to CCUserDefault
}
