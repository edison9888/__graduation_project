//
//  MCControllerDelegate.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCControllerDelegate__
#define __Military_Confrontation__MCControllerDelegate__

#include <cocos2d.h>
#include "MCType.h"

USING_NS_CC;

class MCRole;
class MCTeam;
class MCItem;

/* 控制器代理 */
class MCJoypadControllerDelegate {
public:
    virtual void controllerDidMove(MCJoypadControllerDelegate *aSender, const CCPoint &delta) {}
    
     /* 行动 */
    virtual void controllerDidActivate() {}
};

class MCBattleControllerDelegate {
public:
    /**
     * 选择了人物。
     */
    virtual void controllerDidSelectRole(MCBattleControllerDelegate *aSender, MCRole *aSelectedRole) {}
    
    /**
     * 取消了选择的人物。
     */
    virtual void controllerDidUnselectRole(MCBattleControllerDelegate *aSender, MCRole *aSelectedRole) {}
    
    /**
     * 选择全部，若已经全部选择，则全部取消选择。
     */
    virtual void controllerDidSelectAll(MCBattleControllerDelegate *aSender, MCTeam *aTeam) {}
    
    /**
     * 全部取消选择。
     */
    virtual void controllerDidUnselectAll(MCBattleControllerDelegate *aSender, MCTeam *aTeam) {}
    
    /**
     * 进入多选模式
     */
    virtual void controllerDidEnterMultiSelectionMode(MCBattleControllerDelegate *aSender) {}
    
    /**
     * 退出多选模式
     */
    virtual void controllerDidExitMultiSelectionMode(MCBattleControllerDelegate *aSender) {}
    
    /**
     * 在选择了人物的情况下，指定移动到某个位置(在地图上的)
     */
    virtual void controllerDidPointTo(MCBattleControllerDelegate *aSender, const CCPoint &locationAtMap) {}
    
//    /**
//     * 是否允许拖动anItem。按下图标的时候执行。
//     */
//    virtual bool controllerShouldDragItem(MCBattleControllerDelegate *aSender, MCItem *anItem) { return false; }
    
    /**
     * 将要开始拖动anItem。按下图标后，首次移动anItem的时候执行。
     */
    virtual void controllerWillDragItem(MCBattleControllerDelegate *aSender, MCItem *anItem) {}
    
    /**
     * 拖动完anItem，拖动到了人物aRole上，若aRole等于NULL，则表示没有拖动到任何人物上。放开anItem的时候执行。
     */
    virtual void controllerDidFinishDragItem(MCBattleControllerDelegate *aSender, MCItem *anItem, MCRole *aRole) {}
    
    /**
     * 选择了anItem。按下和放开手指都在anItem的范围内时执行。
     */
    virtual void controllerDidSelectItem(MCBattleControllerDelegate *aSender, MCItem *anItem) {}
    
    /**
     * 拖动地图
     */
    virtual void controllerDidDragMap(MCBattleControllerDelegate *aSender, const CCPoint &anOffset) {}
    
    virtual void controllerDid(MCBattleControllerDelegate *aSender) {}
};

#endif /* defined(__Military_Confrontation__MCControllerDelegate__) */
