//
//  MCSkill.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSkill__
#define __Military_Confrontation__MCSkill__

#include "MCObject.h"
#include "MCEquipmentProperty.h"
#include "MCRoleProperty.h"
#include "MCDice.h"
#include "MCOBB.h"

extern const char *kMCSkillActionIsDoneNotification;

class MCSkillManager;
class MCEffect;
class MCRole;
class MCSkillAction;

typedef float mc_release_time_t;
typedef float mc_active_time_t;
typedef float mc_cold_time_t;
typedef float mc_revise_t;

enum {
    MCNoSpecial,
    MCIgnorePP
};
typedef mc_enum_t MCSpecial;

/*
 字段                  |  字段说明       | 解释
 ---------------------|----------------|---------
 ID                   | 技能ID         | 技能ID为技能字典中技能的key
 name                 | 技能名称        | 技能的名称
 icon                 | 技能图标        | 技能的图标，敌方技能没有图标
 effect-id            | 效果ID         | 效果ID为效果字典中效果的key
 release-time         | 释放所需时间     | 从启动释放到释放完成所需要的时间(此时间完成后才开始技能判定)
 active-time          | 完成所需时间     | 从释放到消失所经过的时间(此时间完成后效果消失)
 cold-time            | 冷却时间        | 到下一次释放技能所需要的等待时间
 distance             | 攻击距离        | 释放中心点到消失位置的距离
 length               | 攻击范围长      | OBB的高
 breadth              | 攻击范围宽      | OBB的宽
 consume              | 体力消耗        | 释放所需要消耗的体力
 required-proficiency | 熟练度          | 使用该技能所需要的熟练度
 revise               | 效果(伤害修正)   | 对对方产生的伤害的倍数
 count                | 连续释放次数     | 技能释放次数
 hp                   | HP影响         | 佣兵被动技能所有，对整个队伍的人物的HP的加成
 pp                   | PP影响         | 佣兵被动技能所有，对整个队伍的人物的PP的加成
 damage-bonus         | 伤害值加成      | 佣兵被动技能所有，对整个队伍的人物的伤害值的加成
 armor-check-penalty  | 防具检定减值加成 | 佣兵被动技能所有，对整个队伍的人物的防具检定减值的加成
 special              | 特殊效果        | 佣兵被动技能所有，具体效果查看特殊效果字典
 effect               | 附带效果        | 状态攻击
 effect-check         | 附带效果判定     | 状态攻击的判断范围
 trigger-type         | 效果触发类型     | 值为0或者1，0表示被动技能，1表示主动技能
 */
class MCSkill : public MCObject {
    friend class MCSkillManager;
    friend class MCSkillAction;
public:
    MCSkill()
    : name_(NULL)
    , icon_(NULL)
    , effect_(NULL)
    , fakeNode_(NULL) {}
    
    ~MCSkill();
    
    enum {
        MCPassiveSkill, /* 被动技能 */
        MCActiveSkill /* 主动技能 */
    };
    typedef mc_enum_t MCTriggerType;
    
    bool canRoleUse(MCRole *aRole);
    
    bool willBeExhausted(MCRole *aRole);
    
    /**
     * 使用技能
     * anOrigin为技能释放的初始位置
     * anAngle为释放角度，对原地释放的技能来说角度没意义
     */
    void use(const CCPoint &anOrigin, float anAngle);
    
    /**
     * 攻击附带效果
     */
    MCRoleState attackWithState();
    
    /* 运行由夹层世界生成的MCSkillAction */
    void runSkillAction(MCSkillAction *aSkillAction);
    
    void skillDidHitTarget(MCRole *aTarget);
    
    inline CCNode *fakeNode() {
        return fakeNode_;
    }
    
    CCObject *copy();
    
    
public:
    mc_release_time_t releaseTime; /* 释放所需时间 */
    mc_active_time_t activeTime; /* 完成所需时间 */
    mc_cold_time_t coldTime; /* 冷却时间 */
    mc_distance_t distance; /* 攻击距离 */
    mc_distance_t length; /* 攻击范围长 */
    mc_distance_t breadth; /* 攻击范围宽 */
    mc_pp_t consume; /* 体力消耗 */
    mc_proficiency_t requiredProficiency; /* 熟练度 */
    mc_revise_t revise; /* 效果(伤害修正) */
    mc_size_t count; /* 连续释放次数 */
    mc_hp_t hp; /* HP影响 */
    mc_pp_t pp; /* PP影响 */
    mc_damage_t damageBonus; /* 伤害值加成 */
    mc_armor_check_penalty_t armorCheckPenalty; /* 防具检定减值加成 */
    MCSpecial special; /* 特殊效果 */
    MCRoleState effect; /* 附带效果 */
    MCDiceRange effectCheck; /* 附带效果判定 */
    MCTriggerType triggerType; /* 效果触发类型 */
    bool isContinuable; /* 是否为可持续的，即命中后是否继续存在至结束 */
    
    CC_SYNTHESIZE(CCString *, name_, Name);
    CC_SYNTHESIZE(CCString *, icon_, Icon);
    CC_SYNTHESIZE(MCEffect *, effect_, Effect); /* 注意！effect_和effect是不同的东西。。。 */
    
    CC_SYNTHESIZE(MCRole *, launcher_, Launcher);
    
    /* 碰撞运算 */
    MCOBB obb;
    
private:
    void onReady(CCObject *anObject); /* 技能释放准备结束，释放技能 */
    
    CCNode *fakeNode_; /* 给MCSkillAction用 */
};

class MCSkillAction : public CCActionInterval {
public:
    /** initializes the action */
    bool initWithSkill(MCSkill *aSkill);
    
    CCObject *copyWithZone(CCZone* pZone);
    void startWithTarget(CCNode *pTarget);
    void step(float dt);
    void update(float time);
    
public:
    /** creates the action */
    static MCSkillAction *create(MCSkill *aSkill);
protected:
    CCPoint positionDelta_;
    CCPoint startPosition_;
    CCPoint previousPosition_;
};

#endif /* defined(__Military_Confrontation__MCSkill__) */
