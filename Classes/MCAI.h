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

/* AI所认为的人物 */
class MCAIRole : public CCObject {
public:
    MCRole *role; /* 指向真正的人物 */
    struct cc_timeval foundTimestamp; /* 发现该人物的时间 */
    bool isEnemy; /* 该人物为敌人 */
    mc_ushort_t aggro; /* 仇恨值 */
};

class MCAI : public MCObject {
    friend class MCRole;
public:
    MCAI();
    ~MCAI();
    
    bool init();
    
    CREATE_FUNC(MCAI);
    
    /**
     * 绑定到人物
     * 此方法假定aRole已经实现了MCAIDelegate的代理方法
     */
    void bind(MCRole *aRole);
    MCRole *unbind();
    
    void update(float dt); /* 大脑在转动 */
    
    void checkObjects(float dt); /* 检查视野中的对象是否还在 */
    
    CCArray *rolesInVisions();
    
protected:
    MCRole *role_;
    
    CCArray *rolesInVisions_;   /* 某一瞬间在视野中的人物，每次返回之的时候都会更新 */
    CCDictionary *inVisions_;   /* 在视野中的人物 */
    CCDictionary *lostVisions_; /* 原本在视野中，但失去踪影的人物 */
    
    CC_SYNTHESIZE(MCAIDelegate *, delegate_, Delegate);
    CC_SYNTHESIZE_READONLY(MCVision *, vision_, Vision); /* 视野 */
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
};

#endif /* defined(__Military_Confrontation__MCAI__) */
