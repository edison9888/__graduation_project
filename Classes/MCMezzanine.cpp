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

MCMezzanine::~MCMezzanine()
{
    CC_SAFE_RELEASE(skills_);
}

bool
MCMezzanine::init()
{
    if (CCLayer::init()) {
        skills_ = CCArray::create();
        skills_->retain();
        
        return true;
    }
    
    return false;
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
        detectsSkills(role);
        scene->detectsCollidesWithSemiTransparents(role);
    }
    
    /* hero and mercenaries */
    CCARRAY_FOREACH(team, obj) {
        role = dynamic_cast<MCRole *>(obj);
        detectsSkills(role);
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
#warning autorelease?
    skills_->removeObject(anObject);
}
