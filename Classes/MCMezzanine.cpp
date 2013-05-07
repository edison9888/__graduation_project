//
//  MCMezzanine.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCMezzanine.h"
#include "MCTeam.h"
#include "MCSkill.h"
#include "MCTrap.h"

MCMezzanine::~MCMezzanine()
{
    CC_SAFE_RELEASE(drawer_);
    CC_SAFE_RELEASE(traps_);
    CC_SAFE_RELEASE(skills_);
}

bool
MCMezzanine::init()
{
    if (CCLayer::init()) {
        skills_ = CCArray::create();
        skills_->retain();
        traps_ = CCArray::create();
        traps_->retain();
        
        draggingTrap_ = NULL;
        
        drawer_ = CCDrawNode::create();
        addChild(drawer_);
        
        return true;
    }
    
    return false;
}

void
MCMezzanine::draw()
{
    CCLayer::draw();
    
    CCDrawNode *drawer = drawer_;
    drawer->clear();
    /* 没有陷阱不需要绘制 */
    if (traps_->count() == 0 && !draggingTrap_) {
        return;
    }
    CCPoint mapOffset = MCSceneContextManager::sharedSceneContextManager()->currentContext()->getScene()->getMapOffset();
    ccColor4F color = ccc4f(0.8, 0.2, 0.2, 0.4);
    
    /* 绘制拖拽ing的陷阱的范围 */
    if (draggingTrap_) {
//        drawer->drawDot(ccpSub(draggingTrap_->getPosition(), mapOffset),
        drawer->drawDot(draggingTrap_->getPosition(),
                        draggingTrap_->radius,
                        color);
    }
    
    /* 绘制已放置的陷阱的范围 */
    CCArray *traps = traps_;
    MCTrap *trap;
    CCObject *obj;
    CCARRAY_FOREACH(traps, obj) {
        trap = dynamic_cast<MCTrap *>(obj);
        drawer->drawDot(ccpAdd(trap->getPosition(), mapOffset),
//        drawer->drawDot(ccpSub(trap->getPosition(), mapOffset),
                        trap->radius,
                        color);
    }
}

void
MCMezzanine::drawDraggingTrap(MCTrap *aTrap)
{
    
}

/* 已设置好位置的陷阱 */
void
MCMezzanine::installTrap(MCTrap *aTrap)
{
    traps_->addObject(aTrap);
}

/* aLocation为屏幕上位置 */
void
MCMezzanine::installTrap(MCTrap *aTrap, const CCPoint &aLocation)
{
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = sceneContext->getScene();
    aTrap->setPosition(ccpSub(aLocation, scene->getMapOffset()));
    traps_->addObject(aTrap);
}

void
MCMezzanine::onEnter()
{
    CCLayer::onEnter();
    schedule(schedule_selector(MCMezzanine::flow));
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(MCMezzanine::skillDidFinish),
                                                                  kMCSkillActionIsDoneNotification,
                                                                  NULL);
}

void
MCMezzanine::onExit()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kMCSkillActionIsDoneNotification);
    unschedule(schedule_selector(MCMezzanine::flow));
    CCLayer::onExit();
}

void
MCMezzanine::flow(float dt)
{
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = sceneContext->getScene();
    CCObject *obj;
    MCRole *role;
    CCArray *roles = sceneContext->getObjects();
    CCArray *team = MCTeam::sharedTeam()->getRoles();
    
    /* 技能变化检测 */
    
    /* 对象状态变化 */
    /* objects */
    CCARRAY_FOREACH(roles, obj) {
        role = dynamic_cast<MCRole *>(obj);
        /* check traps */
        detectsTraps(role);
        /* check skills */
        detectsSkills(role);
        /* check semi-transparents */
        scene->detectsCollidesWithSemiTransparents(role);
    }
    
    /* hero and mercenaries */
    CCARRAY_FOREACH(team, obj) {
        role = dynamic_cast<MCRole *>(obj);
        /* check skills */
        detectsSkills(role);
        /* check semi-transparents */
        scene->detectsCollidesWithSemiTransparents(role);
    }
    
}

void
MCMezzanine::attach(MCSkill *aSkill)
{
    MCSkillAction *skillAction = MCSkillAction::create(aSkill);
    
    skills_->addObject(aSkill);
    addChild(aSkill->fakeNode());
    aSkill->runSkillAction(skillAction);
}

void
MCMezzanine::detectsSkills(MCRole *aRole)
{
    CCObject *obj;
    CCArray *skills = skills_;
    MCSkill *skill;
    MCRole *launcher;
    const MCOBB &obb = aRole->getEntity()->getOBB();
    
    CCARRAY_FOREACH(skills, obj) {
        skill = dynamic_cast<MCSkill *>(obj);
        launcher = skill->getLauncher();
        /* 不击中自己人 */
        if (! launcher->isEnemy(aRole)) {
            continue;
        }
        if (skill->obb.collidesWith(obb)) {
            /* 命中目标！ */
            skill->skillDidHitTarget(aRole);
        }
    }
}

void
MCMezzanine::skillDidFinish(CCObject *anObject)
{
    skills_->removeObject(anObject);
//    anObject->release();
}

void
MCMezzanine::detectsTraps(MCRole *aRole)
{
    CCObject *obj;
    CCArray *traps = traps_;
    MCTrap *trap;
    
    CCARRAY_FOREACH(traps, obj) {
        trap = dynamic_cast<MCTrap *>(obj);
        if (trap->collidesWith(aRole)) {
            trapDidTriggerByRole(trap, aRole);
        }
    }
}

void
MCMezzanine::trapDidTriggerByRole(MCTrap *aTrap, MCRole *aRole)
{
    CCPoint mapOffset = MCSceneContextManager::sharedSceneContextManager()->currentContext()->getScene()->getMapOffset();
    MCEffect *hitEffect = dynamic_cast<MCEffect *>(aTrap->effect->copy());
    
    hitEffect->autorelease();
    hitEffect->attach(this, ccpAdd(aTrap->getPosition(), mapOffset));
    
    /* 命中伤害 */
    mc_effect_t effect = {
        aTrap->hp, /* 已经是减值 */
        aTrap->positive_state,
        aTrap->lasting_time
    };
    aRole->roleWasAttacked(effect);
    traps_->removeObject(aTrap);
}
