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

#include <cocos2d.h>
USING_NS_CC;

class MCEffect : public CCSprite {
    friend class MCEffectManager;
public:
    MCEffect()
    : id_(NULLObjectId)
    , effect_(NULL)
    , animation_(NULL)
    , implType_(MCUnknownEffectImplType) {}
    
    ~MCEffect();

    enum {
        MCUnknownEffectImplType,
        MCSpriteSheet,
        MCPList
    };
    typedef mc_enum_t MCEffectImplType;
    
    void attach(CCNode *aParent);
    
    CCObject *copy();
    
protected:
    void runEffect();
    
    void detach();
    
    CC_SYNTHESIZE_READONLY(mc_object_id_t, id_, ID);
    
    /* MCSpriteSheet时为动画贴图的第一帧的名字，MCPList时为粒子效果的路径 */
    CC_SYNTHESIZE_READONLY(CCString *, effect_, Effect);
    
    /* MCSpriteSheet */
    CCAnimation *animation_;
    
    CC_SYNTHESIZE_READONLY(MCEffectImplType, implType_, ImplType);
};

#endif /* defined(__Military_Confrontation__MCEffect__) */
