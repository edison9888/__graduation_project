//
//  MCBattle.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBattle__
#define __Military_Confrontation__MCBattle__

#include <cocos2d.h>
USING_NS_CC;

#include "MCType.h"
#include "MCEquipment.h"
#include "MCEffectiveItem.h"

/* namespace MCBattle {} */
#ifdef __cplusplus
#define NS_MC_BATTLE_BEGIN                     namespace MCBattle {
#define NS_MC_BATTLE_END                       }
#define USING_NS_MC_BATTLE                     using namespace MCBattle
#else
#define NS_MC_BATTLE_BEGIN
#define NS_MC_BATTLE_END
#define USING_NS_MC_BATTLE
#endif

NS_MC_BATTLE_BEGIN

typedef struct {
    mc_hp_change_value_t hp;             /* HP变化值 */
    /* 实际上并无减少PP的效果 */
//    mc_pp_change_value_t pp;             /* PP变化值 */
    MCRoleState          positive_state; /* 会增加的状态 */
    /* 实际上并无会消除效果的物品 */
//    MCRoleState          negative_state; /* 会消除的状态 */
    mc_lasting_time_t    remaining_time; /* 剩余效果时间 */
} mc_effect_t;

typedef mc_short_t mc_offensive_t;

enum {
    MCNormalBattle,
    MCMutiplayerBattle
};
typedef unsigned int MCBattleType;

class MCOffensiveProtocol {
public:
    /**
     * 重击判断
     * 投掷D20的骰子，命中范围后，再投掷第二次依然命中，则重击
     */
    virtual bool attackCheckCriticalHit(MCDiceRange aRange) { return false; }
    
    /**
     * 重击范围
     */
    virtual MCDiceRange getCriticalHitRange(bool inVision=true) { return MCDiceRange(); }
    
    /**
     * 重击倍数
     */
    virtual mc_critical_hit_t getCriticalHit() { return 0.0f; }
    
    /**
     * 己方攻击判定
     */
    virtual mc_offensive_t getOffensive() { return 0; }
    
    /**
     * 己方防御等级
     */
    virtual mc_ac_t getAC() { return 0; }
    
    /**
     * 命中判断
     * 命中=己方攻击判定>=防御等级
     * 己方攻击判定= D20+武器敏捷调整值
     * 对方防御等级=10+最大防御加值+敏捷调整值"
     */
    virtual bool attackCheckHit(mc_offensive_t anOffensive, mc_ac_t anAC) { return false; }
    
    /**
     * 攻击伤害
     * 武器伤害值
     */
    virtual mc_damage_t getOffensiveDamage() { return 0; }
    
    /**
     * 防具检定减值
     */
    virtual mc_armor_check_penalty_t getArmorCheckPenalty() { return 0; }
    
    /**
     * 攻击附带效果
     */
    virtual MCRoleState attackWithState() { return MCNormalState; }
    
    /**
     * 普通攻击效果
     */
    virtual MCEffect *effectForNormalAttack() { return NULL; }
    
    /**
     * 技能攻击效果
     */
    virtual MCEffect *effectForSkillAttack() { return NULL; }
    
    /**
     * 伤害判定
     * 无论结果为神马，最小值为1
     * D10+攻击伤害+防具检定减值
     */
    virtual mc_damage_t attackGetDamage(mc_damage_t anOffensiveDamage, mc_armor_check_penalty_t anArmorCheckPenalty) = 0;
    
    /**
     * 获取人物的所有技能
     */
    virtual CCArray *getSkills() const { return NULL; }

    /**
     * 技能攻击评分
     * 伤害值——最大伤害值*伤害调整*技能次数
     * 对方状态——若有异常状态则4分，若没则0分。
     * 攻击范围——(obb.width*obb.height)*m(m值待定)
     */
    virtual mc_score_t getSkillDamageScore(MCSkill *aSkill) { return 0; }
};

NS_MC_BATTLE_END

#endif /* defined(__Military_Confrontation__MCBattle__) */
