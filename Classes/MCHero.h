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
    void attackTarget(MCRole *aTarget);
    
    /* 技能攻击 */
    void attackTargetWithSkill(MCRole *aTarget, MCSkill *aSkill);
    
    void roleDidApproachTarget(CCObject *anObject);
    
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
    
    CCObject *copy() { return copyWithZone(0); }
private:
    bool atEntrance_;
};

#endif /* defined(__Military_Confrontation__MCHero__) */
