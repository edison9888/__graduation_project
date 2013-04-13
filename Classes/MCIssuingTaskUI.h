//
//  MCIssuingTaskUI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCIssuingTaskUI__
#define __Military_Confrontation__MCIssuingTaskUI__

#include "MCType.h"
#include "MCRegion.h"
#include "MCConfirm.h"
#include <cocos-ext.h>

USING_NS_CC_EXT;

class MCScene;
class MCTask;
class MCTableViewTextFieldCell;

class MCIssuingTaskUI : public CCLayer, public MCConfirmDelegate, public CCTableViewDataSource, public CCTableViewDelegate {
public:
    bool init();
    
    void initPosition();
    void attach(MCScene *aScene);
    void detach();
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    
    /* CCScrollViewDelegate */
    void scrollViewDidScroll(CCScrollView *view) {}
    void scrollViewDidZoom(CCScrollView *view) {}
    
    /* CCTableViewDataSource */
    CCSize cellSizeForTable(CCTableView *table);
    CCTableViewCell *tableCellAtIndex(CCTableView *table, unsigned int idx);
    unsigned int numberOfCellsInTableView(CCTableView *table);
    
    /* CCTableViewDelegate */
    void tableCellTouched(CCTableView *table, CCTableViewCell *cell);
    
    CREATE_FUNC(MCIssuingTaskUI);
    
protected:
    void region_clicked(CCObject* aSender); /* 点击显示该区域的任务列表 */
    void backward(CCObject* aSender); /* 关闭.... */
    void acceptTask_click(CCObject* aSender);
    
    void generate(); /* 生成任务列表 */
    void destroy();
    void loadTask(MCTask *aTask);
    
    CCTableView *tableView_; /* 任务列表 */
    CCSize tableViewSize_;
    MCTableViewTextFieldCell *selectedCell_;
    
    CCLayer *taskLayer_; /* 显示任务信息的层 */
    CCArray *tasks_;
    CCMenuItemLabel *lastSelectedRegionMenuItem_;
    CCMenuItemLabel *lastSelectedTaskMenuItem_;
    MCTask *currentTask_; /* 当前显示的任务 */
    
    int confirmType_; /* 不同的询问 */
    
    CCLabelTTF *name_;
    CCLabelTTF *description_;           /* 任务说明，好吧，其实只描述了任务的位置而已，所以应该是任务地点。*/
    
    CCLabelTTF *trapType_;              /* 陷阱类型 */
    
    CCLabelTTF *trapWideMaxSize_;       /* 大范围陷阱限制个数 */
    CCLabelTTF *trapDamageMaxSize_;     /* 大伤害陷阱限制个数 */
    CCLabelTTF *healthPotionMaxSize_;   /* 生命药水限制个数 */
    CCLabelTTF *physicalPotionMaxSize_; /* 体力药水限制个数 */
    
    CCLabelTTF *trapWideBonus_;         /* 大范围陷阱赠送个数 */
    CCLabelTTF *trapDamageBonus_;       /* 大伤害陷阱赠送个数 */
    CCLabelTTF *healthPotionBonus_;     /* 生命药水赠送个数 */
    CCLabelTTF *physicalPotionBonus_;   /* 体力药水赠送个数 */
    
    CCLabelTTF *cashPledge_;            /* 任务押金 */
    CCLabelTTF *bonus_;                 /* 任务奖励 */
    
    CCLabelTTF *targets_;               /* 任务目标 */
    
    CC_SYNTHESIZE(MCSceneRegion, sceneRegion_, SceneRegion); /* 要显示的任务区域 */
};

#endif /* defined(__Military_Confrontation__MCIssuingTaskUI__) */
