//
//  MCEnemy.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEnemy__
#define __Military_Confrontation__MCEnemy__

#include "MCNPC.h"
#include "MCEquipmentProperty.h"
#include "MCDice.h"

/* 基础怪物 */
class MCEnemy : public MCNPC {
    friend class MCRoleManager;
public:
    ~MCEnemy();
    
    bool init(MCRoleRace aRoleRace);
    
    static MCEnemy *create(mc_object_id_t anObjectId);
    
    MCRoleEntity *getEntity();
    
    /* MCAIStateMachineDelegate */
    /**
     * 空闲状态下回调
     */
    void performWhenIdleState();
    
    /**
     * 战斗状态下回调
     */
    void performWhenCombatantStatus() { MCRole::performWhenCombatantStatus(); }
    
    /**
     * 休息状态下回调
     */
    void performWhenRestingState() { MCRole::performWhenRestingState(); }
    
    /**
     * 攻击状态下回调
     */
    void performWhenAttackState() { MCRole::performWhenAttackState(); }
    
    /* 巡逻 */
    void round();
    
    /* MCOffensiveProtocol */
    /**
     * 重击范围
     */
    MCDiceRange getCriticalHitRange(bool inVision=true);
    
    /**
     * 重击倍数
     */
    mc_critical_hit_t getCriticalHit();
    
    /**
     * 己方攻击判定
     */
    mc_offensive_t getOffensive();
    
    /**
     * 己方防御等级
     */
    mc_ac_t getAC();
    
    /**
     * 攻击伤害
     * 武器伤害值
     */
    mc_damage_t getOffensiveDamage();
    
    /**
     * 防具检定减值
     */
    mc_armor_check_penalty_t getArmorCheckPenalty();
    
    /**
     * 攻击附带效果
     */
    MCRoleState attackWithState();
    
    /**
     * 普通攻击效果
     */
    MCEffect *effectForNormalAttack();
    
    /**
     * 技能攻击评分
     * 伤害值——最大伤害值*伤害调整*技能次数
     * 对方状态——若有异常状态则4分，若没则0分。
     * 攻击范围——(obb.width*obb.height)*m(m值待定)
     */
    mc_score_t getSkillDamageScore(MCSkill *aSkill);
    
    /**
     * 攻击音效
     */
    inline const char *actionEffect() {
        return actionEffect_->getCString();
    }
    
    /* 动作 */
    /* 能否发动攻击 */
    bool canAttackTarget(MCRole *aRole);
    
    /* 普通攻击 */
    void attackTarget(MCRole *aTargetRole, CCObject *aTarget=NULL, SEL_CallFuncO aSelector=NULL, CCObject *anUserObject=NULL);

    /**
     * 死亡
     */
    void died();
    
    CCObject *copy();
    
protected:
    void roundDidFinish(CCObject *anObject);
    
    CC_SYNTHESIZE(mc_pp_t, consume_, Consume); /* 攻击体力消耗 */
    CC_SYNTHESIZE(mc_dexterity_t, dexterity_, Dexterity); /* 敏捷调整值 */
    CC_SYNTHESIZE(mc_ac_t, ac_, AC); /* 防御等级 */
    CC_SYNTHESIZE(mc_armor_check_penalty_t, armorCheckPenalty_, ArmorCheckPenalty); /* 防具检定减值 */
    CC_SYNTHESIZE(MCDice *, damage_, Damage); /* 伤害值 */
    CC_SYNTHESIZE(mc_bonus_t, damageBonus_, DamageBonus); /* 伤害值加成 */
    CC_SYNTHESIZE(MCDiceRange, criticalHitVisible_, CriticalHitVisible); /* 可视区域重击范围 */
    CC_SYNTHESIZE(MCDiceRange, criticalHitInvisible_, CriticalHitInvisible); /* 非可视区域重击范围 */
    CC_SYNTHESIZE(mc_critical_hit_t, criticalHit_, CriticalHit); /* 重击倍数 */
    CC_SYNTHESIZE(mc_distance_t, distance_, Distance); /* 攻击距离 */
    CC_SYNTHESIZE(CCArray *, skills_, Skills); /* 技能，最多4个 */
    CC_SYNTHESIZE(MCRoleState, effect_, Effect); /* 附带效果 */
    CC_SYNTHESIZE(MCDiceRange, effectCheck_, EffectCheck); /* 附带效果判定 */
    
    CC_SYNTHESIZE(MCEffect *, attackEffect_, AttackEffect); /* 攻击效果 */
    
    CC_SYNTHESIZE(CCString *, actionEffect_, ActionEffect); /* 攻击效果音 */
    
    CCPoint center_; /* 活动中心点 */
    
    mc_score_t damageScore_; /* 普通攻击评分 */
};

#endif /* defined(__Military_Confrontation__MCEnemy__) */
