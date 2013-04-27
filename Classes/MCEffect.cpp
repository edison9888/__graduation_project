//
//  MCEffect.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-9.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEffect.h"

MCEffect::~MCEffect()
{
    CC_SAFE_RELEASE(effect_);
    CC_SAFE_RELEASE(animation_);
}

void
MCEffect::attach(CCNode *aParent)
{
    aParent->addChild(this);
}

CCObject *
MCEffect::copy()
{
    MCEffect *effect = new MCEffect;
    
    effect->init();
    effect->id_ = id_;
    effect->effect_ = CCString::create(effect_->getCString()); /* 会被释放掉，所以要copy一个 */
    effect->effect_->retain();
    if (implType_ == MCEffect::MCSpriteSheet
        && animation_ != NULL) {
        effect->animation_ = animation_;
        animation_->retain();
    }
    
    return effect;
}

void
MCEffect::runEffect()
{
}

void
MCEffect::detach()
{
    
}