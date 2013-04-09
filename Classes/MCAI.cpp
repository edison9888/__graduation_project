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
    rolesInVisions_ = CCArray::create();
    rolesInVisions_->retain();
    inVisions_ = CCDictionary::create();
    inVisions_->retain();
    lostVisions_ = CCDictionary::create();
    lostVisions_->retain();
}

MCAI::~MCAI()
{
    CC_SAFE_DELETE(vision_);
    CC_SAFE_RELEASE(rolesInVisions_);
    CC_SAFE_RELEASE(inVisions_);
    CC_SAFE_RELEASE(lostVisions_);
}

bool
MCAI::init()
{
    return true;
}

/* 绑定到人物 */
void
MCAI::bind(MCRole *aRole)
{
    role_ = aRole;
    delegate_ = aRole;
    MCRoleEntity *entity = aRole->getEntity();
    vision_ = MCVision::create(entity);
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

void
MCAI::update(float dt) /* 大脑在转动 */
{
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    CCArray *roles = sceneContext->getObjects();
    CCObject *obj;
    MCRole *role;
    MCAIRole *aiRole;
    struct cc_timeval tv;
    
    CCARRAY_FOREACH(roles, obj) {
        role = dynamic_cast<MCRole *>(obj);
        if (vision_->collideWith(role->getEntity())) {
            aiRole = dynamic_cast<MCAIRole *>(inVisions_->objectForKey(MCObjectIdToDickKey(role->getID())));
            CCTime::gettimeofdayCocos2d(&tv, NULL);
            if (aiRole) { /* 更新人物的发现时间 */
                aiRole->foundTimestamp = tv;
            } else { /* 初次发现人物 */
                aiRole = new MCAIRole;
                aiRole->autorelease();
                aiRole->role = role;
                aiRole->foundTimestamp = tv;
                aiRole->isEnemy = role->getRoleType() == MCRole::MCEnemy;
                aiRole->aggro = 0;
                delegate_->roleDidEnterVision(role, aiRole->isEnemy);
                inVisions_->setObject(aiRole, MCObjectIdToDickKey(role->getID()));
            }
        }
    }
}

/* 检查视野中的对象是否还在 */
void
MCAI::checkObjects(float dt)
{
    MCRole *role;
    MCAIRole *aiRole;
    struct cc_timeval tv;
    CCDictionary *roles = inVisions_;
    CCDictElement *elem;
    
    CCDICT_FOREACH(roles, elem) {
        aiRole = dynamic_cast<MCAIRole *>(elem->getObject());
        role = aiRole->role;
        CCTime::gettimeofdayCocos2d(&tv, NULL);
        if (! vision_->collideWith(role->getEntity())) { /* 人物离开了视野 */
            delegate_->roleDidExitVision(role, aiRole->isEnemy);
            int key = MCObjectIdToDickKey(role->getID());
            lostVisions_->setObject(aiRole, key);
            inVisions_->removeObjectForKey(key);
        }
    }
}

CCArray *
MCAI::rolesInVisions()
{
    MCRole *role;
    MCAIRole *aiRole;
    CCDictionary *roles = inVisions_;
    CCDictElement *elem;
    
    rolesInVisions_->removeAllObjects();
    CCDICT_FOREACH(roles, elem) {
        aiRole = dynamic_cast<MCAIRole *>(elem->getObject());
        role = aiRole->role;
        if (vision_->collideWith(role->getEntity())) { /* 人物离开了视野 */
            rolesInVisions_->addObject(role);
        }
    }
    
    return rolesInVisions_;
}
