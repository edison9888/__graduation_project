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
#include "MCSkillManager.h"
#include "MCSimpleAudioEngine.h"

#if MC_DEBUG_SAVEDATA == 1
const char *kMCSpawnPointKey = "spawn-point";
const char *kMCSpawnPointDefaultValue = "M001";
#else
const char *kMCSpawnPointKey = "c3Bhd24tcG9pbnQ"; /* spawn-point的BASE64编码没有最后的= */
const char *kMCSpawnPointDefaultValue = "TTAwMQ=="; /* M001的BASE64编码没有最后的== */
#endif

static const char *kMCIntroduction[] = {
    "欢迎来到我的游戏。",
    "游戏玩法很简单，去任务屋接受任务然后到野外杀怪。没有接受任务的时候，野外是没有怪的。",
    "任务屋里有佣兵雇佣处。",
    "物品交易与装备升级也在一个屋子里。至于位置，自己找吧，就这么几间屋子~~~~",
    NULL
};

static const char *kMCMissEffect = "voices/miss.wav";

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
    dmDialog->attach(CCDirector::sharedDirector()->getRunningScene());
}

/* 刚开始游戏时游戏介绍 */
void
MCDungeonMaster::introduce()
{
    introduce(kMCIntroduction);
}

/**
 * aRole将要攻击aTarget
 * 由AI调用
 *
 * 筛选掉释放后会体力透支的技能攻击方式，然后在剩余的技能中选择评分最高的技能。
 * 若没留下，则普通攻击。
 *
 */
void
MCDungeonMaster::roleWillAttack(MCRole *aRole, MCRole *aTargetRole, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserObject)
{
    MCSkill *skill;
    MCSkill *maxScoreSkill = NULL;
    mc_score_t skillScore;
    mc_score_t maxSkillScore = 0;
    CCArray *skills = aRole->getSkills();
    CCObject *obj;
    bool isNotHero = !aRole->isHero();
    
    /* 技能攻击评分 */
    if (isNotHero) {
        CCARRAY_FOREACH(skills, obj) {
            skill = dynamic_cast<MCSkill *>(obj);
            if (! skill->willBeExhausted(aRole)) { /* 必须使用后不体力透支才参与判定 */
                skillScore = aRole->getSkillDamageScore(skill);
                if (maxScoreSkill == NULL) {
                    maxScoreSkill = skill;
                    maxSkillScore = skillScore;
                } else if (skillScore > maxSkillScore) {
                    maxScoreSkill = skill;
                    maxSkillScore = skillScore;
                }
            }
        }
    }
    
    if (isNotHero && maxScoreSkill != NULL && maxScoreSkill->canRoleUse(aRole)) { /* 技能攻击 */
        aRole->attackTargetWithSkill(aTargetRole, dynamic_cast<MCSkill *>(maxScoreSkill->copy()),
                                     aTarget, aSelector, anUserObject);
    } else { /* 尝试进行普通攻击，可能会体力不足 */
        aRole->attackTarget(aTargetRole,
                            aTarget, aSelector, anUserObject);
    }
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
        mc_damage_t damage = aRole->attackGetDamage(aRole->getOffensiveDamage(),
                                                    aTarget->getArmorCheckPenalty());
        if (isCriticalHit) {
            damage *= aRole->getCriticalHit();
        }
#if MC_BATTLE_INFO_LEVEL == 1
        printf("伤害值: %hi\n", damage);
#endif
        
        MCRoleState attackWithState = aRole->attackWithState(); /* 某些有特殊效果的普通攻击 */
        mc_effect_t effect = {
            -damage,
            attackWithState,
            attackWithState == MCNormalBattle ? 0.0f : 3.0f /* 送3秒钟效果 */
        };
        aTarget->roleWasAttacked(effect);
        
        if (aRole->hasState(MCCurseState)) { /* 被诅咒了，移动减少的体力值增加(double)，发动攻击会扣血(一半) */
            mc_effect_t effect = {
                -damage / 2,
                MCNormalBattle,
                0.0f
            };
            aRole->roleWasAttacked(effect);
        }
        
        /* 声音效果 */
        const char *actionEffect = aRole->actionEffect();
        if (actionEffect) {
            MCSimpleAudioEngine::sharedSimpleAudioEngine()->playEffect(actionEffect);
        }
    } else {
        MCEffectManager::sharedEffectManager()->missEffect()->attach(scene, aTarget);
        /* 声音效果 */
        MCSimpleAudioEngine::sharedSimpleAudioEngine()->playEffect(kMCMissEffect);
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
#if MC_DEBUG_SAVEDATA == 1
    const char *output = c_str_sp_id;
#else
    const char *input = c_str_sp_id;
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
    CCUserDefault::sharedUserDefault()->setStringForKey(kMCSpawnPointKey, output);
#if MC_DEBUG_SAVEDATA != 1
    delete []output;
#endif
}

void
MCDungeonMaster::loadSpawnPoint()
{
    std::string data = CCUserDefault::sharedUserDefault()->getStringForKey(kMCSpawnPointKey, kMCSpawnPointDefaultValue);
    if (MCGameState::sharedGameState()->isSaveFileExists() && data.size() > 0) {
#if MC_DEBUG_SAVEDATA == 1
        const char *output = data.c_str();
#else
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
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
MCDungeonMaster::introduce(const char **sentences)
{
    MCDialog *dmDialog = MCDialog::sharedDialog(MCDMDialog);
    dmDialog->setMessage(*sentences++);
    dmDialog->setDismissSelector(this,
                                 dismiss_selector(MCDungeonMaster::dismiss),
                                 sentences);
    dmDialog->attach(CCDirector::sharedDirector()->getRunningScene());
}

void
MCDungeonMaster::dismiss(void *anUserdata)
{
    const char **sentences = (const char **) anUserdata;
    
    if (*sentences != NULL) {
        introduce(sentences);
    }
}
