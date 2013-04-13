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
#include "MCSlider.h"
#include <cocos-ext.h>

USING_NS_CC_EXT;

class MCScene;
class MCBackpackItem;
class MCTableViewTextFieldCell;

extern const float kMCSellPercentage;

enum {
    MCUnknownOperation,
    MCBuyOperation,
    MCSellOperation
};
typedef mc_enum_t MCTradingOperation;

class MCTradingPropsUI : public CCLayer, public MCSliderDelegate, public CCTableViewDataSource, public CCTableViewDelegate {
public:
    bool init();
    
    void initPosition();
    void attach(MCScene *aScene);
    void detach();
    
    void sliderDidOk(MCSlider *aSlider);
    
    /* CCScrollViewDelegate */
    void scrollViewDidScroll(CCScrollView *view) {}
    void scrollViewDidZoom(CCScrollView *view) {}
    
    /* CCTableViewDataSource */
    CCSize cellSizeForTable(CCTableView *table);
    CCTableViewCell *tableCellAtIndex(CCTableView *table, unsigned int idx);
    unsigned int numberOfCellsInTableView(CCTableView *table);
    
    /* CCTableViewDelegate */
    void tableCellTouched(CCTableView *table, CCTableViewCell *cell);
    
    CREATE_FUNC(MCTradingPropsUI);
    
protected:
    void buy_click(CCObject* aSender);
    void sell_click(CCObject* aSender);
    void backward(CCObject* aSender); /* 关闭.... */
    
    void destroy();
    void loadItem(CCNode *aBackpackItemObject);
    
    MCTradingOperation tradingOperation_;
    
    CCArray *effectiveItems_;
    
    CCTableView *tableView_; /* 装备列表 */
    CCSize tableViewSize_;
    MCTableViewTextFieldCell *selectedCell_;
    
    CCLabelTTF *name_;
    CCLabelTTF *description_;
    CCLabelTTF *count_;
    CCLabelTTF *price_;
    CCSprite *icon_;
    
    CCLabelTTF *money_;
    
    CCLayer *info_;
};

#endif /* defined(__Military_Confrontation__MCTradingPropsUI__) */
