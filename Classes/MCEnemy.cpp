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

MCEnemy::~MCEnemy()
{
    CC_SAFE_RELEASE(attackEffect_);
}

bool
MCEnemy::init(MCRoleRace aRoleRace)
{
    roleType_ = MCRole::MCEnemy;
    setRoleRace(aRoleRace);
    face_ = NULL;
    defaultDialogue_ = NULL;
    center_ = CCPointZero;
    
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
    float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
    
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

CCObject *
MCEnemy::copy()
{
    MCEnemy *enemy = new MCEnemy;
    
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
    enemy->maxHP_ = maxHP_;
    enemy->maxPP_ = maxPP_;
    enemy->exhaustion_ = exhaustion_;
    enemy->tired_ = tired_;
    enemy->dexterity_ = dexterity_;
    enemy->roleState_ = roleState_;
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
    
#warning 木有配置技能
    
    return enemy;
}

void
MCEnemy::roundDidFinish(CCObject *anObject)
{
    ai_->unactivate();
}
