//
//  MCRoleEntity.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCRoleEntity.h"
#include "MCMicsUtil.h"
#include "MCScene.h"

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
    requirements_ = CCArray::create();
    requirements_->retain();
}

MCRoleEntityMetadata::~MCRoleEntityMetadata()
{
    CC_SAFE_RELEASE(requirements_);
    
    CC_SAFE_RELEASE(spriteSheet_);
    
    CC_SAFE_RELEASE(animationGoUp_);
    CC_SAFE_RELEASE(animationGoDown_);
    CC_SAFE_RELEASE(animationGoLeft_);
    CC_SAFE_RELEASE(animationGoRight_);
}

MCRoleEntity::MCRoleEntity()
{
    moveToActions_ = new CCArray;
    moveToActions_->init();
}

MCRoleEntity::~MCRoleEntity()
{
    CC_SAFE_RELEASE(moveToActions_);
}

void
MCRoleEntity::onEnter()
{
    CCSprite::onEnter();
    schedule(schedule_selector(MCRoleEntity::update));
}

void
MCRoleEntity::onExit()
{
    CCSprite::onExit();
    unschedule(schedule_selector(MCRoleEntity::update));
}

void
MCRoleEntity::update(float dt)
{
#warning search roles
//    MCRole *role = role_;
}

const MCOBB &
MCRoleEntity::getOBB()
{
    CCPoint origin = getPosition();
    CCSize size = getContentSize();
    CCPoint center(origin.x + size.width / 2,
                   origin.y + size.height / 4);
    
    /* 加上地图的偏移值 */
    MCSceneContext *currentContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    if (currentContext) {
        center = ccpSub(center,
                        currentContext->getScene()->getMapOffset());
    }
    obb_.setup(center, size.width, size.height / 2, 0);
    
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
MCRoleEntity::moveBy(CCPoint &aDelta)
{
    CCAction *action = CCSequence::create(CCMoveBy::create(kMCDurationHero, aDelta),
                                          __mc_dumy::create(),
                                          NULL);
    moveToActions_->addObject(action);
    runAction(action);
}

void
MCRoleEntity::walkOnScreen(const CCPoint &aDestinationPosition, const CCPoint &offset)
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
    
    action = CCSequence::create(CCMoveBy::create(kMCDurationHero, aDestinationPosition),
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
MCRoleEntity::actionEnded(CCObject* anObject)
{
//    CCAction *moveToAction = (CCAction *)moveToActions_->lastObject();
//    if (moveToAction->isDone()) {
//        moveToActions_->removeLastObject();
//    }
}

void
MCRoleEntity::walkEnded()
{
    if (((CCAction *) moveToActions_->lastObject())->isDone() && isWalking()) {
        stopWalking();
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
