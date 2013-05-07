//
//  MCAI.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCAI.h"
#include "MCRole.h"
#include "MCScene.h"
#include "MCTeam.h"

MCAI::MCAI()
{
    role_ = NULL;
    vision_ = NULL;
    
    rolesInVision_ = CCDictionary::create();
    rolesInVision_->retain();
    enemiesInVision_ = CCDictionary::create();
    enemiesInVision_->retain();
}

MCAI::~MCAI()
{
    CC_SAFE_DELETE(vision_);
    CC_SAFE_RELEASE(rolesInVision_);
    CC_SAFE_RELEASE(enemiesInVision_);
}

bool
MCAI::init()
{
    activating_ = false;
    stateLocked_ = false;
    lastActivationTime_ = 0;
    
    return true;
}

/* 绑定到人物 */
void
MCAI::bind(MCRole *aRole)
{
    role_ = aRole;
    delegate_ = aRole;
    AIStateMachineDelegate_ = aRole;
    vision_ = MCVision::create(aRole->entity_); /* 已经生成 */
    AIState_ = MCIdleState;
}

MCRole *
MCAI::unbind()
{
    MCRole *temp = role_;
    
    if (temp) {
        temp->getEntity()->stopThinking();
    }
    role_ = NULL;
    
    return temp;
}

/**
 * 无敌人则返回NULL
 */
MCRole *
MCAI::roleForMaxAggro()
{
    MCAIRole *aiRole;
    MCAIRole *maxAggroAIRole = NULL;
    CCDictionary *roles = enemiesInVision_;
    CCDictElement *rolesElement;
    
    /* 搜集视野内人物 */
    CCDICT_FOREACH(roles, rolesElement) {
        aiRole = dynamic_cast<MCAIRole *>(rolesElement->getObject());
        if (maxAggroAIRole == NULL) {
            maxAggroAIRole = aiRole;
        } else if (aiRole->aggro > maxAggroAIRole->aggro) {
            maxAggroAIRole = aiRole;
        }
    }
    
    return maxAggroAIRole ? maxAggroAIRole->role : NULL;
}

void
MCAI::update(float dt) /* 大脑在转动 */
{
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    
    /* 搜集视野内人物 */
    checkRoles(sceneContext->getObjects());
    checkRoles(MCTeam::sharedTeam()->getRoles());

    /* 更新人物是否透支的状态 */
    role_->isExhausted();

    changingState();
}

/* 检查视野中的对象是否还在 */
void
MCAI::checkObjects(float dt)
{
    MCRole *role;
    MCAIRole *aiRole;
    CCDictionary *roles;
    CCDictElement *rolesElement;
    CCDictElement *enemiesElement;
    
    /* 敌人 */
    roles = enemiesInVision_;
    CCDICT_FOREACH(roles, enemiesElement) {
        aiRole = dynamic_cast<MCAIRole *>(enemiesElement->getObject());
        role = aiRole->role;
        if (! vision_->collideWith(role->getEntity())) { /* 人物离开了视野 */
            int key = MCObjectIdToDickKey(role->getID());
            delegate_->roleDidExitVision(role, true);
            enemiesInVision_->removeObjectForKey(key);
        }
    }
    
    /* 通常人物 */
    roles = rolesInVision_;
    CCDICT_FOREACH(roles, rolesElement) {
        aiRole = dynamic_cast<MCAIRole *>(rolesElement->getObject());
        role = aiRole->role;
        if (! vision_->collideWith(role->getEntity())) { /* 人物离开了视野 */
            int key = MCObjectIdToDickKey(role->getID());
            delegate_->roleDidExitVision(role, false);
            rolesInVision_->removeObjectForKey(key);
        }
    }
}

CCObject *
MCAI::copy()
{
    MCAI *ai = new MCAI;
    
    ai->init();
    
    return ai;
}

void
MCAI::checkRoles(CCArray *roles)
{
    CCObject *obj;
    MCRole *role;
    MCAIRole *aiRole;
    struct cc_timeval tv;
    
    CCARRAY_FOREACH(roles, obj) {
        role = dynamic_cast<MCRole *>(obj);
        if (role_ == role) {
            continue;
        }
        if (vision_->collideWith(role->getEntity())) {
            aiRole = roleInVision(role);
            CCTime::gettimeofdayCocos2d(&tv, NULL);
            if (aiRole) { /* 更新人物的发现时间 */
                aiRole->foundTimestamp = tv;
            } else { /* 初次发现人物 */
                bool isEnemy = role_->isEnemy(role);
                aiRole = new MCAIRole;
                aiRole->autorelease();
                aiRole->role = role;
                aiRole->foundTimestamp = tv;
                aiRole->isEnemy = isEnemy;
                aiRole->aggro = 0;
                delegate_->roleDidEnterVision(role, isEnemy);
                if (isEnemy) {
                    enemiesInVision_->setObject(aiRole, MCObjectIdToDickKey(role->getID()));
                } else {
                    rolesInVision_->setObject(aiRole, MCObjectIdToDickKey(role->getID()));
                }
            }
        }
    }
}

void
MCAI::changingState() /* 状态切换 */
{
    if (activating_) {
        return;
    }
    AIStateMachineDelegate_->activate(AIState_);
}

MCAIRole *
MCAI::roleInVision(MCRole *aRole)
{
    MCAIRole *aiRole;
    CCDictionary *roles;
    CCDictElement *rolesElement;
    CCDictElement *enemiesElement;
    
    roles = enemiesInVision_;
    CCDICT_FOREACH(roles, enemiesElement) {
        aiRole = dynamic_cast<MCAIRole *>(enemiesElement->getObject());
        if (aiRole->role == aRole) {
            return aiRole;
        }
    }
    
    roles = rolesInVision_;
    CCDICT_FOREACH(roles, rolesElement) {
        aiRole = dynamic_cast<MCAIRole *>(rolesElement->getObject());
        if (aiRole->role == aRole) {
            return aiRole;
        }
    }
    
    return NULL;
}

#pragma mark -
#pragma mark *** MCAIStateMachineDelegate ***

void
MCAIStateMachineDelegate::activate(MCAIState anAIState)
{
    if (anAIState == MCIdleState) {
        performWhenIdleState();
    } else if (anAIState == MCCombatantStatus) {
        performWhenCombatantStatus();
    } else if (anAIState == MCRestingState) {
        performWhenRestingState();
    } else if (anAIState == MCAttackState) {
        performWhenAttackState();
    } else if (anAIState == MCDeathState) {
        performWhenDeathState();
    } else {
        performUnknownReaction();
    }
}
