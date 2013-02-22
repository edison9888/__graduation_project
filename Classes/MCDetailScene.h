//
//  MCDetailScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-29.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCDetailScene__
#define __Military_Confrontation__MCDetailScene__

#include <cocos2d.h>
USING_NS_CC;

#include "MCStateLayer.h"

class MCDetailScene : public CCScene {
public:
    bool init();
    
    void menuItem_clicked(CCObject* aSender);
    void backward(CCObject* aSender);
    
    void onEnter();
    void onExit();
    
    void showState(); /* 显示状态选项卡 */
    void showProps(); /* 显示道具选项卡 */
    void showTasks(); /* 显示任务选项卡 */
    void showEquipment(); /* 显示装备选项卡 */
    void showSkills(); /* 显示技能选项卡 */
    void showMercenary(); /* 显示佣兵选项卡 */
    void showQuitWindow(); /* 显示退出窗口 */
        
    CREATE_FUNC(MCDetailScene);
private:
    CCMenuItem *lastSelectedMenuItem_;
    MCDetailLayer *lastShownLayer_;
    
    MCStateLayer *stateLayer_;
};

#endif /* defined(__Military_Confrontation__MCDetailScene__) */
