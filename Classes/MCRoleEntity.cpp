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

static float kMCDurationHero = 0.1f;
static float kMCPathFindingMoveDuration = 0.1f;

class CC_DLL __mc_dumy : public CCActionInstant
{
public:
    __mc_dumy(){}
    virtual ~__mc_dumy() {}
        //super methods
    virtual void update(float time) { CC_UNUSED_PARAM(time); }
    
    static __mc_dumy *create() {
        __mc_dumy* pRet = new __mc_dumy;
        if (pRet) {
            pRet->autorelease();
        }
        
        return pRet;
    };
};

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
    
    moveToDestinations_ = CCArray::create();
    moveToDestinations_->retain();
    
    pathFindingAlgo_ = NULL;
    
    shadow_ = NULL;
}

MCRoleEntity::~MCRoleEntity()
{
    CC_SAFE_RELEASE(moveToDestinations_);
    CC_SAFE_RELEASE(moveToActions_);
}

void
MCRoleEntity::onEnter()
{
    CCSprite::onEnter();
    schedule(schedule_selector(MCRoleEntity::update));
    CCAssert(role_->ai_ != NULL, "没脑子");
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
    //warning: search roles
//    MCRole *role = role_;
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
MCRoleEntity::walk(MCFacade aFacade)
{
    CCAnimation *animation = NULL;
    CCAction *walkAction;
    
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
        walkAction = CCRepeatForever::create(CCAnimate::create(animation));
        walkAction->setTag(13);
        runAction(walkAction);
    }
    metadata_->facade_ = aFacade;
}

void
MCRoleEntity::walk(const CCPoint &delta)
{
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

/* 坑爹啊！直接moveby居然不行！ */
void
MCRoleEntity::moveBy(const CCPoint &aDelta)
{
    CCAction *action = CCSequence::create(CCMoveBy::create(kMCDurationHero, aDelta),
                                          __mc_dumy::create(),
                                          NULL);
    moveToActions_->addObject(action);
    runAction(action);
}

void
MCRoleEntity::drag(const CCPoint &aDelta)
{
    setPosition(ccpAdd(m_obPosition, aDelta));
}

bool
MCRoleEntity::isWalking()
{
    CCAction *walkAction = getActionByTag(13);
    
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
    CCAction *walkAction;
    
    walkAction = getActionByTag(13);
    if (walkAction && !walkAction->isDone()) {
        stopAction(walkAction);
        
        face(metadata_->facade_);
    }
}

void
MCRoleEntity::stopAllMoveToActions()
{
    CCAction *moveToAction;
    CCObject *obj;
    
    while ((obj = moveToActions_->lastObject()) != NULL) {
        moveToAction = (CCAction *)obj;
        if (!moveToAction->isDone()) {
            stopAction(moveToAction);
        }
        moveToActions_->removeLastObject();
    }
}

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
//    CCAction *moveToAction = (CCAction *)moveToActions_->lastObject();
//    if (moveToAction->isDone()) {
//        moveToActions_->removeLastObject();
//    }
}

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
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = sceneContext->getScene();
    MCAStar *aStar = scene->getAStar();
    
    aStar->findPath(this, ccpSub(aDestinationLocation, scene->getMapOffset()));
    
    target_ = aTarget;
    pathFindingSelector_ = aSelector;
    pathFindingSelectorUserdata_ = anUserdata;
}

/**
 * 测试某个位置是否能站
 * aDestinationLocation为屏幕上的坐标，所以要加上地图偏移
 */
bool
MCRoleEntity::testPosition(const CCPoint &aDestinationLocation)
{
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = sceneContext->getScene();
    MCAStar *aStar = scene->getAStar();
    
    return aStar->testPosition(this, ccpSub(aDestinationLocation, scene->getMapOffset()));
}

/**
 * 寻路结束
 */
void
MCRoleEntity::pathFindingDidFinish(CCObject *obj)
{
    CCNotificationCenter *notificatinCenter = CCNotificationCenter::sharedNotificationCenter();
    
    notificatinCenter->removeObserver(this, kMCAStarDidFinishAlgorithmNotification);
    
    walkWithPathFinding(obj);
}

void
MCRoleEntity::walkWithPathFinding(CCObject *algoObject)
{
    MCAStarAlgorithm *algo = dynamic_cast<MCAStarAlgorithm *>(algoObject);
    std::stack<CCPoint> *route = algo->route;
    if (route->size() > 0) {
        CCPoint target = route->top(); /* 位于地图的位置 */
        CCPoint mapOffset = MCSceneContextManager::sharedSceneContextManager()->currentContext()->getScene()->getMapOffset();
        CCFiniteTimeAction *action = NULL;
        CCPoint offset = ccpSub(ccpAdd(target, mapOffset), m_obPosition);
        
        route->pop();
        walk(offset);
        action = CCSequence::create(CCMoveBy::create(kMCPathFindingMoveDuration, offset),
                                    CCCallFuncO::create(this, callfuncO_selector(MCRoleEntity::walkWithPathFinding), algoObject),
                                    NULL);
        moveToActions_->addObject(action);
        runAction(action);
    } else {
        if (target_ && pathFindingSelector_) {
            (target_->*pathFindingSelector_)(pathFindingSelectorUserdata_ ? pathFindingSelectorUserdata_ : this);
        }
        target_ = NULL;
        pathFindingSelector_ = NULL;
        pathFindingSelectorUserdata_ = NULL;
        stopWalking();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCAStarAlgorithmWillRemoveNotification);
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
