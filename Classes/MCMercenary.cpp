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
#include "MCAStar.h"

#if MC_DEBUG_SERVER == 1
#include "MCSimpleGameSceneContextServer.h"
static bool __log__ = true;
#endif

struct __mc_offset {
    int x;
    int y;
};

static struct __mc_offset __delta[] = {
    {-1, -1},
    {0, -1},
    {1, -1},
    
    {-1, 0},
    {1, 0},
    
    {-1, 1},
    {0, 1},
    {1, 1}
};

MCMercenary::~MCMercenary()
{
    
}

bool
MCMercenary::init()
{
    MCNPC::init();
    roleType_ = MCRole::MCMercenary;
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

#pragma mark -
#pragma mark *** MCAIStateMachineDelegate ***

/**
 * 空闲状态下回调
 */
void
MCMercenary::performWhenIdleState()
{
    MCRole::performWhenIdleState();
    /* 待改进 */
//    ai_->setAIState(MCFollowingState);
}

#pragma mark -
#pragma mark *** MCAIStateMachineDelegate ***

/**
 * 跟随状态下回调
 */
void
MCMercenary::performWhenFollowingState()
{
    /* 主角不在自己视野内才触发跟随，不科学是吧= - */
    CCDictionary *roles = ai_->getRolesInVision();
    MCRole *hero = MCHero::sharedHero();
    bool shouldFollow = true;
    
    if (roles->count() > 0) {
        CCDictElement *elem;
        
        CCDICT_FOREACH(roles, elem) {
            if (dynamic_cast<MCAIRole *>(elem->getObject())->role == hero) {
                shouldFollow = false;
                break;
            }
        }
    }
    CCPoint rolePosition = getEntity()->getPosition();
    CCPoint heroPosition = hero->getEntity()->getPosition();
    CCSize frameSize = getEntityMetadata()->getFrameSize();
    
    /* 距离超过4个身位才跟随 */
    if (shouldFollow
        && ccpLength(ccpSub(rolePosition, heroPosition)) > 4 * frameSize.width) {
        follow();
    }
}

/**
 * 逃跑状态下回调
 */
void
MCMercenary::performWhenFleeState()
{
    ai_->activate();
#warning flee!
}

void
MCMercenary::follow()
{
    ai_->activate();
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = sceneContext->getScene();
    MCAStar *aStar = scene->getAStar();
    MCRoleEntity *heroEntity = MCHero::sharedHero()->getEntity();
    MCRoleEntity *roleEntity = getEntity();
    CCPoint heroPosition = heroEntity->getPosition();
    MCOBB roleOBB = roleEntity->getOBB();
    
    /* 测试hero周围的八个位置能否前往 */
    for (mc_index_t i = 0; i < 8; ++i) {
        CCPoint checkPosition = ccp(heroPosition.x + __delta[i].x,
                                    heroPosition.y + __delta[i].y);
        if (aStar->testPosition(roleEntity, checkPosition)) {
            roleEntity->findPath(checkPosition, this, callfuncO_selector(MCMercenary::followingDidFinish));
            break;
        };
    }
}

void
MCMercenary::followingDidFinish(CCObject *anObject)
{
    CCLog("end following");
    ai_->unactivate();
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
    mercenary->cost_ = cost_;
    if (ai_) {
        mercenary->ai_ = dynamic_cast<MCMercenaryAI *>(ai_->copy());
    }
    mercenary->trigger_ = trigger_;
    
    return mercenary;
}
