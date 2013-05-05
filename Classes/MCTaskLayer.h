//
//  MCTaskLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskLayer__
#define __Military_Confrontation__MCTaskLayer__

#include "MCBaseInfoLayer.h"
#include "MCConfirm.h"

class MCTaskLayer : public MCBaseInfoLayer, public MCConfirmDelegate {
public:
    ~MCTaskLayer();
    bool init();
    
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    
    static MCTaskLayer *create();
    
protected:
    void loadData();
    
    void abortTask(CCObject *);
    
private:
    CCLayer *taskLayer_;        /* 当前已接受任务显示的层 */
    CCLayer *noTaskLayer_;      /* 当前没接受任务显示的层 */
    CCMenu *abortTaskButton_;   /* 放弃任务按钮 */
    
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
};

#endif /* defined(__Military_Confrontation__MCTaskLayer__) */
