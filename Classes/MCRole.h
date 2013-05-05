//
//  MCRole.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRole__
#define __Military_Confrontation__MCRole__

#include "MCObject.h"
#include "MCRoleEntity.h"
#include "MCRoleProperty.h"
#include "MCAI.h"
#include "MCBattle.h"

extern const char *kMCRoleDiedNotification;

USING_NS_CC;
USING_NS_MC_BATTLE;

class MCScript;
class MCSkill;

/* 基础角色 */
class MCRole : public MCObject, public MCAIDelegate, public MCAIStateMachineDelegate, public MCOffensiveProtocol {
    friend class MCRoleEntity;
    friend class MCAI;
public:
    /**
     * 角色类型
     */
    enum {
        MCUnknownRole   = 0,             /* 未知类型的人物 */
        MCHero          = MCMakeEnum(0), /* 主角 */
        MCNPC           = MCMakeEnum(1), /* NPC */
        MCEnemy         = MCMakeEnum(2), /* 敌人 */
        MCMercenary     = MCMakeEnum(3)  /* 佣兵 */
    };
    typedef mc_enum_t MCRoleType;
    
    MCRole();
    ~MCRole();
    
    bool init();
    
    void loadSpriteSheet();
    void loadSpriteSheet(const char *aSpritesheetPath);
    
    /**
     * aRole在视野中
     */
    bool roleInVision(MCRole *aRole);
    
    /* MCAIDelegate */
    /**
     * 某人进入视野
     * 默认看到的都是敌人
     */
    void roleDidEnterVision(MCRole *aRole, bool isEnermy = true);
    
    /**
     * 某人离开视野
     * 默认离开的都是敌人
     */
    void roleDidExitVision(MCRole *aRole, bool isEnermy = true);
    
    /**
     * 被攻击
     */
    void roleWasAttacked(const mc_effect_t &anEffect);
    
    /**
     * 攻击结束
     */
    void attackDidFinish();
    
    /**
     * 状态切换
     */
    void roleDidChangeStateTo(MCAIState anAIState);
    
    /* MCAIStateMachineDelegate */
    /**
     * 空闲状态下回调
     */
    void performWhenIdleState();
    
    /**
     * 战斗状态下回调
     */
    void performWhenCombatantStatus();
    
    /**
     * 休息状态下回调
     */
    void performWhenRestingState();
    
    /**
     * 攻击状态下回调
     */
    void performWhenAttackState();
    
    /**
     * 死亡状态下回调
     */
    void performWhenDeathState();
    
    /* 动作 */
    /* 普通攻击 */
    virtual void attackTarget(MCRole *aTarget)  {
        ai_->activate();
    }
    
    /* 技能攻击 */
    virtual void attackTargetWithSkill(MCRole *aTarget, MCSkill *aSkill) {
        ai_->activate();
    }
    
    virtual void roleDidApproachTarget(CCObject *anObject) {}
        
    /* MCOffensiveProtocol */
    /**
     * 重击判断
     * 投掷D20的骰子，命中范围后，再投掷第二次依然命中，则重击
     */
    bool attackCheckCriticalHit(MCDiceRange aRange);
    
    /**
     * 命中判断
     * 命中=己方攻击判定>=防御等级
     * 己方攻击判定= D20+武器敏捷调整值
     * 对方防御等级=10+最大防御加值+敏捷调整值"
     */
    bool attackCheckHit(mc_offensive_t anOffensive, mc_ac_t anAC) {
#if MC_BATTLE_INFO_LEVEL == 1
        printf("攻击对抗：%hu %s %hu => %s\n",
               anOffensive,
               anOffensive >= anAC ? ">=" : "<",
               anAC,
               anOffensive >= anAC ? "命中！" : "未命中！");
#endif
        return anOffensive >= anAC;
    }
    
    /**
     * 伤害判定
     * 无论结果为神马，最小值为1
     * D10+攻击伤害+防具检定减值
     */
    mc_damage_t attackGetDamage(mc_damage_t anOffensiveDamage, mc_armor_check_penalty_t anArmorCheckPenalty);
    
    /**
     * 死亡
     */
    virtual void died();
    
    virtual CCObject *copy() = 0;
    
    inline bool isEnemy(MCRole *aRole) {
        if ((roleType_ == MCRole::MCEnemy && aRole->roleType_ != MCRole::MCEnemy)
            || (roleType_ != MCRole::MCEnemy && aRole->roleType_ == MCRole::MCEnemy)) {
            return true;
        }
        
        return false;
    }
    
    inline mc_hp_t updateHP(mc_hp_t var) {
        hp_ += var;
        if (hp_ > maxHP_) {
            hp_ = maxHP_;
        } else if (hp_ < 0) {
            hp_ = 0;
        }
        
        return hp_;
    }

    inline mc_pp_t updatePP(mc_pp_t var) {
        pp_ += var;
        if (pp_ > maxPP_) {
            pp_ = maxPP_;
        } else if (pp_ < 0) {
            pp_ = 0;
        }
        
        return pp_;
    }
    
    inline bool isExhausted() {
        return exhausted_;
    }
    
protected:
    std::vector<mc_effect_t> effects_;
    
    /* 角色属性 */
    CC_SYNTHESIZE(MCRoleType, roleType_, RoleType); /* 角色类型 */
    CC_SYNTHESIZE(MCRoleRace, roleRace_, RoleRace); /* 角色种族 */
    CC_SYNTHESIZE(mc_hp_t, hp_, HP); /* 角色生命值 */
    CC_SYNTHESIZE(mc_pp_t, pp_, PP); /* 角色体力值 */
    bool exhausted_; /* 体力透支导致快挂了 */
    CC_SYNTHESIZE(mc_pp_t, exhaustion_, Exhaustion); /* 体力透支线 */
    CC_SYNTHESIZE(mc_pp_t, tired_, Tired); /* 体力疲惫线 */
    CC_SYNTHESIZE(mc_hp_t, maxHP_, MaxHP); /* 角色满生命值 */
    CC_SYNTHESIZE(mc_pp_t, maxPP_, MaxPP); /* 角色满体力值 */
    CC_SYNTHESIZE(MCRoleState, roleState_, RoleState); /* 角色状态 */
    CC_SYNTHESIZE(CCString *, face_, Face); /* 角色头像 */
    CC_SYNTHESIZE(CCString *, spriteSheet_, SpriteSheet); /* 角色精灵表 */
    
    CC_SYNTHESIZE(CCString *, defaultDialogue_, DefaultDialogue); /* 默认对白 */
    
    /* 显示相关 */
    CC_PROPERTY_READONLY(MCRoleEntity *, entity_, Entity); /* entity_将共用metadata_ */
    
    CC_SYNTHESIZE_READONLY(MCRoleEntityMetadata *, entityMetadata_, EntityMetadata); /* entity_将共用这个metadata */
    
    /* AI */
    CC_SYNTHESIZE_READONLY(MCAI *, ai_, AI);
    
    /* 点击活动键触发 */
    CC_SYNTHESIZE(MCScript *, trigger_, Trigger);
    
    CC_SYNTHESIZE_RETAIN(CCObject *, userdata_, UserData);
};

#endif /* defined(__Military_Confrontation__MCRole__) */
