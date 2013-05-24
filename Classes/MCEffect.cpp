//
//  MCEffect.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-9.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEffect.h"
#include "MCCallbackableParticleSystemQuad.h"
#include "MCRole.h"

MCEffect::~MCEffect()
{
    CC_SAFE_RELEASE(effect_);
    CC_SAFE_RELEASE(animation_);
}

void
MCEffect::attach(CCNode *aParent, MCRole *aRole, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserObject)
{
    if (implType_ == MCEffect::MCSpriteSheet) {
        MCRoleEntity *entity = aRole->getEntity();
        CCPoint center = entity->getPosition();
        CCSize size = entity->getContentSize();
        
        center.x += size.width / 2;
        center.y += size.height / 2;
        attach(aParent, center, aTarget, aSelector, anUserObject);
    } else if (implType_ == MCEffect::MCPList) {
        MCRoleEntity *entity = aRole->getEntity();
        CCPoint center = entity->getPosition();
        CCSize size = entity->getContentSize();
        
        center.x += size.width / 2;
        center.y += size.height / 2;
        attach(aParent, center, aTarget, aSelector, anUserObject);
    }
}

void
MCEffect::attach(CCNode *aParent, const CCPoint &aPosition, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserObject)
{
    target_ = aTarget;
    selector_ = aSelector;
    userObject_ = anUserObject;
    if (implType_ == MCEffect::MCSpriteSheet) {
        retain();
        CCAnimate *animate = CCAnimate::create(animation_);
        animation_->retain();
        CCSprite *effectSprite = CCSprite::createWithSpriteFrameName(effect_->getCString());
        
        effectSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        effectSprite->setPosition(aPosition);
        aParent->addChild(effectSprite);
        effectSprite->runAction(CCSequence::createWithTwoActions(animate,
                                                                 CCCallFuncO::create(this,
                                                                                     callfuncO_selector(MCEffect::detach),
                                                                                     effectSprite)));
    } else if (implType_ == MCEffect::MCPList) {
        retain();
        MCCallbackableParticleSystemQuad *particleEffect = new MCCallbackableParticleSystemQuad;
        
        particleEffect->initWithFile(effect_->getCString());
        particleEffect->setPosition(aPosition);
        particleEffect->setAutoRemoveOnFinish(true);
        particleEffect->setCallback(this, callfuncO_selector(MCEffect::detachParticles));
        if (binding_) {
//            if (isReleaseEffect_) {
            particleEffect->setLifeVar(binding_->releaseTime); /* 时间相当长啊 */
//            } else {
//                particleEffect->setDuration(binding_->activeTime);
//            }
        }
        aParent->addChild(particleEffect);
        particleEffect->release();
    }
}

void
MCEffect::bind(MCSkill *aSkill)
{
    binding_ = aSkill;
}

MCSkill *
MCEffect::unbind()
{
    MCSkill *binding = binding_;
    
    binding_ = NULL;
    
    return binding;
}

CCObject *
MCEffect::copy()
{
    MCEffect *effect = new MCEffect;
    
    effect->id_ = id_;
    effect->implType_ = implType_;
    effect->effect_ = new CCString(effect_->getCString()); /* 会被释放掉，所以要copy一个 */
    if (implType_ == MCEffect::MCSpriteSheet
        && animation_ != NULL) {
        effect->animation_ = animation_;
        animation_->retain();
    }
    effect->duration_ = duration_;
    
    return effect;
}

void
MCEffect::detach(CCObject *anObject)
{
    CCNode *node = dynamic_cast<CCNode *>(anObject);
    
    if (binding_) {
        binding_->getLauncher()->getEntity()->unlockPosition();
    }
    node->removeFromParentAndCleanup(true);
    if (target_) {
        (target_->*selector_)(userObject_ ? userObject_ : this);
    }
}

void
MCEffect::detachParticles(CCObject *anObject)
{
    if (binding_) {
        binding_->getLauncher()->getEntity()->unlockPosition();
    }
    if (target_ != NULL) {
        (target_->*selector_)(userObject_);
    }
    CC_SAFE_RELEASE(this);
}
