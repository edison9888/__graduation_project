//
//  MCSlider.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRangeInput__
#define __Military_Confrontation__MCRangeInput__

#include "MCScene.h"
#include <cocos-ext.h>
USING_NS_CC_EXT;

class MCSliderDelegate;

enum {
    MCDisplayInteger           = MCMakeEnum(0),
    MCDisplayOneDecimalPlace    = MCMakeEnum(1),
    MCDisplayTwoDecimalPlaces   = MCMakeEnum(2)
};
typedef mc_enum_t MCDisplayMethod;

class MCSlider : public CCLayerColor {
public:
    MCSlider()
    : delegate_(NULL)
    , displayMethod_(MCDisplayInteger) {}
    
    ~MCSlider();
    bool init();
    
    static MCSlider *defaultSlider(float aMinValue = 0.0f, float aMaxVale = 10.0f, float aDefaultValue = 0.0f);
    
    /* 不是附加到scene中用的，应该附加到层上 */
    void attach(CCNode *aParent);
    void detach();
    
    float getValue();
    void setValue(float val);
    
    float getMinimumValue();
    void setMinimumValue(float val);
    
    float getMaximumValue();
    void setMaximumValue(float val);
    
    virtual bool isEnabled();
    virtual void setEnabled(bool enabled);
    
    float getMinimumAllowedValue();
    void setMinimumAllowedValue(float val);
    
    float getMaximumAllowedValue();
    void setMaximumAllowedValue(float val);
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void inc(CCObject *obj);
    void dec(CCObject *obj);
    void valueChanged(CCObject *sender, CCControlEvent controlEvent);
    
    void ok(CCObject *obj);
    void cancel(CCObject *obj);
    
    void destroy();
    
    CCLabelTTF *valueLabel_;
    
    CCControlSlider *slider_;

    CC_SYNTHESIZE(MCSliderDelegate *, delegate_, Delegate);
    CC_SYNTHESIZE(MCDisplayMethod, displayMethod_, DisplayMethod);
};

class MCSliderDelegate {
public:
    virtual void sliderDidOk(MCSlider *aSlider) {}
    virtual void sliderDidCancel(MCSlider *aSlider) {}
};

#endif /* defined(__Military_Confrontation__MCRangeInput__) */
