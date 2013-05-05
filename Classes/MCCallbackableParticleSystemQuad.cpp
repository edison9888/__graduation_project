//
//  MCCallbackableParticleSystemQuad.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-4.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCCallbackableParticleSystemQuad.h"

MCCallbackableParticleSystemQuad *
MCCallbackableParticleSystemQuad::create(const char *plistFile)
{
    MCCallbackableParticleSystemQuad *pRet = new MCCallbackableParticleSystemQuad;
    
    if (pRet && pRet->initWithFile(plistFile)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    
    return pRet;
}

void
MCCallbackableParticleSystemQuad::cleanup()
{
    if (target_) {
        (target_->*selector_)(userObject_ ? userObject_ : this);
    }
    CCParticleSystemQuad::cleanup();
}
