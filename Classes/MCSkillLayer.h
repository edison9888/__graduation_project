//
//  MCSkillLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSkillLayer__
#define __Military_Confrontation__MCSkillLayer__

#include "MCBaseInfoLayer.h"
#include "JsonBox.h"

#include <cocos-ext.h>

USING_NS_CC_EXT;

class MCSkill;
class MCTableViewTextFieldCell;

class MCSkillLayer : public MCBaseInfoLayer, public CCTableViewDataSource, public CCTableViewDelegate {
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
    void tableCellTouched(CCTableView *table, CCTableViewCell *cell);
    
    CREATE_FUNC(MCSkillLayer);
    
protected:
    void loadData();
    
    void selectSkillType(CCObject *anObject);
    
private:
    CCTableView *tableView_; /* 技能列表 */
    CCTableView *contentTableView_;
    CCSize tableViewSize_;
    MCTableViewTextFieldCell *selectedCell_;
    
    CCSprite *icon_;
    
    MCSkill *selectedSkill_;
    
    CCLabelTTF *proficiency_;
    
    JsonBox::Array content_;
    CCArray *skills_;
};

#endif /* defined(__Military_Confrontation__MCSkillLayer__) */
