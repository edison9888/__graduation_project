//
//  MCObject.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCObject__
#define __Military_Confrontation__MCObject__

#include <cocos2d.h>
#include "MCType.h"

USING_NS_CC;

/* 基本物件 */
class MCObject : public CCObject {
public:
    MCObject()
    : description_(NULL)
        , name_(NULL)
        , id_(NULLObjectId)
        , tag_(CC_INVALID_INDEX) {}
    
    ~MCObject() {
        CC_SAFE_RELEASE(description_);
        CC_SAFE_RELEASE(name_);
    }
    
    CC_SYNTHESIZE(mc_object_id_t, id_, ID); /* 物件ID */
    CC_SYNTHESIZE(mc_tag_t, tag_, Tag); /* 物件标签 */
    CC_SYNTHESIZE(CCString *, name_, Name); /* 物件名称 */
    CC_SYNTHESIZE(CCString *, description_, Description); /* 物件描述 */
};

#endif /* defined(__Military_Confrontation__MCObject__) */
