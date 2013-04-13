//
//  MCTradingPropsUI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTradingPropsUI__
#define __Military_Confrontation__MCTradingPropsUI__

#include "MCType.h"
#include "MCRangeInput.h"

class MCScene;
class MCBackpackItem;

extern const float kMCSellPercentage;

enum {
    MCUnknownOperation,
    MCBuyOperation,
    MCSellOperation
};
typedef mc_enum_t MCTradingOperation;

class MCTradingPropsUI : public CCLayer, public MCRangeInputDelegate {
public:
    bool init();
    
    void initPosition();
    void attach(MCScene *aScene);
    void detach();
        
    void rangeInputDidSetValue(MCRangeInput *aRangeInput);
    
    CREATE_FUNC(MCTradingPropsUI);
    
protected:
    void item_clicked(CCObject *obj);
    void buy_click(CCObject* aSender);
    void sell_click(CCObject* aSender);
    void backward(CCObject* aSender); /* 关闭.... */
    
    void destroy();
    void loadItem(CCMenuItemLabel *aMenuItem);
    
    MCTradingOperation tradingOperation_;
    
    CCMenu *items_;
    CCMenuItemLabel *lastClickedItem_;
    
    CCLabelTTF *name_;
    CCLabelTTF *description_;
    CCLabelTTF *count_;
    CCLabelTTF *price_;
    CCSprite *icon_;
    
    CCLabelTTF *money_;
    
    CCMenuItemLabel *lastSelectedMenuItem_;
    CCLayer *info_;
};

#endif /* defined(__Military_Confrontation__MCTradingPropsUI__) */
