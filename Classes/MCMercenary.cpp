//
//  MCMercenary.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCMercenary.h"
#include "MCHero.h"
#include "MCScene.h"
#include "MCEntrance.h"
#include "MCAStar.h"
#include "MCSkill.h"

MCMercenary::MCMercenary()
{
    mercenaryType_ = MCMercenary::MCNormalMercenary;
}

MCMercenary::~MCMercenary()
{
    
}

bool
MCMercenary::init()
{
    MCEnemy::init(MCTerrans);
    roleType_ = MCRole::MCMercenary;
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

#pragma mark -
#pragma mark *** MCMercenary::MCAIStateMachineDelegate ***

/**
 * 空闲状态下回调
 */
void
MCMercenary::performWhenIdleState()
{
    MCRole::performWhenIdleState();
}

MCRoleEntity *
MCMercenary::getEntity()
{
    MCRoleEntity *roleEntity = MCRole::getEntity();
    
    /* AI */
    if (ai_ == NULL) {
        ai_ = MCMercenaryAI::create();
        ai_->retain();
        ai_->bind(this);
    }
    
    return roleEntity;
}

/**
 * 死亡
 */
void
MCMercenary::died()
{
    MCRoleEntity *roleEntity = getEntity();
    roleEntity->stopPathFinding();
    roleEntity->stopWalking();
    roleEntity->getSpriteSheet()->removeFromParentAndCleanup(true);
    entity_ = NULL;
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCRoleDiedNotification,
                                                                       this);
}

CCObject *
MCMercenary::copy()
{
    MCMercenary *mercenary = new MCMercenary;
    
    mercenary->init();
    mercenary->id_ = id_;
    mercenary->tag_ = tag_;
    mercenary->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    mercenary->name_->retain();
    mercenary->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
    mercenary->description_->retain();

    mercenary->roleType_ = roleType_;
    mercenary->roleRace_ = roleRace_;
    mercenary->hp_ = hp_;
    mercenary->pp_ = pp_;
    mercenary->maxHP_ = maxHP_;
    mercenary->maxPP_ = maxPP_;
    mercenary->exhaustion_ = exhaustion_;
    mercenary->tired_ = tired_;
    mercenary->dexterity_ = dexterity_;
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
    mercenary->attackEffect_ = dynamic_cast<MCEffect *>(attackEffect_->copy());
    mercenary->cost_ = cost_;
    if (ai_) {
        mercenary->ai_ = dynamic_cast<MCMercenaryAI *>(ai_->copy());
    }
    mercenary->trigger_ = trigger_;
    
    CCObject *obj;
    CCARRAY_FOREACH(skills_, obj) {
        mercenary->skills_->addObject(dynamic_cast<MCSkill *>(obj)->copy());
    }
    mercenary->damageScore_ = -1;
    
    return mercenary;
}

#pragma mark -
#pragma mark *** MCNervousMercenary ***

MCNervousMercenary::MCNervousMercenary()
{
    mercenaryType_ = MCMercenary::MCNervousMercenary;
}

MCNervousMercenary::~MCNervousMercenary()
{
}

MCNervousMercenary *
MCNervousMercenary::create(mc_object_id_t anObjectId)
{
    MCNervousMercenary *mercenary = new MCNervousMercenary;
    
    if (mercenary && mercenary->init()) {
        mercenary->setID(anObjectId);
        mercenary->autorelease();
    } else {
        CC_SAFE_DELETE(mercenary);
        mercenary = NULL;
    }
    
    return mercenary;
}

#pragma mark -
#pragma mark *** MCNervousMercenary::MCAIStateMachineDelegate ***

/**
 * 逃跑状态下回调
 */
void
MCNervousMercenary::performWhenFleeState()
{
    if (mercenaryType_ != MCMercenary::MCNervousMercenary) {
        return;
    }
    
    ai_->activate();
    
    MCSceneContext *context = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCEntrance *entrance;
    MCEntrance *nearestEntrance = NULL;
    CCDictionary *entrances = context->getScene()->getEntrances();
    CCDictElement *elem;
    MCRoleEntity *entity = getEntity();
    CCPoint currentPosition = entity->getPosition();
    float distance = 10000.0f; /* 够大了 */
    
    CCDICT_FOREACH(entrances, elem) {
        entrance = dynamic_cast<MCEntrance *>(elem->getObject());
        CCPoint origin = entrance->getFrame().origin;
        if (ccpLength(ccpSub(origin, currentPosition)) < distance) {
            nearestEntrance = entrance;
        }
    }
    
    if (nearestEntrance) {
        entity->findPath(nearestEntrance->getFrame().origin);
    }
}

CCObject *
MCNervousMercenary::copy()
{
    MCNervousMercenary *mercenary = new MCNervousMercenary;
    
    mercenary->init();
    mercenary->id_ = id_;
    mercenary->tag_ = tag_;
    mercenary->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    mercenary->name_->retain();
    mercenary->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
    mercenary->description_->retain();
    
    mercenary->roleType_ = roleType_;
    mercenary->roleRace_ = roleRace_;
    mercenary->hp_ = hp_;
    mercenary->pp_ = pp_;
    mercenary->consume_ = consume_;
    mercenary->maxHP_ = maxHP_;
    mercenary->maxPP_ = maxPP_;
    mercenary->dying_ = dying_;
    mercenary->exhaustion_ = exhaustion_;
    mercenary->tired_ = tired_;
    mercenary->dexterity_ = dexterity_;
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
    mercenary->attackEffect_ = dynamic_cast<MCEffect *>(attackEffect_->copy());
    mercenary->cost_ = cost_;
    if (ai_) {
        mercenary->ai_ = dynamic_cast<MCMercenaryAI *>(ai_->copy());
    }
    mercenary->trigger_ = trigger_;
    
    CCObject *obj;
    CCARRAY_FOREACH(skills_, obj) {
        mercenary->skills_->addObject(dynamic_cast<MCSkill *>(obj)->copy());
    }
    mercenary->damageScore_ = -1;
    
    return mercenary;
}
