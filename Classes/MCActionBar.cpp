//
//  MCActionBar.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCActionBar.h"
#include "MCTaskManager.h"
#include "MCTaskContext.h"

const char *kItemBackgroundFilepath = "UI/abi_item_background.png";
const char *kItemBoxFilepath = "UI/abi_item_box.png";

#pragma mark *** MCActionBarItem ***

bool
MCActionBarItem::init(MCBackpackItem *aBackpackItem)
{
    if (CCSprite::initWithFile(aBackpackItem->item->getIcon()->getCString())) {
        CCPoint anchorPoint = ccp(0.5f, 0.0f);
        
//        CCSize itemBoxSize = CCSizeMake(72, 72);
//        CCRect itemBoxSourceRect = CCRectMake(0, 0, 32, 32);
//        CCRect itemBoxInseRect = CCRectMake(11, 11, 20, 20);
        
//        itemBox_ = CCScale9Sprite::create(kItemBoxFilepath, itemBoxSourceRect, itemBoxInseRect);
//        addChild(itemBox_);
//        itemBox_->setContentSize(itemBoxSize);
//        itemBox_->setAnchorPoint(anchorPoint);
//        itemBox_->setPosition(ccp(0, 0));
        
        count_ = CCLabelTTF::create(CCString::createWithFormat("%hi", aBackpackItem->count)->getCString(),
                                    "",
                                    24);
        addChild(count_);
        count_->setAnchorPoint(anchorPoint);
        count_->setColor(ccc3(96, 96, 96));
        count_->setPosition(ccp(this->getContentSize().width / 2, 2));
        
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

void 
MCActionBarItem::resetPosition()
{
    setPosition(itemPosition_);
}

CCPoint
MCActionBarItem::getItemPosition()
{
    return itemPosition_;
}

void
MCActionBarItem::setItemPosition(CCPoint var)
{
    setPosition(var);
    itemPosition_ = var;
}

#pragma mark -
#pragma mark *** MCActionBar ***

MCActionBar::~MCActionBar()
{
    CC_SAFE_RELEASE(trapWide_);
    CC_SAFE_RELEASE(trapDamage_);
    CC_SAFE_RELEASE(healthPotion_);
    CC_SAFE_RELEASE(physicalPotion_);
}

bool
MCActionBar::init()
{
    if (CCLayer::init()) {
        MCTaskContext *taskContxt = MCTaskManager::sharedTaskManager()->getCurrentTask()->getTaskContext();
        CCPoint anchorPoint = ccp(0.5f, 0.0f);
        
        trapWide_ = MCActionBarItem::create(taskContxt->getTrapWide());
        addChild(trapWide_);
        trapWide_->setAnchorPoint(anchorPoint);
        trapDamage_ = MCActionBarItem::create(taskContxt->getTrapDamage());
        addChild(trapDamage_);
        trapDamage_->setAnchorPoint(anchorPoint);
        healthPotion_ = MCActionBarItem::create(taskContxt->getHealthPotion());
        addChild(healthPotion_);
        healthPotion_->setAnchorPoint(anchorPoint);
        physicalPotion_ = MCActionBarItem::create(taskContxt->getPhysicalPotion());
        addChild(physicalPotion_);
        physicalPotion_->setAnchorPoint(anchorPoint);
        
        align();
        
        return true;
    }
    
    return false;
}

MCActionBarItem *
MCActionBar::itemForTouch(CCTouch *pTouch)
{
    CCPoint touchLocation = pTouch->getLocation();

    /* trapWide_ */
    CCPoint local = trapWide_->convertToNodeSpace(touchLocation);
    CCSize s = trapWide_->getContentSize();
    CCRect r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return trapWide_;
    }
    /* trapDamage_ */
    local = trapDamage_->convertToNodeSpace(touchLocation);
    s = trapDamage_->getContentSize();
    r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return trapDamage_;
    }
    /* healthPotion_ */
    local = healthPotion_->convertToNodeSpace(touchLocation);
    s = healthPotion_->getContentSize();
    r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return healthPotion_;
    }
    /* physicalPotion_ */
    local = physicalPotion_->convertToNodeSpace(touchLocation);
    s = physicalPotion_->getContentSize();
    r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return physicalPotion_;
    }
    
    return NULL;
}

void
MCActionBar::align()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    CCSize itemSize = trapWide_->getContentSize();
    CCPoint centerPoint = ccp(winSize.width / 2, 0);
    CCPoint itemPosition = ccp(centerPoint.x - itemSize.width * 1.5, centerPoint.y);
    
    trapWide_->setItemPosition(itemPosition);
    itemPosition = ccp(centerPoint.x - itemSize.width * 0.5, centerPoint.y);
    trapDamage_->setItemPosition(itemPosition);
    itemPosition = ccp(centerPoint.x + itemSize.width * 0.5, centerPoint.y);
    healthPotion_->setItemPosition(itemPosition);
    itemPosition = ccp(centerPoint.x + itemSize.width * 1.5, centerPoint.y);
    physicalPotion_->setItemPosition(itemPosition);
}
