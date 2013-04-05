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

typedef mc_ushort_t mc_offensive_t;

enum {
    MCNormalBattle,
    MCMutiplayerBattle
};
typedef unsigned int MCBattleType;

class MCBattleProtoacol;

class MCBattle : public CCObject {
private:
    MCBattle() { };
    
    bool _init();
    
public:
    static MCBattle *sharedBattle();
    
    MCBattleProtoacol *battleWithType(MCBattleType type);
};

class MCBattleProtoacol : public CCObject {
public:
};

class MCOffensiveProtoacol : public CCObject {
public:
    /**
     * 重击判断
     * 投掷D20的骰子，命中范围后，再投掷第二次依然命中，则重击
     */
    virtual bool acttackCheckCriticalHit(MCDiceRange aRange) = 0;
    
    /**
     * 己方攻击判定
     */
    virtual mc_offensive_t getOffensive() = 0;
    
    /**
     * 对方防御等级
     */
    virtual mc_ac_t getAC() = 0;
    
    /**
     * 命中判断	
     * 命中=己方攻击判定>=防御等级
     * 己方攻击判定= D20+武器敏捷调整值
     * 对方防御等级=10+最大防御加值+敏捷调整值"
     */
    virtual bool attackCheckHit(mc_offensive_t anOffensive, mc_ac_t anAC) = 0;
    
    /**
     * 攻击伤害
     * 武器伤害值
     */
    virtual mc_damage_t getOffensiveDamage() = 0;
    
    /**
     * 伤害判定	
     * 无论结果为神马，最小值为1	
     * D10+攻击伤害+防具检定减值
     */
    virtual mc_damage_t attackGetDamage(mc_damage_t anOffensiveDamage, mc_armor_check_penalty_t anArmorCheckPenalty);
};

#endif /* defined(__Military_Confrontation__MCBattle__) */
