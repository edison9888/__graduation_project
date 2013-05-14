//
//  MCHero.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCHero.h"
#include "MCAI.h"
#include "MCEquipmentManager.h"
#include "MCSkill.h"
#include "MCSkillManager.h"
#include "MCDungeonMaster.h"

static MCHero *__shared_hero = NULL;
const char *kMCHeroFacePath = "faces/x-000.png";
const char *kMCHeroSpriteSheetPath = "spritesheets/x-000";


MCHero::~MCHero()
{
}

bool
MCHero::init()
{
    CCString *face = CCString::create(kMCHeroFacePath);
    setFace(face);
    face->retain();
    loadSpriteSheet(kMCHeroSpriteSheetPath);
    
    roleType_ = MCRole::MCHero;
    roleRace_ = MCTerrans;
    mc_object_id_t o_id = {
        'X', '0', '0', '0'
    };
    setID(o_id);
    name_ = CCString::create("男猪脚");
    name_->retain();
    setHP(100);
    setMaxHP(100);
    setPP(100);
    setMaxPP(100);
    
    exhausted_ = false;
    exhaustion_ = 20;
    tired_ = 50;
    
    roleState_ = 0;
    spriteSheet_ = NULL;
    defaultDialogue_ = NULL;
    
    return true;
}

MCHero *
MCHero::sharedHero()
{
    if (__shared_hero == NULL) {
        __shared_hero = new MCHero;
        if (__shared_hero && __shared_hero->init()) {
        } else {
            CC_SAFE_DELETE(__shared_hero);
            __shared_hero = NULL;
        }
    }
    
    return __shared_hero;
}

MCRoleEntity *
MCHero::getEntity()
{
    MCRoleEntity *roleEntity = MCRole::getEntity();
    
    /* AI */
    if (ai_ == NULL) {
        ai_ = MCAI::create();
        ai_->retain();
        ai_->bind(this);
    }
    
    return roleEntity;
}


/**
 * 在主角面前的人物
 * 判断当前视野中的人物，然后返回最近的那个
 */
MCRole *
MCHero::roleOfFront()
{
    CCDictionary *roles = ai_->getRolesInVision();
    CCDictElement *elem;
    MCAIRole *role;
    MCRole *nearest = NULL;
    CCPoint roleCenter = getEntity()->getOBB().center;
    float minLength = 10000; /* 一个灰常大的值 */
    float length;
    
    CCDICT_FOREACH(roles, elem) {
        role = dynamic_cast<MCAIRole *>(elem->getObject());
        /* 对敌人表示无视 */
        if (role->role->getRoleType() == MCRole::MCEnemy) {
            continue;
        }
        length = ccpLength(ccpSub(role->role->getEntity()->getOBB().center, roleCenter));
        if (length < minLength) {
            nearest = role->role;
            minLength = length;
        }
    }
    
    return nearest;
}

/* 动作 */
/* 普通攻击 */
void
MCHero::attackTarget(MCRole *aTargetRole, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserObject)
{
    MCEquipmentItem *currentWeapon = MCEquipmentManager::sharedEquipmentManager()->getCurrentWeapon();
    MCWeapon *weapon = dynamic_cast<MCWeapon *>(currentWeapon->getEquipment());
    
    if (!canAttackTarget(aTargetRole)
        || weapon->consume > pp_ /* 不够体力 */
        || ai_->isStateLocked() /* 状态被锁定 */) {
        attackDidFail();
        return;
    }
    
    MCRole::attackTarget(aTargetRole);
    
    /* 检测攻击距离 */
    MCRoleEntity *selfEntity = getEntity();
    MCOBB targetOBB = aTargetRole->getEntity()->getOBB();
    MCOBB selfOBB = selfEntity->getOBB();
    CCPoint offset = ccpSub(targetOBB.center, selfOBB.center);
    float distance = ccpLength(offset) / selfOBB.width - 1;
    
    if (distance > (float) weapon->distance) { /* 太远了干不了，需要走过去 */
        target_ = aTarget;
        attackDidFinishSelector_ = aSelector;
        userObject_ = anUserObject;
        /* approachTargetAndKeepDistance版本不能保持距离和approachTarget效果一样，暂时放弃 */
//        selfEntity->approachTargetAndKeepDistance(aTarget,
//                                                  this,
//                                                  callfuncO_selector(MCHero::roleDidApproachTarget),
//                                                  aTarget,
//                                                  weapon->distance);
        selfEntity->approachTarget(aTargetRole,
                                   this,
                                   callfuncO_selector(MCRole::roleDidApproachTarget),
                                   aTargetRole);
        return;
    }
    pp_ -= weapon->consume;
    /* 提升熟练度 */
    MCSkillManager::sharedSkillManager()->improveProficiency(currentWeapon->getID().sub_class_ - '0');
    /* 进入攻击判断 */
    MCDungeonMaster::sharedDungeonMaster()->roleAttackTarget(this, aTargetRole);
    if (aTarget) {
        (aTarget->*aSelector)(anUserObject ? anUserObject : this);
    }
    target_ = NULL;
    attackDidFinishSelector_ = NULL;
    userObject_ = NULL;
}

/**
 * 死亡状态下回调
 */
void
MCHero::died()
{
    CCLog("hero is died");
    return;
    MCRoleEntity *roleEntity = getEntity();
    roleEntity->removeFromParentAndCleanup(false);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCRoleDiedNotification);
}

#pragma mark -
#pragma mark *** MCOffensiveProtocol ***

/**
 * 重击范围
 */
MCDiceRange
MCHero::getCriticalHitRange(bool inVision)
{
    MCWeapon *weapon = dynamic_cast<MCWeapon *>(MCEquipmentManager::sharedEquipmentManager()->getCurrentWeapon()->getEquipment());
    MCDiceRange diceRance = inVision ? weapon->criticalHitVisible : weapon->criticalHitInvisible;
    
#if MC_BATTLE_INFO_LEVEL == 1
    printf("%s在视野范围内，重击范围(%s%s%hu)\n",
           inVision ? "" : "不",
           diceRance.min == diceRance.max
           ? ""
           : CCString::createWithFormat("%hu", diceRance.min)->getCString(),
           diceRance.min == diceRance.max
           ? ""
           : "-",
           diceRance.max);
#endif
    
    return diceRance;
}

/**
 * 重击倍数
 */
mc_critical_hit_t
MCHero::getCriticalHit()
{
    MCEquipmentItem *weapon = MCEquipmentManager::sharedEquipmentManager()->getCurrentWeapon();
    
    return weapon->getCriticalHit();
}

/**
 * 己方攻击判定
 */
mc_offensive_t
MCHero::getOffensive()
{
    MCEquipmentItem *weapon = MCEquipmentManager::sharedEquipmentManager()->getCurrentWeapon();
    
    return weapon->getAttackCheck();
}

/**
 * 己方防御等级
 */
mc_ac_t
MCHero::getAC()
{
    return MCEquipmentManager::sharedEquipmentManager()->getAC();
}

/**
 * 攻击伤害
 * 武器伤害值
 */
mc_damage_t
MCHero::getOffensiveDamage()
{
    MCEquipmentItem *weapon = MCEquipmentManager::sharedEquipmentManager()->getCurrentWeapon();
    
    return weapon->getDamage();
}

/**
 * 防具检定减值
 */
mc_armor_check_penalty_t
MCHero::getArmorCheckPenalty()
{
    return MCEquipmentManager::sharedEquipmentManager()->getArmorCheckPenalty();
}

/**
 * 攻击附带效果
 */
MCRoleState
MCHero::attackWithState()
{
    MCWeapon *weapon = dynamic_cast<MCWeapon *>(MCEquipmentManager::sharedEquipmentManager()->getCurrentWeapon()->getEquipment());
    MCRoleState state = MCNormalState;
    
    if (weapon->effect != state) {
#if MC_BATTLE_INFO_LEVEL == 1
        printf("攻击附带状态检测: ");
#endif
        if (MCDiceRangeCheck(weapon->effectCheck)) {
#if MC_BATTLE_INFO_LEVEL == 1
            printf("附带上[%s]状态\n", MCRoleStateGetName(weapon->effect));
#endif
            state |= weapon->effect;
        }
    }
    
    return state;
}

/**
 * 普通攻击效果
 */
MCEffect *
MCHero::effectForNormalAttack()
{
    MCWeapon *weapon = dynamic_cast<MCWeapon *>(MCEquipmentManager::sharedEquipmentManager()->getCurrentWeapon()->getEquipment());
    
    return weapon->attackEffect;
}

/**
 * 获取人物的所有技能
 */
CCArray *
MCHero::getSkills() const
{
    MCSkillType skillType = MCEquipmentManager::sharedEquipmentManager()->getCurrentWeapon()->getID().sub_class_ - '0';
    
    return MCSkillManager::sharedSkillManager()->skillsForSkillType(skillType);
}

/**
 * 技能攻击评分
 * 伤害值——最大伤害值*伤害调整*技能次数
 * 对方状态——若有异常状态则4分，若没则0分。
 * 攻击范围——(obb.width*obb.height)*m(m值待定)
 */
mc_score_t
MCHero::getSkillDamageScore(MCSkill *aSkill)
{
    MCEquipmentItem *currentWeapon = MCEquipmentManager::sharedEquipmentManager()->getCurrentWeapon();
    MCWeapon *weapon = dynamic_cast<MCWeapon *>(currentWeapon->getEquipment());
    
    return MCDiceSize(weapon->damage)
            + (aSkill->effect != MCNormalState ? 4 : 0)
#warning m=2
            + (aSkill->breadth * aSkill->length) * 2;
}
