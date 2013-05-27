//
//  MCEnemy.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEnemy.h"
#include "MCEnemyAI.h"
#include "MCScene.h"
#include "MCSkill.h"
#include "MCDungeonMaster.h"

MCEnemy::~MCEnemy()
{
    CC_SAFE_RELEASE(skills_);
    CC_SAFE_RELEASE(attackEffect_);
    CC_SAFE_RELEASE(actionEffect_);
}

bool
MCEnemy::init(MCRoleRace aRoleRace)
{
    roleType_ = MCRole::MCEnemy;
    setRoleRace(aRoleRace);
    roleState_ = MCNormalState;
    face_ = NULL;
    defaultDialogue_ = NULL;
    center_ = CCPointZero;
    skills_ = CCArray::create();
    skills_->retain();
    
    damageScore_ = -1;
    
    return true;
}

MCEnemy *
MCEnemy::create(mc_object_id_t anObjectId)
{
    MCEnemy *enemy = new MCEnemy;
    
    if (enemy && enemy->init(MCTerrestrial)) {
        enemy->setID(anObjectId);
        enemy->autorelease();
        /* 加载enemy数据 */
        /* 检测怪物种族 */
    } else {
        CC_SAFE_DELETE(enemy);
        enemy = NULL;
    }
    
    return enemy;
}

#pragma mark -
#pragma mark *** MCMercenary::MCAIStateMachineDelegate ***

/**
 * 空闲状态下回调
 */
void
MCEnemy::performWhenIdleState()
{
    MCRole::performWhenIdleState();
    
    time_t lastActivationTime = ai_->lastActivationTime_;
    time_t now = time(NULL);
    
    /* 停4秒 */
    if ((lastActivationTime != 0
        && now - lastActivationTime > 4)
        || lastActivationTime == 0) {
        round();
    }
}

MCRoleEntity *
MCEnemy::getEntity()
{
    MCRoleEntity *roleEntity = MCRole::getEntity();
    
    /* AI */
    if (ai_ == NULL) {
        ai_ = MCEnemyAI::create();
        ai_->retain();
        ai_->bind(this);
    }
    
    return roleEntity;
}

/* 巡逻 */
void
MCEnemy::round()
{
    ai_->activate();
    
    MCRoleEntity *entity = getEntity();
    float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
    
    if (center_.equals(CCPointZero)) {
        MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
        MCScene *scene = sceneContext->getScene();
        center_ = ccpSub(entity->getPosition(), scene->getMapOffset());
    }
    
    /* 大概4个基本身位的半径范围内随便走 */
    float dx = (rand() % 4 * 24 / contentScaleFactor) * (rand() % 2 == 1 ? 1 : -1);
    float dy = (rand() % 4 * 24 / contentScaleFactor) * (rand() % 2 == 1 ? 1 : -1);
    
    entity->findPathAtMap(ccpAdd(center_, ccp(dx, dy)),
                          this,
                          callfuncO_selector(MCEnemy::roundDidFinish));
}

#pragma mark -
#pragma mark *** MCOffensiveProtocol ***

/**
 * 重击范围
 */
MCDiceRange
MCEnemy::getCriticalHitRange(bool inVision)
{
    MCDiceRange diceRance = inVision ? criticalHitVisible_ : criticalHitInvisible_;
    
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
MCEnemy::getCriticalHit()
{
    return criticalHit_;
}

/**
 * 己方攻击判定
 */
mc_offensive_t
MCEnemy::getOffensive()
{
    return MCDiceMaker::sharedDiceMaker()->attackCheck() + dexterity_;
}

/**
 * 己方防御等级
 */
mc_ac_t
MCEnemy::getAC()
{
    return ac_;
}

/**
 * 攻击伤害
 * 武器伤害值
 */
mc_damage_t
MCEnemy::getOffensiveDamage()
{
    return damage_->roll() + damageBonus_;
}

/**
 * 防具检定减值
 */
mc_armor_check_penalty_t
MCEnemy::getArmorCheckPenalty()
{
    return armorCheckPenalty_;
}

/**
 * 攻击附带效果
 */
MCRoleState
MCEnemy::attackWithState()
{
    MCRoleState state = MCNormalState;
    
    if (effect_ != state) {
#if MC_BATTLE_INFO_LEVEL == 1
        printf("攻击附带状态检测: ");
#endif
        if (MCDiceRangeCheck(effectCheck_)) {
#if MC_BATTLE_INFO_LEVEL == 1
            printf("附带上[%s]状态\n", MCRoleStateGetName(effect_));
#endif
            state |= effect_;
        }
    }
    
    return state;
}

/**
 * 普通攻击效果
 */
MCEffect *
MCEnemy::effectForNormalAttack()
{
    return attackEffect_;
}

/**
 * 技能攻击评分
 * 伤害值——最大伤害值*伤害调整*技能次数
 * 对方状态——若有异常状态则4分，若没则0分。
 * 攻击范围——(obb.width*obb.height)*m(m值待定)
 */
mc_score_t
MCEnemy::getSkillDamageScore(MCSkill *aSkill)
{
    return damage_->size()
            + (aSkill->effect != MCNormalState ? 4 : 0)
#warning m=2
            + (aSkill->breadth * aSkill->length) * 2;
}

/* 动作 */
/* 能否发动攻击 */
bool
MCEnemy::canAttackTarget(MCRole *aRole)
{
    if (MCRole::canAttackTarget(aRole)
        && pp_ >= consume_ /*足够体力 */) {
        return true;
    }
    
    return false;
}

/* 普通攻击 */
void
MCEnemy::attackTarget(MCRole *aTargetRole, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserObject)
{
    if (! canAttackTarget(aTargetRole)) {
        attackDidFail();
        return;
    }
    
    MCRole::attackTarget(aTargetRole);
    ai_->lockState();
    
    /* 检测攻击距离 */
    MCRoleEntity *selfEntity = getEntity();
    MCOBB targetOBB = aTargetRole->getEntity()->getOBB();
    MCOBB selfOBB = selfEntity->getOBB();
    CCPoint offset = ccpSub(targetOBB.center, selfOBB.center);
    float distance = ccpLength(offset) / selfOBB.width - 1;
    
    if (distance > (float) distance_) { /* 太远了干不了，需要走过去 */
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
    /* 进入攻击判断 */
//    printf("进入攻击判断\n");
    pp_ -= consume_;
    MCDungeonMaster::sharedDungeonMaster()->roleAttackTarget(this, aTargetRole);
    ai_->unlockState();
    if (aTarget) {
        (aTarget->*aSelector)(anUserObject ? anUserObject : this);
    }
    target_ = NULL;
    attackDidFinishSelector_ = NULL;
    userObject_ = NULL;
}

/**
 * 死亡
 */
void
MCEnemy::died()
{
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    sceneContext->enemyWasDied(this);
        
    MCRoleEntity *roleEntity = getEntity();
    roleEntity->stopPathFinding();
    roleEntity->stopWalking();
    roleEntity->getSpriteSheet()->removeFromParentAndCleanup(true);
    entity_ = NULL;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCRoleDiedNotification,
                                                                       this);
}

CCObject *
MCEnemy::copy()
{
    MCEnemy *enemy = new MCEnemy;
    
    enemy->init(roleRace_);
    enemy->id_ = id_;
    enemy->tag_ = tag_;
    enemy->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    enemy->name_->retain();
    enemy->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
    enemy->description_->retain();
    
    enemy->roleType_ = roleType_;
    enemy->roleRace_ = roleRace_;
    enemy->hp_ = hp_;
    enemy->pp_ = pp_;
    enemy->consume_ = consume_;
    enemy->maxHP_ = maxHP_;
    enemy->maxPP_ = maxPP_;
    enemy->exhaustion_ = exhaustion_;
    enemy->tired_ = tired_;
    enemy->dexterity_ = dexterity_;
    enemy->roleState_ = MCNormalState;
    enemy->face_ = NULL;
    enemy->spriteSheet_ = CCString::create(spriteSheet_->getCString()); /* 会被释放掉，所以要copy一个 */
    enemy->spriteSheet_->retain();
    enemy->defaultDialogue_ = NULL;
    enemy->ac_ =ac_;
    enemy->armorCheckPenalty_ = armorCheckPenalty_;
    enemy->damage_ = damage_;
    enemy->damageBonus_ = damageBonus_;
    enemy->criticalHitVisible_ = criticalHitVisible_;
    enemy->criticalHitInvisible_ = criticalHitInvisible_;
    enemy->criticalHit_ = criticalHit_;
    enemy->distance_ =distance_;
    enemy->effect_ = effect_;
    enemy->effectCheck_ = effectCheck_;
    enemy->attackEffect_ = dynamic_cast<MCEffect *>(attackEffect_->copy());
    if (ai_) {
        enemy->ai_ = dynamic_cast<MCEnemyAI *>(ai_->copy());
    }
    enemy->trigger_ = trigger_;
    
    CCObject *obj;
    CCARRAY_FOREACH(skills_, obj) {
        enemy->skills_->addObject(dynamic_cast<MCSkill *>(obj)->copy());
    }
    enemy->damageScore_ = -1;
    enemy->actionEffect_ = CCString::create(actionEffect_->getCString()); /* 会被释放掉，所以要copy一个 */
    enemy->actionEffect_->retain();
    
    return enemy;
}

void
MCEnemy::roundDidFinish(CCObject *anObject)
{
    ai_->unactivate();
}
