//
//  MCEffectiveItem.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEffectiveItem.h"
#include "MCEffect.h"

MCEffectiveItem::~MCEffectiveItem()
{
    CC_SAFE_RELEASE(effect);
}

bool
MCEffectiveItem::init()
{
    if (MCItem::init()) {
        hp = 0;
        pp = 0;
        positive_state = 0;
        
        return true;
    }
    
    return false;
}

CCObject *
MCEffectiveItem::copy()
{

    MCEffectiveItem *effectiveItem = new MCEffectiveItem;
    
    effectiveItem->id_ = id_;
    effectiveItem->tag_ = tag_;
    effectiveItem->name_ = new CCString(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    if (description_ != NULL) {
        effectiveItem->description_ = new CCString(description_->getCString()); /* 会被释放掉，所以要copy一个 */
    }
    effectiveItem->price_ = price_;
    effectiveItem->itemType_ = itemType_;
    effectiveItem->icon_ = new CCString(icon_->getCString()); /* 会被释放掉，所以要copy一个 */
    effectiveItem->radius = radius;
    effectiveItem->effect = dynamic_cast<MCEffect *>(effect->copy());
    
    /* effect */
    effectiveItem->hp = hp;             /* HP变化值 */
    effectiveItem->pp = pp;             /* PP变化值 */
    effectiveItem->positive_state = positive_state; /* 会增加的状态 */
    effectiveItem->lasting_time = lasting_time;   /* 效果时间 */
    
    return effectiveItem;
}
