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

#define kMCDurationHero  0.025f

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
    /* deprecated */
//    CCPoint origin = getPosition();
//    CCSize size = getContentSize();
//    CCPoint center(origin.x + size.width / 2,
//                   origin.y + size.height / 4);
//    
//    /* 加上地图的偏移值 */
//    MCSceneContext *currentContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
//    if (currentContext) {
//        center = ccpSub(center,
//                        currentContext->getScene()->getMapOffset());
//    }
//    obb_.setup(center, size.width, size.height / 2, 0);
    
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
    setDisplayFrame(((CCAnimationFrame *) metadata_->animationGoDown_->getFrames()->objectAtIndex(0))->getSpriteFrame());
    metadata_->facade_ = aFacade;
}

void
MCRoleEntity::walk(MCFacade aFacade)
{
    CCAnimation *animation = NULL;
    CCRepeatForever *walkAction;
    
    /* 走向同一个方向将不创建新动作！*/
    if (metadata_->facade_ == aFacade
        && isWalking()) {
        return;
    }
    
    /* 停止上一个动作 */
    if (isWalking()) {
        stopWalking();
    }
    
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
MCRoleEntity::walkTo(CCPoint &aDestinationPosition)
{
    CCFiniteTimeAction *action = CCSequence::create(CCMoveTo::create(kMCDurationHero, aDestinationPosition),
                                                    CCCallFunc::create(this, callfunc_selector(MCRoleEntity::walkEnded)),
                                                    NULL);
    float angle;
    CCPoint offset = ccpSub(aDestinationPosition, m_obPosition);
    //每个方向分配60度角的空间
    MCGetAngleForPoint(offset, angle);
    if (angle < 22.5f) {
        if (offset.x > 0) {
            walk(MCFacingRight);
        } else {
            walk(MCFacingLeft);
        }
    } else if (angle < 67.5f) {
        if (offset.y > 0) {
            walk(MCFacingUp);
        } else {
            walk(MCFacingDown);
        }
    } else {
        if (offset.y > 0) {
            walk(MCFacingUp);
        } else {
            walk(MCFacingDown);
        }
    }
    moveToActions_->addObject(action);
    runAction(action);
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

void
MCRoleEntity::walkOnScreen(const CCPoint &aDestinationLocation, const CCPoint &offset)
{
    CCFiniteTimeAction *action = NULL;
    float angle;
    
    //每个方向分配60度角的空间
    MCGetAngleForPoint(offset, angle);
    if (angle < 22.5f) {
        if (offset.x > 0) {
            walk(MCFacingRight);
        } else {
            walk(MCFacingLeft);
        }
    } else if (angle < 67.5f) {
        if (offset.y > 0) {
            walk(MCFacingUp);
        } else {
            walk(MCFacingDown);
        }
    } else {
        if (offset.y > 0) {
            walk(MCFacingUp);
        } else {
            walk(MCFacingDown);
        }
    }
    
    action = CCSequence::create(CCMoveBy::create(kMCDurationHero, aDestinationLocation),
                                CCCallFunc::create(this, callfunc_selector(MCRoleEntity::walkEnded)),
                                NULL);
    moveToActions_->addObject(action);
    runAction(action);
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
    CCAnimation *animation = NULL;
    MCFacade facade = metadata_->facade_;
    CCAction *walkAction;
    
    walkAction = getActionByTag(13);
    if (walkAction && !walkAction->isDone()) {
        stopAction(walkAction);
        
        if (facade == MCFacingUp) {
            animation = metadata_->animationGoUp_;
        } else if (facade == MCFacingDown) {
            animation = metadata_->animationGoDown_;
        } else if (facade == MCFacingLeft) {
            animation = metadata_->animationGoLeft_;
        } else if (facade == MCFacingRight) {
            animation = metadata_->animationGoRight_;
        }
        
        if (animation) {
            setDisplayFrame(((CCAnimationFrame *) animation->getFrames()->objectAtIndex(0))->getSpriteFrame());
        }
    }
    stopAllMoveToActions();
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
 */
void
MCRoleEntity::findPath(const CCPoint &aDestinationLocation)
{
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCAStar *aStar = sceneContext->getScene()->getAStar();
    aStar->findPath(getPrototype(), aDestinationLocation);
}

/**
 * 寻路结束
 */
void
MCRoleEntity::findPathDidFinish(CCObject *obj)
{
//    CCArray *route = algo->getRoute();
//    CCPoint *position;
    CCNotificationCenter *notificatinCenter = CCNotificationCenter::sharedNotificationCenter();
    
    notificatinCenter->removeObserver(this, kMCAStarDidFinishAlgorithmNotification);
//    CCLog("%s::nodes: %d", __FILE__+76,route->count());
//    CCARRAY_FOREACH(route, obj) {
//        position = (CCPoint *) obj;
//        moveToDestinations_->addObject(position);
//    }
//    CC_SAFE_RELEASE(route);
    walkWithPathFinding(obj);
    
    //warning: goto destination
//    if (moveToDestinations_->count() > 0) {
//        CCArray *actions = CCArray::createWithCapacity(moveToDestinations_->count());
//        CCPoint *destination = (CCPoint *) moveToDestinations_->lastObject();
//        CCLog("%s::destination is: (%.0f %.0f)", __FILE__+76, destination->x, destination->y);
//        CCARRAY_FOREACH(moveToDestinations_, obj) {
//            destination = dynamic_cast<CCPoint *>(obj);
//            actions->addObject(CCMoveTo::create(0.1, *destination));
////            CCLog("%s::Point(%.0f %.0f)", __FILE__+76, destination->x, destination->y);
//        }
//        runAction(CCSequence::create(actions));
//    }
//    runAction(CCMoveTo::create(1, *((CCPoint *)moveToDestinations_->lastObject())));
}

void
MCRoleEntity::walkEnded()
{
    if (((CCAction *) moveToActions_->lastObject())->isDone() && isWalking()) {
        stopWalking();
    }
}

void
MCRoleEntity::walkWithPathFinding(CCObject *algoObject)
{
    MCAStarAlgorithm *algo = dynamic_cast<MCAStarAlgorithm *>(algoObject);
    std::stack<CCPoint> *route = algo->route;
    if (route->size() > 0) {
        CCPoint target = route->top();
        CCFiniteTimeAction *action = NULL;
        float angle;
        CCLog("target: <%.0f %.0f>", target.x, target.y);
        CCPoint offset = ccpSub(target, m_obPosition);
        
        route->pop();
        //每个方向分配60度角的空间
        MCGetAngleForPoint(offset, angle);
        if (angle < 22.5f) {
            if (offset.x > 0) {
                walk(MCFacingRight);
            } else {
                walk(MCFacingLeft);
            }
        } else if (angle < 67.5f) {
            if (offset.y > 0) {
                walk(MCFacingUp);
            } else {
                walk(MCFacingDown);
            }
        } else {
            if (offset.y > 0) {
                walk(MCFacingUp);
            } else {
                walk(MCFacingDown);
            }
        }
        action = CCSequence::create(CCMoveTo::create(kMCDurationHero, target),
                                    CCCallFunc::create(this, callfunc_selector(MCRoleEntity::walkEnded)),
                                    CCCallFuncO::create(this, callfuncO_selector(MCRoleEntity::walkWithPathFinding), algoObject),
                                    NULL);
        moveToActions_->addObject(action);
        runAction(action);
    } else {
        CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCAStarAlgorithmWillRemoveNotification);
    }
}

void
MCRoleEntity::walkPath(CCObject *obj)
{
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
