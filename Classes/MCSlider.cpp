//
//  MCSlider.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCSlider.h"
#include <cocos-ext.h>
USING_NS_CC_EXT;

static const char *kMCRangeInputIncFilepath = "UI/ri_inc.png";
static const char *kMCRangeInputIncPressedFilepath = "UI/ri_inc_pressed.png";
static const char *kMCRangeInputDecFilepath = "UI/ri_dec.png";
static const char *kMCRangeInputDecPressedFilepath = "UI/ri_dec_pressed.png";

static const char *kMCSliderProgressFilepath = "UI/ri_slider_progress.png";
static const char *kMCSliderThumbFilepath = "UI/ri_slider_thumb.png";
static const char *kMCSliderTrackFilepath = "UI/ri_slider_track.png";

static const float kMCActionDuration = 0.1f;

static MCSlider *__default_slider = NULL;

MCSlider::~MCSlider()
{
}

bool
MCSlider::init()
{
    if (CCLayerColor::initWithColor(ccc4(32, 32, 32, 192))) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCLabelTTF *label;
        CCMenuItem *menuItem;
        
        CCControlSlider *slider = CCControlSlider::create(kMCSliderTrackFilepath,
                                                          kMCSliderProgressFilepath,
                                                          kMCSliderThumbFilepath);
        addChild(slider);
        slider->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        slider->setMinimumValue(0.0f); // Sets the min value of range
        slider->setMaximumValue(5.0f); // Sets the max value of range
		slider->setMaximumAllowedValue(4.0f);
		slider->setMinimumAllowedValue(1.5f);
		slider->setValue(3.0f);
        slider->addTargetWithActionForControlEvents(this,
                                                    cccontrol_selector(MCSlider::valueChanged),
                                                    CCControlEventValueChanged);
        slider_ = slider;
        
        CCPoint sliderPosition = slider->getPosition();
        CCMenu *menu = CCMenu::create();
        addChild(menu);
        menuItem = CCMenuItemImage::create(kMCRangeInputDecFilepath, kMCRangeInputDecPressedFilepath);
        menuItem->setTarget(this, menu_selector(MCSlider::dec));
        menu->addChild(menuItem);
        menu->setPosition(ccp(menuItem->getContentSize().width * 2,
                              sliderPosition.y));
        
        menu = CCMenu::create();
        addChild(menu);
        menuItem = CCMenuItemImage::create(kMCRangeInputIncFilepath, kMCRangeInputIncPressedFilepath);
        menuItem->setTarget(this, menu_selector(MCSlider::inc));
        menu->addChild(menuItem);
        menu->setPosition(ccp(winSize.width - menuItem->getContentSize().width * 2,
                              sliderPosition.y));
        
        label = CCLabelTTF::create("0", "", 36);
        addChild(label);
        sliderPosition.y += label->getContentSize().height;
        label->setPosition(sliderPosition);
        valueLabel_ = label;
        
        menu = CCMenu::create();
        addChild(menu);
        label = CCLabelTTF::create("确定", "", 36);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCSlider::ok));
        menu->addChild(menuItem);
        
        label = CCLabelTTF::create("取消", "", 36);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCSlider::cancel));
        menu->addChild(menuItem);
        
        menu->alignItemsHorizontally();
        menu->setPosition(ccp(winSize.width / 2, winSize.height / 4));
        slider->setTouchPriority(menu->getTouchPriority());
        
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, menu->getTouchPriority(), true);
        
        return true;
    }
    
    return false;
}

MCSlider *
MCSlider::defaultSlider(float aMinValue, float aMaxVale, float aDefaultValue)
{
    if (__default_slider == NULL) {
        __default_slider = new MCSlider;
        if (__default_slider && __default_slider->init()) {
        } else {
            delete __default_slider;
            __default_slider = NULL;
        }
    }
    __default_slider->setMinimumValue(aMinValue);
    __default_slider->setMaximumValue(aMaxVale);
    if (aDefaultValue < aMinValue) {
        aDefaultValue = aMinValue;
    } else if (aDefaultValue > aMaxVale) {
        aDefaultValue = aMaxVale;
    }
    __default_slider->setValue(aDefaultValue);
    
    CCString *ccstring;
    
    if (__default_slider->displayMethod_ == MCDisplayInteger) {
        ccstring = CCString::createWithFormat("%.0f", aDefaultValue);
    } else if (__default_slider->displayMethod_ == MCDisplayOneDecimalPlace) {
        ccstring = CCString::createWithFormat("%.1f", aDefaultValue);
    } else {
        ccstring = CCString::createWithFormat("%.2f", aDefaultValue);
    }
    
    __default_slider->valueLabel_->setString(ccstring->getCString());
    
    return __default_slider;
}

void
MCSlider::attach(CCNode *aParent)
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
MCSlider::detach()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    runAction(CCSequence::createWithTwoActions(CCMoveTo::create(kMCActionDuration, ccp(0, winSize.height)),
                                               CCCallFunc::create(this, callfunc_selector(MCSlider::destroy))));
}

float
MCSlider::getValue()
{
    return slider_->getValue();
}

void
MCSlider::setValue(float val)
{
    slider_->setValue(val);
}

float
MCSlider::getMinimumValue()
{
    return slider_->getMinimumValue();
}

void
MCSlider::setMinimumValue(float val)
{
    slider_->setMinimumValue(val);
}

float
MCSlider::getMaximumValue()
{
    return slider_->getMaximumValue();
}

void
MCSlider::setMaximumValue(float val)
{
    slider_->setMaximumValue(val);
}

bool
MCSlider::isEnabled()
{
    return slider_->isEnabled();
}

void
MCSlider::setEnabled(bool enabled)
{
    slider_->setEnabled(enabled);
}

float
MCSlider::getMinimumAllowedValue()
{
    return slider_->getMinimumAllowedValue();
}

void
MCSlider::setMinimumAllowedValue(float val)
{
    slider_->setMinimumAllowedValue(val);
}

float
MCSlider::getMaximumAllowedValue()
{
    return slider_->getMaximumAllowedValue();
}

void
MCSlider::setMaximumAllowedValue(float val)
{
    slider_->setMaximumAllowedValue(val);
}

bool
MCSlider::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    /* 截取触摸事件 */
    return true;
}

void
MCSlider::inc(CCObject *obj)
{
    slider_->setValue(slider_->getValue() + 1.0f);
    if (slider_->getValue() > slider_->getMaximumAllowedValue()) {
         slider_->setValue(slider_->getMaximumAllowedValue());
    }
    valueChanged(obj, CCControlEventValueChanged);
}

void
MCSlider::dec(CCObject *obj)
{
    slider_->setValue(slider_->getValue() - 1.0f);
    if (slider_->getValue() < slider_->getMinimumAllowedValue()) {
        slider_->setValue(slider_->getMinimumAllowedValue());
    }
    valueChanged(obj, CCControlEventValueChanged);
}

void
MCSlider::valueChanged(CCObject *sender, CCControlEvent controlEvent)
{
    float value = slider_->getValue();
    CCString *ccstring;
    
    if (displayMethod_ == MCDisplayInteger) {
        ccstring = CCString::createWithFormat("%.0f", value);
    } else if (displayMethod_ == MCDisplayOneDecimalPlace) {
        ccstring = CCString::createWithFormat("%.1f", value);
    } else {
        ccstring = CCString::createWithFormat("%.2f", value);
    }
    
    valueLabel_->setString(ccstring->getCString());
}

void
MCSlider::ok(CCObject *obj)
{
    detach();
    if (delegate_) {
        delegate_->sliderDidOk(this);
    }
}

void
MCSlider::cancel(CCObject *obj)
{
    detach();
    if (delegate_) {
        delegate_->sliderDidCancel(this);
    }
}

void
MCSlider::destroy()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    removeFromParentAndCleanup(false);
}
