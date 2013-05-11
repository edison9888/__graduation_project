//
//  MCHero.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCHero__
#define __Military_Confrontation__MCHero__

#include "MCRole.h"

/* 主角 */
class MCHero : public MCRole {
public:
    ~MCHero();
    bool init();
    
    static MCHero *sharedHero();
    
    MCRoleEntity *getEntity();
    
    inline bool atEntrance() {
        return atEntrance_;
    }
    
    inline void setAtEntrance(bool var) {
        atEntrance_ = var;
    }
    
    /**
     * 在主角面前的人物
     * 判断当前视野中的人物，然后返回最近的那个
     */
    MCRole *roleOfFront();
    
    /* 动作 */
    /* 普通攻击 */
    void attackTarget(MCRole *aTargetRole, CCObject *aTarget=NULL, SEL_CallFuncO aSelector=NULL, CCObject *anUserObject=NULL);
    
    /**
     * 主角挂了
     */
    void died();
    
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
     * 获取人物的所有技能
     */
    CCArray *getSkills() const;
    
    /**
     * 技能攻击评分
     * 伤害值——最大伤害值*伤害调整*技能次数
     * 对方状态——若有异常状态则4分，若没则0分。
     * 攻击范围——(obb.width*obb.height)*m(m值待定)
     */
    mc_score_t getSkillDamageScore(MCSkill *aSkill);
    
    inline bool isHero() {
        return true;
    }
    
    CCObject *copy() { return copyWithZone(0); }
private:
    bool atEntrance_;
};

#endif /* defined(__Military_Confrontation__MCHero__) */
