//
//  MCEffect.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-9.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEffect__
#define __Military_Confrontation__MCEffect__

#include "MCType.h"
#include "MCSkill.h"

#include <cocos2d.h>
USING_NS_CC;

class MCRole;

class MCEffect : public CCObject {
    friend class MCEffectManager;
public:
    MCEffect()
    : id_(NULLObjectId)
    , effect_(NULL)
    , animation_(NULL)
    , implType_(MCUnknownEffectImplType)
    , binding_(NULL)
    , isReleaseEffect_(false)
    , target_(NULL)
    , selector_(NULL)
    , userObject_(NULL) {}
    
    ~MCEffect();

    enum {
        MCUnknownEffectImplType,
        MCSpriteSheet,
        MCPList /* 粒子效果 */
    };
    typedef mc_enum_t MCEffectImplType;
    
    void attach(CCNode *aParent, MCRole *aRole, CCObject *aTarget=NULL, SEL_CallFuncO aSelector=NULL, CCObject *anUserObject=NULL);
    
    void bind(MCSkill *aSkill);
    MCSkill *unbind();
    
    CCObject *copy();
    
    inline bool isReleaseEffect() {
        return isReleaseEffect_;
    }
    
protected:
    void detach(CCObject *anObject);
    void detachParticles(CCObject *anObject);
    
    CC_SYNTHESIZE_READONLY(mc_object_id_t, id_, ID);
    
    /* MCSpriteSheet时为动画贴图的第一帧的名字，MCPList时为粒子效果的路径 */
    CC_SYNTHESIZE_READONLY(CCString *, effect_, Effect);
    
    /* MCSpriteSheet */
    CCAnimation *animation_;
    
    CC_SYNTHESIZE_READONLY(MCEffectImplType, implType_, ImplType);
    CC_SYNTHESIZE_READONLY(float, duration_, Duration);
    CC_SYNTHESIZE_READONLY(MCSkill *, binding_, Binding);
    
    bool isReleaseEffect_;
    
    CCObject *target_;
    SEL_CallFuncO selector_;
    CCObject *userObject_;
};

#endif /* defined(__Military_Confrontation__MCEffect__) */
