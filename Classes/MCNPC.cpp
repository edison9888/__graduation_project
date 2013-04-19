//
//  MCNPC.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCNPC.h"
#include "MCNPCAI.h"

MCNPC::MCNPC()
{
    roleType_ = MCRole::MCNPC;
}

MCNPC::~MCNPC()
{
    CC_SAFE_RELEASE(face_);
    CC_SAFE_RELEASE(spriteSheet_);
    CC_SAFE_RELEASE(defaultDialogue_);
//    CC_SAFE_DELETE(viewport_); /* 非CCObject子类 */
}

MCNPC *
MCNPC::create(mc_object_id_t anObjectId)
{
    MCNPC *npc = new MCNPC;
    
    if (npc && npc->init()) {
        npc->setID(anObjectId);
        npc->autorelease();
        /* 加载NPC数据 */
    } else {
        CC_SAFE_DELETE(npc);
        npc = NULL;
    }
    
    return npc;
}

MCRoleEntity *
MCNPC::getEntity()
{
    MCRoleEntity *roleEntity = MCRole::getEntity();
    
    /* AI */
    if (ai_ == NULL) {
        ai_ = MCNPCAI::create();
        ai_->retain();
        ai_->bind(this);
    }
    
    return roleEntity;
}

CCObject *
MCNPC::copy()
{
    MCNPC *npc = new MCNPC;
    
    npc->id_ = id_;
    npc->tag_ = tag_;
    npc->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    npc->name_->retain();
    npc->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
    npc->description_->retain();
    
    npc->roleType_ = roleType_;
    npc->roleRace_ = roleRace_;
    npc->hp_ = hp_;
    npc->pp_ = pp_;
    npc->maxHP_ = maxHP_;
    npc->maxPP_ = maxPP_;
    npc->exhaustion_ = exhaustion_;
    npc->tired_ = tired_;
    npc->roleState_ = roleState_;
    npc->face_ = CCString::create(face_->getCString()); /* 会被释放掉，所以要copy一个 */
    npc->face_->retain();
    npc->spriteSheet_ = CCString::create(spriteSheet_->getCString()); /* 会被释放掉，所以要copy一个 */
    npc->spriteSheet_->retain();
    npc->defaultDialogue_ = CCString::create(defaultDialogue_->getCString()); /* 会被释放掉，所以要copy一个 */
    npc->defaultDialogue_->retain();
    if (ai_) {
        npc->ai_ = dynamic_cast<MCNPCAI *>(ai_->copy());
    }
    npc->trigger_ = trigger_;
    
    return npc;
}
