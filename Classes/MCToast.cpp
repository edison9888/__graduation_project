//
//  MCToast.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-6.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCToast.h"

static const float kMCActionDuration = 0.1f;

static MCToast *__default_toast = NULL;

bool
MCToast::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        float contentScaleFactor = CCDirectorGetContentScaleFactor();
        
        CCRect skinRect = CCRectMake(0, 0, 128, 128);
        CCRect skinCapInsets = CCRectMake(0, 0, 128, 128);
        
        skin_ = CCScale9Sprite::create("dialog/wskin.png", skinRect, skinCapInsets);
        addChild(skin_);
        skin_->setContentSize(CCSizeMake(winSize.width - 4 / contentScaleFactor,
                                         winSize.height / 3));
        skin_->setPosition(ccp(winSize.width / 2 - 2 / contentScaleFactor,
                               winSize.height / 6));
        
        CCRect edgeRect = CCRectMake(0, 0, 64, 64);
        
        edge_ = CCScale9Sprite::create("dialog/edge.png", edgeRect);
        addChild(edge_);
        edge_->setContentSize(CCSizeMake(winSize.width, winSize.height / 3));
        edge_->setPosition(ccp(winSize.width / 2, winSize.height / 6));
        
        dialogue_ = CCLabelTTF::create();
        dialogue_->setFontName("Marker Felt");
        dialogue_->setFontSize(24);
        addChild(dialogue_);
        dialogue_->setDimensions(CCSizeMake(edge_->getContentSize().width - 32 / contentScaleFactor,
                                            0));
        dialogue_->setHorizontalAlignment(kCCTextAlignmentLeft);
        dialogue_->setAnchorPoint(ccp(0, 1)); /* 左上角为锚点 */
        dialogue_->setPosition(ccp(16 / contentScaleFactor,
                                   edge_->getContentSize().height - 16 / contentScaleFactor));
        
        return true;
    }
    
    return false;
}

MCToast *
MCToast::make(CCNode *aParent, const char *aMessage, MCToastLength aToastLength)
{
    MCToast *toast;
    
    if (__default_toast == NULL) {
        __default_toast = new MCToast;
        if (__default_toast && __default_toast->init()) {
        } else {
            delete __default_toast;
            __default_toast = NULL;
        }
    }
    toast = __default_toast;
    
    toast->parent_ = aParent;
    toast->message_ = CCString::create(aMessage);
    toast->message_->retain();
    toast->length_ = aToastLength;
    
    return toast;
}

void
MCToast::show()
{
    CCAssert(parent_ != NULL, "parent node is null!");
    dialogue_->setString(message_->getCString());
    CC_SAFE_RELEASE(message_);
    m_obPosition = ccp(0, -edge_->getContentSize().height);
    parent_->addChild(this);
    runAction(CCMoveTo::create(kMCActionDuration, CCPointZero));
    scheduleOnce(schedule_selector(MCToast::hide), length_);
}

void
MCToast::cancel()
{
    hide(NULL);
}

void
MCToast::hide(CCObject *obj)
{
    runAction(CCSequence::createWithTwoActions(CCMoveBy::create(kMCActionDuration,
                                                                ccp(0, -edge_->getContentSize().height)),
                                               CCCallFuncO::create(this,
                                                                   callfuncO_selector(MCToast::destroy),
                                                                   NULL)));
}

void
MCToast::destroy(CCObject *obj)
{
    removeFromParentAndCleanup(true);
}