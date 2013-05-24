//
//  MCPlayerInfo.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-29.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCPlayerInfo__
#define __Military_Confrontation__MCPlayerInfo__

#include "MCConfirm.h"

extern const char *kMCPlayerInfoDidHideNotification;

class MCBaseInfoLayer;
class MCStateLayer;
class MCPropsLayer;
class MCTaskLayer;
class MCEquipmentLayer;
class MCSkillLayer;
class MCMercenaryLayer;

class MCPlayerInfo : public CCLayer, public MCConfirmDelegate {
public:
    bool init();
    
    static MCPlayerInfo *create();
    
    void menuItem_clicked(CCObject* aSender);
    void backward(CCObject* aSender);
    
    void show();
    void hide();
    
    void showState(); /* 显示状态选项卡 */
    void showProps(); /* 显示道具选项卡 */
    void showTask(); /* 显示任务选项卡 */
    void showEquipment(); /* 显示装备选项卡 */
    void showSkills(); /* 显示技能选项卡 */
    void showMercenary(); /* 显示佣兵选项卡 */
    void showQuitWindow(); /* 显示退出窗口 */
    
    void confirmDidClickYesButton(MCConfirm *aConfirm);
private:
    CCMenuItem *lastSelectedMenuItem_;
    MCBaseInfoLayer *lastShownLayer_;
    
    MCStateLayer     *stateLayer_;
    MCPropsLayer     *propsLayer_;
    MCTaskLayer      *taskLayer_;
    MCEquipmentLayer *equipmentLayer_;
    MCSkillLayer     *skillLayer_;
    MCMercenaryLayer *mercenaryLayer_;
    
    CCLayer *viewSelector_;
};

#endif /* defined(__Military_Confrontation__MCPlayerInfo__) */
