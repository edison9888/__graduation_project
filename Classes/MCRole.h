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
#include "MCEffect.h"
#include "MCAI.h"

extern const char *kMCRoleDiedNotification;

class MCScript;

/* 基础角色 */
class MCRole : public MCObject, public MCAIDelegate, public MCAIStateMachineDelegate {
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
    void roleWasAttacked(const MCEffect &anEffect);
    
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
    
    /**
     * 死亡
     */
    virtual void died();
    
    virtual CCObject *copy() = 0;
    
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
};

#endif /* defined(__Military_Confrontation__MCRole__) */
