//
//  MCSkill.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSkill.h"
#include "MCEffect.h"
#include "MCScene.h"
#include "MCMezzanine.h"
#include "MCEffectManager.h"

const char *kMCSkillActionIsDoneNotification = "kMCSkillActionIsDoneNotification";

MCSkill::~MCSkill()
{
    CC_SAFE_RELEASE(fakeNode_);
}

bool
MCSkill::canRoleUse(MCRole *aRole)
{
    return aRole->getPP() >= consume;
}

bool
MCSkill::willBeExhausted(MCRole *aRole)
{
    return aRole->getPP() - consume <= aRole->getExhaustion();
}

/**
 * 使用技能
 * anOrigin为技能释放的初始位置
 * anAngle为释放角度，对原地释放的技能来说角度没意义
 */
void
MCSkill::use(const CCPoint &anOrigin, float anAngle)
{
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = sceneContext->getScene();
    MCMezzanine *mezzanine = scene->mezzanine();
    float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
    
    obb.setup(ccpSub(anOrigin, scene->getMapOffset()),
              breadth * 24 / contentScaleFactor,
              length * 24 / contentScaleFactor,
              anAngle);
    CCLog("%lu",effect_->retainCount());
    effect_->bind(this);
    CC_SAFE_RELEASE(fakeNode_);
    fakeNode_ = new CCNode;
    fakeNode_->init();
    fakeNode_->setUserObject(this);
    fakeNode_->setPosition(anOrigin);
    
    MCEffect *launchingEffect = MCEffectManager::sharedEffectManager()->launchingEffect();
    launchingEffect->retain();
    launchingEffect->bind(this);
    launcher_->getEntity()->lockPosition();
    launchingEffect->attach(scene,
                            launcher_,
                            this,
                            callfuncO_selector(MCSkill::onReady),
                            mezzanine);
}

/**
 * 攻击附带效果
 */
MCRoleState
MCSkill::attackWithState()
{
    MCRoleState state = MCNormalState;
    
    if (effect != state
        && MCDiceRangeCheck(effectCheck)) {
        state |= effect;
    }
    
    return state;
}

/* 运行由夹层世界生成的MCSkillAction */
void
MCSkill::runSkillAction(MCSkillAction *aSkillAction)
{
    fakeNode_->runAction(aSkillAction);
}

void
MCSkill::skillDidHitTarget(MCRole *aTarget)
{
    /* show effect */
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = sceneContext->getScene();
    MCMezzanine *mezzanine = scene->mezzanine();
    effect_->attach(mezzanine, aTarget);
    
    MCRole *launcher = launcher_;
    /* 重击判定 */
    /* 视野范围内？ */
    MCDiceRange diceRange = launcher->getCriticalHitRange(aTarget->roleInVision(launcher));
    bool isCriticalHit = launcher->attackCheckCriticalHit(diceRange);
    
    /* 决定伤害值 */
    mc_damage_t damage = launcher->attackGetDamage(launcher->getOffensiveDamage(), aTarget->getArmorCheckPenalty());
    if (isCriticalHit) {
        damage *= launcher->getCriticalHit();
    }
    
#if MC_BATTLE_INFO_LEVEL == 1
    printf("伤害值: %hi * %.1f = %hi\n",
           damage,
           revise,
           (mc_damage_t) ((float) damage * revise));
#endif
    damage = (mc_damage_t) ((float) damage * revise);
    
    mc_effect_t effect = {
        -damage,
        attackWithState(), /* 某些有特殊效果的技能攻击 */
        0.0f
    };
    aTarget->roleWasAttacked(effect);
    
    if (! isContinuable) {
        /* 不是可持续的技能的话，命中后就将消失 */
        mezzanine->detach(this);
    }
}

CCObject *
MCSkill::copy()
{
    MCSkill *skill = new MCSkill;
    
    skill->id_ = id_;
    skill->tag_ = tag_;
    skill->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    skill->name_->retain();
    if (description_) {
        skill->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
        skill->description_->retain();
    }
    
    skill->releaseTime = releaseTime;
    skill->activeTime = activeTime;
    skill->coldTime = coldTime;
    skill->distance = distance;
    skill->length = length;
    skill->breadth = breadth;
    skill->consume = consume;
    skill->requiredProficiency = requiredProficiency;
    skill->revise = revise;
    skill->count = count;
    skill->hp = hp;
    skill->pp = pp;
    skill->damageBonus = damageBonus;
    skill->armorCheckPenalty = armorCheckPenalty;
    skill->special = special;
    skill->effect = effect;
    skill->effectCheck = effectCheck;
    skill->triggerType = triggerType;
    skill->isContinuable = isContinuable;
    
    if (icon_) {
        skill->icon_ = CCString::create(icon_->getCString()); /* 会被释放掉，所以要copy一个 */
        skill->icon_->retain();
    }
    if (effect_) {
        skill->effect_ = dynamic_cast<MCEffect *>(effect_->copy());
    }
    
    return skill;
}

/* 技能释放准备结束，释放技能 */
void
MCSkill::onReady(CCObject *anObject)
{
    MCMezzanine *mezzanine = dynamic_cast<MCMezzanine *>(anObject);
    /* 添加进夹层世界 */
    mezzanine->attach(this);
}

#pragma mark -
#pragma mark *** MCSkillAction ***

MCSkillAction *
MCSkillAction::create(MCSkill *aSkill)
{
    MCSkillAction *skillAction = new MCSkillAction;
    
    if (skillAction && skillAction->initWithSkill(aSkill)) {
        skillAction->autorelease();
    } else {
        CC_SAFE_DELETE(skillAction);
        skillAction = NULL;
    }
    
    return skillAction;
}

bool
MCSkillAction::initWithSkill(MCSkill *aSkill)
{
    if (CCActionInterval::initWithDuration(aSkill->activeTime)) {
        /* 24是基本单位 */
        CCPoint center = aSkill->obb.center;
        float distance = (float) aSkill->distance * 24.0f / CC_CONTENT_SCALE_FACTOR();
        float radians = aSkill->obb.rotation;
        
        CCPoint targetPosition = ccp(center.x + distance * cosf(radians),
                                     center.y + distance * sinf(radians));
        positionDelta_ = ccpSub(targetPosition, center);
        
        return true;
    }
    
    return false;
}

CCObject *
MCSkillAction::copyWithZone(CCZone *pZone)
{
    CCZone *pNewZone = NULL;
    MCSkillAction *pCopy = NULL;
    
    if(pZone && pZone->m_pCopyObject) {
        //in case of being called at sub class
        pCopy = (MCSkillAction *) (pZone->m_pCopyObject);
    } else {
        pCopy = new MCSkillAction;
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    MCSkill *skill = dynamic_cast<MCSkill *>(m_pTarget->getUserObject());
    pCopy->initWithSkill(skill);
    
    CC_SAFE_DELETE(pNewZone);
    
    return pCopy;
}

void
MCSkillAction::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    MCSkill *skill = dynamic_cast<MCSkill *>(m_pTarget->getUserObject());
    if (skill) {
        previousPosition_ = startPosition_ = skill->fakeNode()->getPosition();
    }
}

void
MCSkillAction::step(float dt)
{
    CCActionInterval::step(dt);
    MCSkill *skill = dynamic_cast<MCSkill *>(m_pTarget->getUserObject());
    
    if (isDone() && skill) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCSkillActionIsDoneNotification,
                                                                           skill);
    }
}


void
MCSkillAction::update(float t)
{
    MCSkill *skill = dynamic_cast<MCSkill *>(m_pTarget->getUserObject());
    if (skill) {
#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint currentPosition = skill->obb.center;
        CCPoint diff = ccpSub(currentPosition, previousPosition_);
        startPosition_ = ccpAdd(startPosition_, diff);
        CCPoint newPosition =  ccpAdd(startPosition_, ccpMult(positionDelta_, t));
        skill->obb.center = newPosition;
        previousPosition_ = newPosition;
#else
        skill->obb.center = ccpAdd(startPosition_, ccpMult(positionDelta_, t));
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}
