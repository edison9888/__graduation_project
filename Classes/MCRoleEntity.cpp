//
//  MCRoleEntity.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCRoleEntity.h"
#include "MCMiscUtil.h"
#include "MCScene.h"
#include "MCAStar.h"
#include "MCRoleManager.h"
#include "MCTaskManager.h"
#include "MCTaskContext.h"

static float kMCPathFindingMoveDuration = 0.2f;

static const int kMCWalkActionTag = 0x13;
static const int kMCWalkAnimationTag = 0x24;

MCRoleEntityMetadata::MCRoleEntityMetadata()
{
    spriteSheet_ = NULL;
    spriteSheetPath_ = NULL;
    
    animationGoUp_ = NULL;
    animationGoDown_ = NULL;
    animationGoLeft_ = NULL;
    animationGoRight_ = NULL;
    
    position_ = CCPointZero;
    frameSize_ = CCSizeZero;
    flags_ = CCArray::create();
    flags_->retain();
}

MCRoleEntityMetadata::~MCRoleEntityMetadata()
{
    CC_SAFE_RELEASE(flags_);
    
    CC_SAFE_RELEASE(spriteSheet_);
    
    CC_SAFE_RELEASE(animationGoUp_);
    CC_SAFE_RELEASE(animationGoDown_);
    CC_SAFE_RELEASE(animationGoLeft_);
    CC_SAFE_RELEASE(animationGoRight_);
}

MCRoleEntity::MCRoleEntity()
{
    moveToActions_ = CCArray::create();
    moveToActions_->retain();
    
    pathFindingAlgo_ = NULL;
    
    shadow_ = NULL;
    
    walkAction_ = NULL;
    
    lockPosition_ = false;
}

MCRoleEntity::~MCRoleEntity()
{
    CC_SAFE_RELEASE(pathFindingAlgo_);
    CC_SAFE_RELEASE(moveToActions_);
}

void
MCRoleEntity::onEnter()
{
    CCSprite::onEnter();
    schedule(schedule_selector(MCRoleEntity::update));
    startThinking();
}

void
MCRoleEntity::onExit()
{
    stopThinking();
    unschedule(schedule_selector(MCRoleEntity::update));
    CCSprite::onExit();
}

void
MCRoleEntity::update(float dt)
{
    CCSprite::update(dt);
    
    /* 更新效果影响 */
    for (std::vector<mc_effect_t>::iterator iterator = role_->effects_.begin();
         iterator != role_->effects_.end();
         ++iterator) {
        role_->hp_ += iterator->hp;
        if (role_->hp_ < 0) {
            role_->died();
        }
        iterator->remaining_time -= dt;
        if (iterator->remaining_time < 0) {
            role_->roleState_ &= ~iterator->positive_state;
            role_->effects_.erase(iterator);
        }
    }
}

const MCOBB &
MCRoleEntity::getOBB()
{
    CCPoint origin = getPosition();
    CCSize size = getContentSize();
    CCSize frameSize = metadata_->frameSize_;

    origin.x += (size.width - frameSize.width) / 2;
    CCPoint center(origin.x + frameSize.width / 2,
                   origin.y + frameSize.height / 2);
    /* 加上地图的偏移值 */
    MCSceneContext *currentContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    if (currentContext) {
        center = ccpSub(center,
                        currentContext->getScene()->getMapOffset());
    }
    obb_.setup(center, frameSize.width, frameSize.height, 0);

    return obb_;
}

/* AABB */
bool
MCRoleEntity::shouldBeSelected(const CCPoint &aPoint)
{
    CCPoint origin = getPosition();
    CCPoint offsetOrigin = ccpAdd(CCPointZero, origin);
    CCSize size = getContentSize();

    return CCRectMake(offsetOrigin.x, offsetOrigin.y,
                      size.width, size.height).containsPoint(aPoint);
}

void
MCRoleEntity::face(MCFacade aFacade)
{
    CCSpriteFrame *spriteFrame;
    
    if (aFacade == MCFacingUp) {
        spriteFrame = ((CCAnimationFrame *) metadata_->animationGoUp_->getFrames()->objectAtIndex(0))->getSpriteFrame();
    } else if (aFacade == MCFacingDown) {
        spriteFrame = ((CCAnimationFrame *) metadata_->animationGoDown_->getFrames()->objectAtIndex(0))->getSpriteFrame();
    } else if (aFacade == MCFacingLeft) {
        spriteFrame = ((CCAnimationFrame *) metadata_->animationGoLeft_->getFrames()->objectAtIndex(0))->getSpriteFrame();
    } else if (aFacade == MCFacingRight) {
        spriteFrame = ((CCAnimationFrame *) metadata_->animationGoRight_->getFrames()->objectAtIndex(0))->getSpriteFrame();
    } else {
        spriteFrame = NULL;
    }
    
    if (spriteFrame) {
        setDisplayFrame(spriteFrame);
        metadata_->facade_ = aFacade;
    }
}

void
MCRoleEntity::face(const CCPoint &delta)
{
    /* 疲惫ing，不接受命令 */
    if (role_->exhausted_) {
        return;
    }
    
    float angle;
    
        //每个方向分配60度角的空间
    MCGetAngleForPoint(delta, angle);
    if (angle < 22.5f) {
        if (delta.x > 0) {
            face(MCFacingRight);
        } else {
            face(MCFacingLeft);
        }
    } else if (angle < 67.5f) {
        if (delta.y > 0) {
            face(MCFacingUp);
        } else {
            face(MCFacingDown);
        }
    } else {
        if (delta.y > 0) {
            face(MCFacingUp);
        } else {
            face(MCFacingDown);
        }
    }
}

#pragma mark -
#pragma mark *** walk ***

void
MCRoleEntity::walk(MCFacade aFacade)
{
    /* 疲惫ing，不接受命令 */
    if (role_->exhausted_) {
        return;
    }
    
    CCAnimation *animation = NULL;
    CCRepeatForever *walkAction;
    
    /* 走向同一个方向将不创建新动作！*/
    if (metadata_->facade_ == aFacade
        && isWalking()) {
        return;
    }
    
    /* 停止上一个动作 */
    stopWalkAction();
    
    if (aFacade == MCFacingUp) {
        animation = metadata_->animationGoUp_;
    } else if (aFacade == MCFacingDown) {
        animation = metadata_->animationGoDown_;
    } else if (aFacade == MCFacingLeft) {
        animation = metadata_->animationGoLeft_;
    } else if (aFacade == MCFacingRight) {
        animation = metadata_->animationGoRight_;
    }
    if (animation != NULL) {
        CCAnimate *animate = CCAnimate::create(animation);
        
        walkAction = new CCRepeatForever;
        walkAction->initWithAction(animate);
        runAction(walkAction);
        walkAction_ = walkAction;
    }
    metadata_->facade_ = aFacade;
}

void
MCRoleEntity::walk(const CCPoint &delta)
{
    /* 疲惫ing，不接受命令 */
    if (role_->exhausted_) {
        return;
    }
    
    float angle;
    
    //每个方向分配60度角的空间
    MCGetAngleForPoint(delta, angle);
    if (angle < 22.5f) {
        if (delta.x > 0) {
            walk(MCFacingRight);
        } else {
            walk(MCFacingLeft);
        }
    } else if (angle < 67.5f) {
        if (delta.y > 0) {
            walk(MCFacingUp);
        } else {
            walk(MCFacingDown);
        }
    } else {
        if (delta.y > 0) {
            walk(MCFacingUp);
        } else {
            walk(MCFacingDown);
        }
    }
}

void
MCRoleEntity::drag(const CCPoint &aDelta)
{
    setPosition(ccpAdd(m_obPosition, aDelta));
}

/**
 * 积累步数消耗体力，用户战斗中移动
 * 1点体力/36像素
 */
void
MCRoleEntity::move(const CCPoint &aDelta)
{
    /* 疲惫ing，不接受命令 */
    if (role_->exhausted_
        || lockPosition_) {
        return;
    }
    
    float length = ccpLength(aDelta);
    
    setPosition(ccpAdd(m_obPosition, aDelta));
    
    MCTask *currentTask = MCTaskManager::sharedTaskManager()->getCurrentTask();
    
    if (currentTask != NULL
        && (currentTask->getTaskContext()->getSpecial() & MCIgnorePP) != MCIgnorePP) { /* 木有忽略PP消耗 */
        role_->pp_ -= length / (36 / CC_CONTENT_SCALE_FACTOR());
    }
}

bool
MCRoleEntity::canMove(float aLength)
{
    if (lockPosition_) {
        return false;
    }
    MCTask *currentTask = MCTaskManager::sharedTaskManager()->getCurrentTask();
    
    if (currentTask != NULL
        && (currentTask->getTaskContext()->getSpecial() & MCIgnorePP) == MCIgnorePP) { /* 已经忽略PP消耗 */
        return true;
    }
    
    mc_pp_t consume = aLength  / (36 / CC_CONTENT_SCALE_FACTOR());
    mc_pp_t remaining = role_->pp_ - consume;
    
    if (remaining > 0) {
        if (remaining < role_->exhaustion_) {
            role_->exhausted_ = true;
            MCAI *ai = role_->ai_;
            ai->setAIState(MCRestingState);
            ai->lockState();
            ai->unactivate();
        }
        return true;
    }
    
    return false;
}

bool
MCRoleEntity::isWalking()
{
    CCAction *walkAction = walkAction_;
    
    return walkAction && !walkAction->isDone();
}

void
MCRoleEntity::stopWalking()
{
    stopWalkAction();
    stopAllMoveToActions();
}

void
MCRoleEntity::stopWalkAction()
{
    CCAction *walkAction = walkAction_;
    
    if (walkAction && !walkAction->isDone()) {
        stopAction(walkAction);
        walkAction->release();
        walkAction_ = NULL;
    }
    
    face(metadata_->facade_);
}

void
MCRoleEntity::stopAllMoveToActions()
{
    CCArray *moveToActions = moveToActions_;
    CCAction *moveToAction;
    CCObject *obj;
    
    CCARRAY_FOREACH(moveToActions, obj) {
        moveToAction = dynamic_cast<CCAction *>(obj);
        if (! moveToAction->isDone()) {
            stopAction(moveToAction);
        }
    }
    moveToActions_->removeAllObjects();
}

/* 创建A星实例 */
MCAStarAlgorithm *
MCRoleEntity::pathFindingAlgoInstance()
{
    if (pathFindingAlgo_ == NULL) {
        MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
        MCScene *scene = sceneContext->getScene();
        MCAStar *aStar = scene->getAStar();
        pathFindingAlgo_ = aStar->createAlgoInstance(this);
    }
    
    return pathFindingAlgo_;
}

#pragma mark -
#pragma mark *** AI ***

void
MCRoleEntity::startThinking()
{
    schedule(schedule_selector(MCRoleEntity::thinking));
    schedule(schedule_selector(MCRoleEntity::checkObjects), 2.0f);
}

void
MCRoleEntity::stopThinking()
{
    unschedule(schedule_selector(MCRoleEntity::checkObjects));
    unschedule(schedule_selector(MCRoleEntity::thinking));
}

void
MCRoleEntity::thinking(float dt)
{
    role_->ai_->update(dt);
}

/* 调用AI的checkObjects */
void
MCRoleEntity::checkObjects(float dt)
{
    role_->ai_->checkObjects(dt);
}

void
MCRoleEntity::actionEnded(CCObject* anObject)
{
}

#pragma mark -
#pragma mark *** path finding ***

/**
 * 使用寻路算法
 * aDestinationLocation为屏幕上的坐标，所以要加上地图偏移
 */
void
MCRoleEntity::findPath(const CCPoint &aDestinationLocation)
{
    findPath(aDestinationLocation, NULL, NULL, NULL);
}

void
MCRoleEntity::findPath(const CCPoint &aDestinationLocation, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserdata)
{
    /* 疲惫ing，不接受命令 */
    if (role_->exhausted_
        || lockPosition_) {
        return;
    }
    
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = sceneContext->getScene();
    MCAStarAlgorithm *pathFindingAlgo = pathFindingAlgoInstance();
    
    target_ = aTarget;
    pathFindingSelector_ = aSelector;
    pathFindingSelectorUserdata_ = anUserdata;
    
    stopAllMoveToActions();
    
    pathFindingAlgo->stopPathFinding();
    pathFindingAlgo->setDestination(ccpSub(aDestinationLocation, scene->getMapOffset()));
    pathFindingAlgo->execute();
}

void
MCRoleEntity::findPathAtMap(const CCPoint &aDestinationLocation)
{
    findPathAtMap(aDestinationLocation, NULL, NULL, NULL);
}

void
MCRoleEntity::findPathAtMap(const CCPoint &aDestinationLocation, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserdata)
{
    /* 疲惫ing，不接受命令 */
    if (role_->exhausted_
        || lockPosition_) {
        return;
    }
    
    MCAStarAlgorithm *pathFindingAlgo = pathFindingAlgoInstance();
    
    target_ = aTarget;
    pathFindingSelector_ = aSelector;
    pathFindingSelectorUserdata_ = anUserdata;
    
    stopAllMoveToActions();
    
    pathFindingAlgo->stopPathFinding();
    pathFindingAlgo->setDestination(aDestinationLocation);
    pathFindingAlgo->execute();
}

void
MCRoleEntity::approachTarget(MCRole *aTarget)
{
    approachTarget(aTarget, NULL, NULL);
}

void
MCRoleEntity::approachTarget(MCRole *aTargetRole, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserdata)
{
    /* 疲惫ing，不接受命令 */
    if (role_->exhausted_
        || lockPosition_) {
        return;
    }
    
    MCRoleEntity *targetEntity = aTargetRole->getEntity();
    MCOBB targetOBB = targetEntity->getOBB();
    MCOBB selfOBB = getOBB();
    CCPoint offset = ccpSub(targetOBB.center, selfOBB.center);
    CCPoint targetPosition = ccp(targetOBB.center.x + selfOBB.width * (offset.x > 0 ? -1 : 1),
                                 targetOBB.center.y + selfOBB.height * (offset.y > 0 ? -1 : 1));
    
    role_->ai_->activate();
    findPathAtMap(targetPosition, aTarget, aSelector, anUserdata);
}

void
MCRoleEntity::approachTargetAndKeepDistance(MCRole *aTarget, mc_distance_t aDistance)
{
    approachTargetAndKeepDistance(aTarget, NULL, NULL);
}

void
MCRoleEntity::approachTargetAndKeepDistance(MCRole *aTargetRole, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserdata, mc_distance_t aDistance)
{
    /* 疲惫ing，不接受命令 */
    if (role_->exhausted_
        || lockPosition_) {
        return;
    }
    
    MCRoleEntity *targetEntity = aTargetRole->getEntity();
    MCOBB targetOBB = targetEntity->getOBB();
    MCOBB selfOBB = getOBB();
    CCPoint offset = ccpSub(targetOBB.center, selfOBB.center);
    float distance = ccpLength(offset) / selfOBB.width - 1;
    
    if (distance > (float) aDistance) {
        offset = ccpMult(offset, (distance - (float) aDistance) / distance);
    }
    CCPoint targetPosition = ccp(targetOBB.center.x + selfOBB.width * (offset.x > 0 ? -1 : 1),
                                 targetOBB.center.y + selfOBB.height * (offset.y > 0 ? -1 : 1));
    
    role_->ai_->activate();
    findPathAtMap(targetPosition, aTarget, aSelector, anUserdata);
}

void
MCRoleEntity::stopPathFinding()
{
    MCAStarAlgorithm *pathFindingAlgo = pathFindingAlgoInstance();
    pathFindingAlgo->stopPathFinding();
}

void
MCRoleEntity::destroyPathFindingAlgorithmInstance()
{
    if (pathFindingAlgo_) {
        CC_SAFE_RELEASE_NULL(pathFindingAlgo_);
    }
}

/**
 * 寻路结束
 */
void
MCRoleEntity::pathFindingDidFinish(CCObject *obj)
{
    walkWithPathFinding(obj);
}

void
MCRoleEntity::walkWithPathFinding(CCObject *algoObject)
{
    MCAStarAlgorithm *algo = dynamic_cast<MCAStarAlgorithm *>(algoObject);
    if (algo->route.size() > 0) {
        CCPoint target = algo->route.top(); /* 位于地图的位置 */
        CCPoint mapOffset = MCSceneContextManager::sharedSceneContextManager()->currentContext()->getScene()->getMapOffset();
        CCSequence *action = new CCSequence;
        CCPoint offset = ccpSub(ccpAdd(target, mapOffset), m_obPosition);
        float length = ccpLength(offset);
        
        if (! canMove(length) || role_->exhausted_) { /* 不能行走了 */
            stopPathFinding();
            stopWalking();
            return;
        }
        
        algo->route.pop();
        walk(offset);
        action->initWithTwoActions(CCMoveBy::create(kMCPathFindingMoveDuration,
                                                    offset),
                                   CCCallFuncO::create(this,
                                                       callfuncO_selector(MCRoleEntity::walkWithPathFinding),
                                                       algoObject));
        moveToActions_->addObject(action);
        action->release();
        runAction(action);
        
        MCTask *currentTask = MCTaskManager::sharedTaskManager()->getCurrentTask();
        if (currentTask != NULL
            && (currentTask->getTaskContext()->getSpecial() & MCIgnorePP) != MCIgnorePP) { /* 木有忽略PP消耗 */
            role_->pp_ -= length / (36 / CC_CONTENT_SCALE_FACTOR());
        }
    } else {
        if (target_ && pathFindingSelector_) {
            (target_->*pathFindingSelector_)(pathFindingSelectorUserdata_ ? pathFindingSelectorUserdata_ : this);
        }
        target_ = NULL;
        pathFindingSelector_ = NULL;
        pathFindingSelectorUserdata_ = NULL;
        stopWalking();
        role_->ai_->unactivate();
    }
}

CCSpriteBatchNode *
MCRoleEntity::getSpriteSheet()
{
    return metadata_->spriteSheet_;
}

MCFacade
MCRoleEntity::getFacade()
{
    return metadata_->facade_;
}

MCRoleEntityMetadata *
MCRoleEntity::getMetadata()
{
    return metadata_;
}
