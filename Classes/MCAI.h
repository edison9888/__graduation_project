//
//  MCAI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCAI__
#define __Military_Confrontation__MCAI__

#include "MCObject.h"
#include "MCVision.h"
#include "MCEffect.h"

class MCAIDelegate;

enum {
    MCUnknownState      = 0,             /* 未知状态 */
    MCIdleState         = MCMakeEnum(0), /* 空闲状态 */
    MCCombatantStatus   = MCMakeEnum(1), /* 战斗状态 */
    MCRestingState      = MCMakeEnum(2), /* 休息状态 */
    MCAttackState       = MCMakeEnum(3), /* 攻击状态 */
    
    /* 佣兵附加 */
    MCFleeState         = MCMakeEnum(4), /* 逃跑状态 */
    MCDeathState        = MCMakeEnum(5)  /* 死亡状态 */
};
typedef mc_enum_t MCAIState;

/* AI所认为的人物 */
class MCAIRole : public CCObject {
public:
    void roleDied();
    
    MCRole *role; /* 指向真正的人物 */
    struct cc_timeval foundTimestamp; /* 发现该人物的时间 */
    bool isEnemy; /* 该人物为敌人 */
    mc_ushort_t aggro; /* 仇恨值 */
};

class MCAIStateMachineDelegate;

/**
 * 人物AI
 * 
 * 最基础的AI，主角的就是这个基础AI
 */
class MCAI : public MCObject {
    friend class MCRole;
public:
    MCAI();
    ~MCAI();
    
    bool init();
    
    /**
     * 绑定到人物
     * 此方法假定aRole已经实现了MCAIDelegate的代理方法
     */
    virtual void bind(MCRole *aRole);
    virtual MCRole *unbind();
    
    inline void unactivate() {
        activating_ = false;
        lastActivationTime_ = time(NULL);
    }
    
    inline void activate() {
        activating_ = true;
    }
    
    /**
     * 无敌人则返回NULL
     */
    MCRole *roleForMaxAggro();
    
    void update(float dt); /* 大脑在转动 */
    
    virtual void checkObjects(float dt); /* 检查视野中的对象是否还在 */
    
    CCObject *copy();
    
    CREATE_FUNC(MCAI);
    
protected:
    virtual void changingState(); /* 状态切换 */
    
    MCAIRole *roleForObjectId(mc_object_id_t anObjectId);
    
    MCRole *role_;
    bool activating_; /* 执行动作中 */
    
    CC_SYNTHESIZE_READONLY(CCDictionary *, rolesInVision_, RolesInVision);   /* 在视野中的非敌人 */
    CC_SYNTHESIZE_READONLY(CCDictionary *, enemiesInVision_, EnemiesInVision); /* 在视野中的敌人 */
    
    time_t lastActivationTime_; /* 最后激活时间，即为最后一次操作的时间 */
    
    CC_SYNTHESIZE(MCAIDelegate *, delegate_, Delegate);
    CC_SYNTHESIZE(MCAIStateMachineDelegate *, AIStateMachineDelegate_, AIStateMachineDelegate);
    CC_SYNTHESIZE_READONLY(MCVision *, vision_, Vision); /* 视野 */
    
    CC_SYNTHESIZE(MCAIState, AIState_, AIState); /* 状态 */
};

class MCAIStateMachineDelegate {
public:
    /**
     * 激活某状态
     */
    virtual void activate(MCAIState anAIState);
    
    /**
     * 空闲状态下回调
     */
    virtual void performWhenIdleState() {}
    
    /**
     * 战斗状态下回调
     */
    virtual void performWhenCombatantStatus() {}
    
    /**
     * 休息状态下回调
     */
    virtual void performWhenRestingState() {}
    
    /**
     * 攻击状态下回调
     */
    virtual void performWhenAttackState() {}
    
    /**
     * 死亡状态下回调
     */
    virtual void performWhenDeathState() {}
    
    /**
     * 这个不会被执行的= -！
     */
    virtual void performUnknownReaction() {}
};

class MCAIDelegate {
public:
    /**
     * 某人进入视野
     * 默认看到的都是敌人
     */
    virtual void roleDidEnterVision(MCRole *aRole, bool isEnermy = true) {}
    
    /**
     * 某人离开视野
     * 默认离开的都是敌人
     */
    virtual void roleDidExitVision(MCRole *aRole, bool isEnermy = true) {}
    
    /**
     * 被攻击
     */
    virtual void roleWasAttacked(const MCEffect &anEffect) {}
    
    /**
     * 状态切换
     */
    virtual void roleDidChangeStateTo(MCAIState anAIState) {}
};

#endif /* defined(__Military_Confrontation__MCAI__) */
