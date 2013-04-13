//
//  MCRangeInput.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCRangeInput.h"

static const char *kMCRangeInputIncFilepath = "UI/ri_inc.png";
static const char *kMCRangeInputIncPressedFilepath = "UI/ri_inc_pressed.png";
static const char *kMCRangeInputDecFilepath = "UI/ri_dec.png";
static const char *kMCRangeInputDecPressedFilepath = "UI/ri_dec_pressed.png";

static const float kMCActionDuration = 0.1f;

static MCRangeInput *__default_range_input = NULL;

MCRangeInput::~MCRangeInput()
{
}

bool
MCRangeInput::init()
{
    if (CCLayerColor::initWithColor(ccc4(32, 32, 32, 192))) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCMenu *menu = CCMenu::create();
        addChild(menu);
        
        CCMenuItem *menuItem;
        menuItem = CCMenuItemImage::create(kMCRangeInputDecFilepath, kMCRangeInputDecPressedFilepath);
        menuItem->setTarget(this, menu_selector(MCRangeInput::dec));
        menu->addChild(menuItem);
        
        CCLabelTTF *label = CCLabelTTF::create("0", "", 48);
        valueLabel_ = label;
        menuItem = CCMenuItemLabel::create(label);
        menuItem->setEnabled(false);
        menu->addChild(menuItem);
        
        menuItem = CCMenuItemImage::create(kMCRangeInputIncFilepath, kMCRangeInputIncPressedFilepath);
        menuItem->setTarget(this, menu_selector(MCRangeInput::inc));
        menu->addChild(menuItem);
        
        menu->alignItemsHorizontallyWithPadding(24);
        menu->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        
        menu = CCMenu::create();
        addChild(menu);
        
        label = CCLabelTTF::create("确定", "", 36);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCRangeInput::ok));
        menu->addChild(menuItem);
        
        label = CCLabelTTF::create("取消", "", 36);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCRangeInput::cancel));
        menu->addChild(menuItem);
        
        menu->alignItemsHorizontally();
        menu->setPosition(ccp(winSize.width / 2, winSize.height / 4));
        
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, menu->getTouchPriority(), true);
        
        return true;
    }
    
    return false;
}

MCRangeInput *
MCRangeInput::defaultRangeInput(mc_size_t aMinValue, mc_size_t aMaxVale, mc_size_t aDefaultValue)
{
    if (__default_range_input == NULL) {
        __default_range_input = new MCRangeInput;
        if (__default_range_input && __default_range_input->init()) {
        } else {
            delete __default_range_input;
            __default_range_input = NULL;
        }
    }
    __default_range_input->minValue_ = aMinValue;
    __default_range_input->maxValue_ = aMaxVale;
    if (aDefaultValue < aMinValue) {
        aDefaultValue = aMinValue;
    } else if (aDefaultValue > aMaxVale) {
        aDefaultValue = aMaxVale;
    }
    __default_range_input->value_ = aDefaultValue;
    __default_range_input->valueLabel_->setString(CCString::createWithFormat("%d", aDefaultValue)->getCString());
    
    return __default_range_input;
}

void
MCRangeInput::attach(CCNode *aParent)
{
    CCSize winSize = CCDirectorGetWindowsSize();

    if (m_pParent) {
        return;
    }
    aParent->addChild(this);
    setPosition(ccpFromSize(winSize));
    
    runAction(CCMoveTo::create(kMCActionDuration, CCPointZero));

}

void
MCRangeInput::detach()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    runAction(CCSequence::createWithTwoActions(CCMoveTo::create(kMCActionDuration, ccp(0, winSize.height)),
                                               CCCallFunc::create(this, callfunc_selector(MCRangeInput::destroy))));
}

bool
MCRangeInput::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    /* 截取触摸事件 */
    return true;
}

void
MCRangeInput::inc(CCObject *obj)
{
    if (value_ < maxValue_) {
        ++value_;
    }
    valueLabel_->setString(CCString::createWithFormat("%d", value_)->getCString());
}

void
MCRangeInput::dec(CCObject *obj)
{
    if (value_ > minValue_) {
        --value_;
    }
    valueLabel_->setString(CCString::createWithFormat("%d", value_)->getCString());
}

void
MCRangeInput::ok(CCObject *obj)
{
    detach();
    if (delegate_) {
        delegate_->rangeInputDidSetValue(this);
    }
}

void
MCRangeInput::cancel(CCObject *obj)
{
    detach();
    if (delegate_) {
        delegate_->rangeInputDidCancel(this);
    }
}

void
MCRangeInput::destroy()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    removeFromParentAndCleanup(false);
}
