//
//  MCCallbackableParticleSystemQuad.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-4.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCCallbackableParticleSystemQuad__
#define __Military_Confrontation__MCCallbackableParticleSystemQuad__

#include <cocos2d.h>
USING_NS_CC;

class MCCallbackableParticleSystemQuad : public CCParticleSystemQuad {
public:
    MCCallbackableParticleSystemQuad()
    : target_(NULL)
    , selector_(NULL)
    , userObject_(NULL) {}
    
    static MCCallbackableParticleSystemQuad *create(const char *plistFile);
    
    void cleanup(void);
    
    inline void setCallback(CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserObject=NULL) {
        setTarget(aTarget);
        setSelector(aSelector);
        setUserObject(anUserObject);
    }
    
protected:
    CC_SYNTHESIZE_RETAIN(CCObject *, target_, Target);
    CC_SYNTHESIZE(SEL_CallFuncO, selector_, Selector);
    CC_SYNTHESIZE_RETAIN(CCObject *, userObject_, UserObject);
};

#endif /* defined(__Military_Confrontation__MCCallbackableParticleSystemQuad__) */
