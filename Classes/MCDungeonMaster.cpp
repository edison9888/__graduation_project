//
//  MCDungeonMaster.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCDungeonMaster.h"
#include "MCBase64.h"
#include "MCGameState.h"
#include "MCScene.h"
#include "MCEffectManager.h"

const char *kMCSpawnPointKey = "c3Bhd24tcG9pbnQ"; /* spawn-point的BASE64编码没有最后的= */
const char *kMCSpawnPointDefaultValue = "TTAwMQ=="; /* M001的BASE64编码没有最后的== */

static const mc_object_id_t kMCDefaultSpawnPointSceneId = {'M', '0', '0', '1'};

static MCDungeonMaster *__shared_dungeon_master = NULL;

MCDungeonMaster *
MCDungeonMaster::sharedDungeonMaster()
{
    if (__shared_dungeon_master == NULL) {
        __shared_dungeon_master = new MCDungeonMaster;
    }
    
    return __shared_dungeon_master;
}

void
MCDungeonMaster::speak(const char *aMessage)
{
    MCDialog *dmDialog = MCDialog::sharedDialog(MCDMDialog);
    dmDialog->setMessage(aMessage);
    dmDialog->setDismissSelector(this,
                                 dismiss_selector(MCDungeonMaster::dismiss),
                                 NULL);
    dmDialog->attach(CCDirector::sharedDirector()->getRunningScene());
}

/**
 * aRole将要攻击aTarget
 *
 * 攻击评分=攻击伤害预测值(即为攻击的最大值)+采取攻击之后的体力剩余值。（若采取攻击之后的体力剩余值为负，则忽略之）
 *
 * 攻击判定方式：
 * 若有范围攻击方式，则收集范围类攻击的评分。
 * 然后跟普通攻击评分对比。
 * 若更佳则采用范围攻击，否则采用单体。
 *
 * 攻击方式判定：
 * 自身体力值——收集所有攻击手段，若采取之，体力会在什么状态。若气绝则1分，若不则4分。
 * 对方状态——若有异常状态则4分，若没则1分。
 */
void
MCDungeonMaster::roleWillAttack(MCRole *aRole, MCRole *aTarget)
{
    /* 攻击方式判定 */
    /* 通常攻击评分 */
    /* 技能攻击评分 */
}

/**
 * aRole攻击aTarget
 */
void
MCDungeonMaster::roleAttackTarget(MCRole *aRole, MCRole *aTarget)
{
    /* 攻击效果 */
    MCScene *scene = MCSceneContextManager::sharedSceneContextManager()->currentContext()->getScene();
    aRole->effectForNormalAttack()->attach(scene, aTarget);
    
    /* 命中判定 */
    bool hit = aRole->attackCheckHit(aRole->getOffensive(), aTarget->getAC());
    
    if (hit) {
        /* 重击判定 */
        /* 视野范围内？ */
        MCDiceRange diceRange = aRole->getCriticalHitRange(aTarget->roleInVision(aRole));
        bool isCriticalHit = aRole->attackCheckCriticalHit(diceRange);
        
        /* 决定伤害值 */
        mc_damage_t damage = aRole->attackGetDamage(aRole->getOffensiveDamage(), aTarget->getArmorCheckPenalty());
        if (isCriticalHit) {
            damage *= aRole->getCriticalHit();
        }
#if MC_BATTLE_INFO_LEVEL == 1
        printf("伤害值: %hi\n", damage);
#endif
        
        mc_effect_t effect = {
            -damage,
            aRole->attackWithState(), /* 某些有特殊效果的普通攻击 */
            0.0f
        };
        aTarget->roleWasAttacked(effect);
    } else {
        MCEffectManager::sharedEffectManager()->missEffect()->attach(scene, aTarget);
    }
}

void
MCDungeonMaster::destroyGameWorld()
{
    MCGameState::sharedGameState()->erase();
}

void
MCDungeonMaster::saveSpawnPoint()
{
    char c_str_sp_id[5] = {0};
    c_str_sp_id[0] = spawnPointID_.class_;
    c_str_sp_id[1] = spawnPointID_.sub_class_;
    c_str_sp_id[2] = spawnPointID_.index_;
    c_str_sp_id[3] = spawnPointID_.sub_index_;
    const char *input = c_str_sp_id;
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
    CCUserDefault::sharedUserDefault()->setStringForKey(kMCSpawnPointKey, output);
    delete []output;
}

void
MCDungeonMaster::loadSpawnPoint()
{
    std::string data = CCUserDefault::sharedUserDefault()->getStringForKey(kMCSpawnPointKey, kMCSpawnPointDefaultValue);
    if (MCGameState::sharedGameState()->isSaveFileExists() && data.size() > 0) {
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
        data.assign(output);
        const char *c_str_o_id = data.c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        spawnPointID_ = o_id;
    } else {
        spawnPointID_ = kMCDefaultSpawnPointSceneId;
    }
}

void
MCDungeonMaster::dismiss(void *anUserdata)
{
}
