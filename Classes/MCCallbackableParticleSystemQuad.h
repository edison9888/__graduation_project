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
    static MCCallbackableParticleSystemQuad *create(const char *plistFile);
    
    void cleanup(void);
    
    inline void setCallback(CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserObject=NULL) {
        target_ = aTarget;
        selector_ = aSelector;
        userObject_ = anUserObject;
    }
    
    inline CCObject *getTarget() {
        return target_;
    }
    
    inline SEL_CallFuncO getSelector() {
        return selector_;
    }
    
    inline CCObject *getUserObject() {
        return userObject_;
    }
    
protected:
    CCObject *target_;
    SEL_CallFuncO selector_;
    CCObject *userObject_;
};

#endif /* defined(__Military_Confrontation__MCCallbackableParticleSystemQuad__) */
