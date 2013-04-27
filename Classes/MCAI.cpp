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
    CCArray *roles = sceneContext->getObjects();
    CCObject *obj;
    MCRole *role;
    MCAIRole *aiRole;
    struct cc_timeval tv;
    
    /* 搜集视野内人物 */
    CCARRAY_FOREACH(roles, obj) {
        role = dynamic_cast<MCRole *>(obj);
        if (vision_->collideWith(role->getEntity())) {
            aiRole = roleForObjectId(role->getID());
            CCTime::gettimeofdayCocos2d(&tv, NULL);
            if (aiRole) { /* 更新人物的发现时间 */
                aiRole->foundTimestamp = tv;
            } else { /* 初次发现人物 */
                bool isEnemy = role->getRoleType() == MCRole::MCEnemy && role_->getRoleType() != MCRole::MCEnemy;
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
    ai->lastActivationTime_ = 0;
    
    return ai;
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
MCAI::roleForObjectId(mc_object_id_t anObjectId)
{
    CCObject *obj;
    int key = MCObjectIdToDickKey(anObjectId);
    
    obj = rolesInVision_->objectForKey(key);
    if (! obj) {
        obj = enemiesInVision_->objectForKey(key);
    }
    
    return dynamic_cast<MCAIRole *>(obj);
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
