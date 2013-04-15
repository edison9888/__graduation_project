//
//  MCSkillManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSkillManager.h"
#include "MCGameState.h"

static MCSkillManager *__shared_skill_manager = NULL;

MCSkillManager::MCSkillManager()
{
    skills_ = CCDictionary::create();
    skills_->release();
}

MCSkillManager::~MCSkillManager()
{
    CC_SAFE_RELEASE(skills_);
}

MCSkillManager *
MCSkillManager::sharedSkillManager()
{
    if (__shared_skill_manager == NULL) {
        __shared_skill_manager = new MCSkillManager;
        __shared_skill_manager->loadData();
    }
    
    return __shared_skill_manager;
}

void
MCSkillManager::saveData()
{
    
}

void
MCSkillManager::loadData()
{
    
}
