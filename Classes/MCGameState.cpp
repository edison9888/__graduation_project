//
//  MCGameState.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCGameState.h"

#include "MCBackpack.h"
#include "MCFlagManager.h"
#include "MCTaskManager.h"
#include "MCSkillManager.h"
#include "MCDungeonMaster.h"

static MCGameState *__shared_game_state = NULL;

MCGameState::MCGameState()
{
    
}

MCGameState::~MCGameState()
{
    
}

MCGameState *
MCGameState::sharedGameState()
{
    if (__shared_game_state == NULL) {
        __shared_game_state = new MCGameState;
    }
    
    return __shared_game_state;
}

/**
 * 保存
 * 
 * 保存的东西列表
 * 装备信息
 * 背包(道具)信息
 * 佣兵信息 所雇佣的佣兵和佣兵的状态
 * 标志信息
 * 任务信息
 * 技能信息
 * 重生点 重生点所在场景的ID
 */
void
MCGameState::save()
{
#warning 装备信息
    MCBackpack::sharedBackpack()->saveData();
#warning 佣兵信息 所雇佣的佣兵和佣兵的状态
    MCFlagManager::sharedFlagManager()->saveAllFlags();
    MCTaskManager::sharedTaskManager()->saveData();
    MCSkillManager::sharedSkillManager()->saveData();
    MCDungeonMaster::sharedDungeonMaster()->saveSpawnPoint();
    
    /* 写入磁盘 */
    CCUserDefault::sharedUserDefault()->flush();
}

/**
 * 读取
 */
void
MCGameState::load()
{
#warning 装备信息
    MCBackpack::sharedBackpack();
#warning 佣兵信息 所雇佣的佣兵和佣兵的状态
    MCFlagManager::sharedFlagManager();
    MCTaskManager::sharedTaskManager();
    MCSkillManager::sharedSkillManager();
    MCDungeonMaster::sharedDungeonMaster();
}
