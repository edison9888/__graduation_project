//
//  MCActionBar.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCActionBar.h"

const char *kItemBackgroundFilepath = "UI/abi_item_background.png";
const char *kItemBoxFilepath = "UI/abi_item_box.png";

#pragma mark *** MCActionBarItem ***

bool
MCActionBarItem::init(MCBackpackItem *aBackpackItem)
{
    if (CCSprite::initWithFile(kItemBackgroundFilepath)) {
        CCPoint anchorPoint = ccp(0, 1);
        
        CCSize itemBoxSize = CCSizeMake(72, 72);
        CCRect itemBoxSourceRect = CCRectMake(0, 0, 32, 32);
        CCRect itemBoxInseRect = CCRectMake(11, 11, 20, 20);
        
        itemBox_ = CCScale9Sprite::create(kItemBoxFilepath, itemBoxSourceRect, itemBoxInseRect);
        addChild(itemBox_);
        itemBox_->setContentSize(itemBoxSize);
        itemBox_->setAnchorPoint(anchorPoint);
        itemBox_->setPosition(ccp(0, 0));
        
        backpackItem_ = aBackpackItem;
        
        return true;
    }
    
    return false;
}

MCActionBarItem *
MCActionBarItem::create(MCBackpackItem *aBackpackItem)
{
    MCActionBarItem *item = new MCActionBarItem;
    
    if (item && item->init(aBackpackItem)) {
        item->autorelease();
    } else {
        CC_SAFE_DELETE(item);
        item = NULL;
    }
    
    return item;
}

#pragma mark -
#pragma mark *** MCActionBar ***
