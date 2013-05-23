//
//  MCGameState.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCGameState.h"

#include "MCBackpack.h"
#include "MCEffectManager.h"
#include "MCEquipmentManager.h"
#include "MCFlagManager.h"
#include "MCTaskManager.h"
#include "MCSkillManager.h"
#include "MCDungeonMaster.h"
#include "MCMercenaryManager.h"

#if MC_DEBUG_SAVEDATA == 1
const char *kMCSaveFileExistsKey = "save-file-exists";
#else
const char *kMCSaveFileExistsKey = "c2F2ZS1maWxlLWV4aXN0cw"; /* save-file-exists的BASE64编码没有最后的== */
#endif

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

bool
MCGameState::isSaveFileExists()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    
    return userDefault->getBoolForKey(kMCSaveFileExistsKey, false);
}

/**
 * 清除所有记录
 */
void
MCGameState::erase()
{
    CCUserDefault::sharedUserDefault()->setBoolForKey(kMCSaveFileExistsKey, false);
    load();
}

/**
 * 保存
 *
 * 保存的东西列表
 * 背包(装备)信息
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
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    
    userDefault->setBoolForKey(kMCSaveFileExistsKey, true);
    MCBackpack::sharedBackpack()->saveData();
    MCEquipmentManager::sharedEquipmentManager()->saveData();
    MCMercenaryManager::sharedMercenaryManager()->saveData();
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
    MCBackpack::sharedBackpack()->loadData();
    MCEquipmentManager::sharedEquipmentManager()->loadData();
    MCMercenaryManager::sharedMercenaryManager()->loadData();
    MCFlagManager::sharedFlagManager()->loadAllFlags();
    MCFlagManager::sharedFlagManager()->spawn(); /* 加载游戏的时候直接出现在重生点 */
    MCTaskManager::sharedTaskManager()->loadData();
    MCSkillManager::sharedSkillManager()->loadData();
    MCDungeonMaster::sharedDungeonMaster()->loadSpawnPoint();
}
