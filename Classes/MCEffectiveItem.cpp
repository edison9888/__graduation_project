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
        effect_.hp = 0;
        effect_.pp = 0;
        effect_.positive_state = 0;
        effect_.negative_state = 0;
        
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
    effectiveItem->effect_ = effect_;
    effectiveItem->radius_ = radius_;
    effectiveItem->path_ = CCString::create(path_->getCString()); /* 会被释放掉，所以要copy一个 */
    effectiveItem->path_->retain();
    
    return effectiveItem;
}
