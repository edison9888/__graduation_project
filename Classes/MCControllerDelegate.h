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
class MCBattleController;

/* 控制器代理 */
class MCJoypadControllerDelegate {
public:
    virtual void controllerDidMove(const CCPoint &delta) {}
    
    virtual void controllerDidRelease() {}
    
     /* 行动 */
    virtual void controllerDidActivate() {}
    
    virtual MCRole *selectedRole() { return NULL; }
};

class MCBattleControllerDelegate {
public:
    /**
     * 选择了人物。
     */
    virtual void controllerDidSelectRole(MCBattleController *aBattleController, MCRole *aSelectedRole) {}
    
    /**
     * 取消了选择的人物。
     */
    virtual void controllerDidUnselectRole(MCBattleController *aBattleController, MCRole *aSelectedRole) {}
    
    /**
     * 选择全部，若已经全部选择，则全部取消选择。
     */
    virtual void controllerDidSelectAll(MCBattleController *aBattleController, MCTeam *aTeam) {}
    
    /**
     * 全部取消选择。
     */
    virtual void controllerDidUnselectAll(MCBattleController *aBattleController, MCTeam *aTeam) {}
    
    /**
     * 进入多选模式
     */
    virtual void controllerDidEnterMultiSelectionMode(MCBattleController *aBattleController) {}
    
    /**
     * 退出多选模式
     */
    virtual void controllerDidExitMultiSelectionMode(MCBattleController *aBattleController) {}
    
    /**
     * 聚焦人物
     */
    virtual void controllerDidFocus(MCBattleController *aBattleController, MCRole *aRole) {}
    
    /**
     * 在选择了人物的情况下，指定移动到某个位置(在地图上的)
     */
    virtual void controllerDidPointTo(MCBattleController *aBattleController, const CCPoint &locationAtMap) {}
    
//    /**
//     * 是否允许拖动anItem。按下图标的时候执行。
//     */
//    virtual bool controllerShouldDragItem(MCBattleController *aBattleController, MCItem *anItem) { return false; }
    
    /**
     * 将要开始拖动anItem。按下图标后，首次移动anItem的时候执行。
     */
    virtual void controllerWillDragItem(MCBattleController *aBattleController, MCItem *anItem) {}
    
    /**
     * 拖动完anItem，拖动到了人物aRole上，若aRole等于NULL，则表示没有拖动到任何人物上。放开anItem的时候执行。
     */
    virtual void controllerDidFinishDragItem(MCBattleController *aBattleController, MCItem *anItem, MCRole *aRole) {}
    
    /**
     * 选择了anItem。按下和放开手指都在anItem的范围内时执行。
     */
    virtual void controllerDidSelectItem(MCBattleController *aBattleController, MCItem *anItem) {}
    
    /**
     * 拖动地图
     */
    virtual void controllerDidDragMap(MCBattleController *aBattleController, const CCPoint &anOffset) {}
    
    virtual void controllerDidAttach(MCBattleController *aBattleController) {}
    
    virtual void controllerDid(MCBattleController *aBattleController) {}
};

#endif /* defined(__Military_Confrontation__MCControllerDelegate__) */
