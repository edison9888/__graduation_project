//
//  MCEffectiveItem.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEffectiveItem.h"

MCEffectiveItem::~MCEffectiveItem()
{
    CC_SAFE_RELEASE(path_);
}

bool
MCEffectiveItem::init()
{
    if (MCItem::init()) {
        hp = 0;
        pp = 0;
        positive_state = 0;
        negative_state = 0;
        
        return true;
    }
    
    return false;
}

CCObject *
MCEffectiveItem::copy()
{

    MCEffectiveItem *effectiveItem = new MCEffectiveItem;
    
    // CCLog("%d, %c%c%c%c", __LINE__, id_.class_,id_.sub_class_,id_.index_,id_.sub_index_);
    effectiveItem->id_ = id_;
    // CCLog("%d, %c%c%c%c", __LINE__, effectiveItem->id_.class_,effectiveItem->id_.sub_class_,effectiveItem->id_.index_,effectiveItem->id_.sub_index_);
    effectiveItem->tag_ = tag_;
    effectiveItem->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    effectiveItem->name_->retain();
    if (description_ != NULL) {
        effectiveItem->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
        effectiveItem->description_->retain();
    }
    effectiveItem->price_ = price_;
    effectiveItem->itemType_ = itemType_;
    effectiveItem->icon_ = CCString::create(icon_->getCString()); /* 会被释放掉，所以要copy一个 */
    effectiveItem->icon_->retain();
    effectiveItem->radius_ = radius_;
    effectiveItem->path_ = CCString::create(path_->getCString()); /* 会被释放掉，所以要copy一个 */
    effectiveItem->path_->retain();
    
    /* effect */
    effectiveItem->hp = hp;             /* HP变化值 */
    effectiveItem->pp = pp;             /* PP变化值 */
    effectiveItem->positive_state = positive_state; /* 会增加的状态 */
    effectiveItem->negative_state = negative_state; /* 会消除的状态 */
    effectiveItem->adjusted_hp = adjusted_hp;    /* 一次调度后hp的变化值 */
    effectiveItem->adjusted_pp = adjusted_pp;    /* 一次调度后pp的变化值 */
    effectiveItem->lasting_time = lasting_time;   /* 效果时间 */
    
    return effectiveItem;
}
