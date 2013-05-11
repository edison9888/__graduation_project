//
//  MCMercenaryLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMercenaryLayer__
#define __Military_Confrontation__MCMercenaryLayer__

#include "MCBaseInfoLayer.h"
#include "JsonBox.h"
#include "MCConfirm.h"

#include <cocos-ext.h>

USING_NS_CC_EXT;

class MCMercenary;
class MCTableViewTextFieldCell;

class MCMercenaryLayer : public MCBaseInfoLayer, MCConfirmDelegate, public CCTableViewDataSource, public CCTableViewDelegate {
public:
    bool init();
    
    /* CCScrollViewDelegate */
    void scrollViewDidScroll(CCScrollView *view) {}
    void scrollViewDidZoom(CCScrollView *view) {}
    
    /* CCTableViewDataSource */
    CCSize cellSizeForTable(CCTableView *table);
    CCTableViewCell *tableCellAtIndex(CCTableView *table, unsigned int idx);
    unsigned int numberOfCellsInTableView(CCTableView *table);
    
    /* CCTableViewDelegate */
    void tableCellTouched(CCTableView *table, CCTableViewCell *cell) {}
    
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    
    CREATE_FUNC(MCMercenaryLayer);
    
protected:
    void loadData();
    
    void mercenary_click(CCObject *anObject);
    void dismiss_click(CCObject *anObject);
    
private:
    CCMenu *teamMenu_;
    CCMenu *dismiss_;
    
    CCTableView *tableView_;

    MCMercenary *selectedMercenary_;
    
    JsonBox::Array content_;
};


#endif /* defined(__Military_Confrontation__MCMercenaryLayer__) */
