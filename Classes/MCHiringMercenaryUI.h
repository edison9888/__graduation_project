//
//  MCHiringMercenaryUI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCHiringMercenaryUI__
#define __Military_Confrontation__MCHiringMercenaryUI__

#include <cocos2d.h>
#include <cocos-ext.h>
#include "MCType.h"
#include "JsonBox.h"
#include "MCConfirm.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MCScene;
class MCMercenary;
class MCTableViewTextFieldCell;

class MCHiringMercenaryUI : public CCLayer, public MCConfirmDelegate, public CCTableViewDataSource, public CCTableViewDelegate {
public:
    bool init();
    
    void initPosition();
    void attach(MCScene *aScene);
    void detach();
    
    /* CCScrollViewDelegate */
    void scrollViewDidScroll(CCScrollView *view) {}
    void scrollViewDidZoom(CCScrollView *view) {}
    
    /* CCTableViewDataSource */
    CCSize cellSizeForTable(CCTableView *table);
    CCTableViewCell *tableCellAtIndex(CCTableView *table, unsigned int idx);
    unsigned int numberOfCellsInTableView(CCTableView *table);
    
    /* CCTableViewDelegate */
    void tableCellTouched(CCTableView *table, CCTableViewCell *cell);
    
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    
    CREATE_FUNC(MCHiringMercenaryUI);
    
protected:
    void hire_click(CCObject* aSender);
    void changeMercenaryType(CCObject* aSender);
    void backward(CCObject* aSender); /* 关闭.... */
    
    void destroy();
    
private:
    CCTableView *tableView_; /* 佣兵列表 */
    CCTableView *contentTableView_;
    CCSize tableViewSize_;
    MCTableViewTextFieldCell *selectedCell_;
    MCMercenary *selectedMercenary_;
    
    JsonBox::Array mercenaries_;
    JsonBox::Array nervousMercenaries_;
    JsonBox::Array content_;
    bool showNervous_;
    
    CCLabelTTF *money_;
    CCMenu *hire_;
};

#endif /* defined(__Military_Confrontation__MCHiringMercenaryUI__) */
