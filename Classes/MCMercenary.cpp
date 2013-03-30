//
//  MCMercenary.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCMercenary.h"


MCMercenary::~MCMercenary()
{
    
}

bool
MCMercenary::init()
{
    setRoleRace(MCTerrans);
    defaultDialogue_ = NULL;
    
    return true;
}

MCMercenary *
MCMercenary::create(mc_object_id_t anObjectId)
{
    MCMercenary *mercenary = new MCMercenary;
    
    if (mercenary && mercenary->init()) {
        mercenary->setID(anObjectId);
        mercenary->autorelease();
    } else {
        CC_SAFE_DELETE(mercenary);
        mercenary = NULL;
    }
    
    return mercenary;
}

CCObject *
MCMercenary::copy()
{
    MCMercenary *mercenary = new MCMercenary;
    
    mercenary->id_ = id_;
    mercenary->tag_ = tag_;
    mercenary->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    mercenary->name_->retain();
    mercenary->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
    mercenary->description_->retain();
    
    mercenary->roleRace_ = roleRace_;
    mercenary->hp_ = hp_;
    mercenary->pp_ = pp_;
    mercenary->maxHP_ = maxHP_;
    mercenary->maxPP_ = maxPP_;
    mercenary->roleState_ = roleState_;
    mercenary->face_ = CCString::create(face_->getCString()); /* 会被释放掉，所以要copy一个 */
    mercenary->face_->retain();
    mercenary->spriteSheet_ = CCString::create(spriteSheet_->getCString()); /* 会被释放掉，所以要copy一个 */
    mercenary->spriteSheet_->retain();
    mercenary->defaultDialogue_ = NULL;
    mercenary->ac_ = ac_;
    mercenary->armorCheckPenalty_ = armorCheckPenalty_;
    mercenary->damage_ = damage_;
    mercenary->damageBonus_ = damageBonus_;
    mercenary->criticalHitVisible_ = criticalHitVisible_;
    mercenary->criticalHitInvisible_ = criticalHitInvisible_;
    mercenary->criticalHit_ = criticalHit_;
    mercenary->distance_ = distance_;
    mercenary->effect_ = effect_;
    mercenary->effectCheck_ = effectCheck_;
    mercenary->cost_ = cost_;
    mercenary->ai_ = ai_;
    
    return mercenary;
}