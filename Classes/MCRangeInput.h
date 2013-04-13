//
//  MCRangeInput.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRangeInput__
#define __Military_Confrontation__MCRangeInput__

#include "MCScene.h"

class MCRangeInputDelegate;

class MCRangeInput : public CCLayerColor {
public:
    ~MCRangeInput();
    bool init();
    
    static MCRangeInput *defaultRangeInput(mc_size_t aMinValue, mc_size_t aMaxVale, mc_size_t aDefaultValue);
    
    /* 不是附加到scene中用的，应该附加到层上 */
    void attach(CCNode *aParent);
    void detach();
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void inc(CCObject *obj);
    void dec(CCObject *obj);
    
    void ok(CCObject *obj);
    void cancel(CCObject *obj);
    
    void destroy();
    
    CCLabelTTF *valueLabel_;
    int priority_;

    CC_SYNTHESIZE(mc_size_t, minValue_, MinValue);
    CC_SYNTHESIZE(mc_size_t, maxValue_, MaxValue);
    CC_SYNTHESIZE(mc_size_t, value_, Value);
    CC_SYNTHESIZE(MCRangeInputDelegate *, delegate_, Delegate);
};

class MCRangeInputDelegate {
public:
    virtual void rangeInputDidSetValue(MCRangeInput *aRangeInput) {}
    virtual void rangeInputDidCancel(MCRangeInput *aRangeInput) {}
};

#endif /* defined(__Military_Confrontation__MCRangeInput__) */
