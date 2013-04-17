//
//  MCObjectLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCObjectLayer__
#define __Military_Confrontation__MCObjectLayer__

#include "MCRoleEntity.h"
#include "MCControllerDelegate.h"
#include "MCSceneManager.h"
#include "MCScene.h"
#include "MCHero.h"

class MCObjectLayer : public CCLayer, public MCJoypadControllerDelegate, public MCBattleControllerDelegate {
    friend class MCScene;
public:
    ~MCObjectLayer();
    bool init();
    
    static MCObjectLayer *create(MCScenePackageType aScenePackageType);
    
    void setTMXTiledMap(CCTMXTiledMap *aMap);
    
    void onEnter();
    void onExit();
    
    CCArray *objects() const;
    
    void loadEntrancesFromScenePackage(MCScenePackage *aScenePackage);
    
protected:
    /**
     * 拖动地图
     */
    void setSceneOffset(const CCPoint &anOffset);
    
    virtual void moveTo(const CCPoint &delta);
#if (MC_COLLISION_USE_OBB == 1)
    virtual void detectsCollidesWithEntrances(const MCOBB &anOBB);
    virtual void detectsCollidesWithEntrances(const MCOBB &anOBB, const CCPoint &anOffset);
    
    virtual bool detectsCollision(const MCOBB &anOBB);
    virtual bool detectsCollision(const MCOBB &anOBB, const CCPoint &anOffset);
    
    virtual void detectsCollidesWithSemiTransparents(const MCOBB &anOBB);
    virtual void detectsCollidesWithSemiTransparents(const MCOBB &anOBB, const CCPoint &anOffset);
    
    virtual bool detectsCollidesWithBarriers(const MCOBB &anOBB);
    virtual bool detectsCollidesWithBarriers(const MCOBB &anOBB, const CCPoint &anOffset);
#else
    virtual void detectsCollidesWithEntrances(const CCRect &anFrame);
    virtual void detectsCollidesWithEntrances(const CCRect &anFrame, const CCPoint &anOffset);
    
    virtual bool detectsCollision(const CCRect &anFrame);
    virtual bool detectsCollision(const CCRect &anFrame, const CCPoint &anOffset);
    
    virtual void detectsCollidesWithSemiTransparents(const CCRect &anFrame);
    virtual void detectsCollidesWithSemiTransparents(const CCRect &anFrame, const CCPoint &anOffset);
    
    virtual CCPoint detectsCollidesWithBarriers(const CCRect &anFrame, const CCPoint &anOffset);
    virtual bool detectsCollidesWithBarriers(const CCRect &anFrame);
#endif
    /* 人物间不在相撞~~~~ */
//    virtual bool detectsCollidesWithObjects(const MCOBB &anOBB);
//    virtual bool detectsCollidesWithObjects(const MCOBB &anOBB, const CCPoint &anOffset);
//    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB) { return false; }
//    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB, const CCPoint &anOffset) { return false; }
    
protected:
    MCRoleEntity *hero_;
    CCArray *mercenaries_;
    CCTMXTiledMap *map_;
    
    CC_SYNTHESIZE_READONLY(CCArray *, barriers_, barriers); /* 障碍物 */
    CCArray *semiTransparents_;  /* 半透明 */
    CCArray *entrances_;         /* 入口 */
//    CCArray *objects_;
    
    CC_SYNTHESIZE(MCSceneDelegate *, sceneDelegate_, SceneDelegate);
    
    MCScenePackageType scenePackageType_; /* 有可能和场景包获取的不同，原因看代码 */
    
    CCSize mapSize_;
    float contentScaleFactor_;
    int mapWidth_;
    int mapHeight_;
    int winWidth_;
    int winHeight_;
};

class MCGameSceneObjectLayer : public MCObjectLayer {
public:
    /**
     * 控制器回调
     */
    void controllerDidMove(const CCPoint &delta);
    
    void controllerDidRelease();
    
     /**
      * 行动
      */
    void controllerDidActivate();
    
    MCRole *selectedRole() { return MCHero::sharedHero(); }
    
protected:
    void dialogDidDismiss(void *anUserdata);
};

class MCBattleFieldSceneObjectLayer : public MCGameSceneObjectLayer {
public:
    /**
     * 选择了人物。
     */
    virtual void controllerDidSelectRole(MCBattleController *aBattleController, MCRole *aSelectedRole);
    
    /**
     * 取消了选择的人物。
     */
    virtual void controllerDidUnselectRole(MCBattleController *aBattleController, MCRole *aSelectedRole);
    
    /**
     * 选择全部，若已经全部选择，则全部取消选择。
     */
    virtual void controllerDidSelectAll(MCBattleController *aBattleController, MCTeam *aTeam);
    
    /**
     * 全部取消选择。
     */
    virtual void controllerDidUnselectAll(MCBattleController *aBattleController, MCTeam *aTeam);
    
    /**
     * 进入多选模式
     */
    virtual void controllerDidEnterMultiSelectionMode(MCBattleController *aBattleController);
    
    /**
     * 退出多选模式
     */
    virtual void controllerDidExitMultiSelectionMode(MCBattleController *aBattleController);
    
    /**
     * 聚焦人物
     */
    virtual void controllerDidFocus(MCBattleController *aBattleController, MCRole *aRole);
    
    /**
     * 在选择了人物的情况下，指定移动到某个位置(在地图上的)
     */
    virtual void controllerDidPointTo(MCBattleController *aBattleController, const CCPoint &locationAtMap);
    
    /**
     * 是否允许拖动anItem。按下图标的时候执行。
     */
    virtual bool controllerShouldDragItem(MCBattleController *aBattleController, MCItem *anItem);
    
    /**
     * 将要开始拖动anItem。按下图标后，首次移动anItem的时候执行。
     */
    virtual void controllerWillDragItem(MCBattleController *aBattleController, MCItem *anItem);
    
    /**
     * 拖动完anItem，拖动到了人物aRole上，若aRole等于NULL，则表示没有拖动到任何人物上。放开anItem的时候执行。
     */
    virtual void controllerDidFinishDragItem(MCBattleController *aBattleController, MCItem *anItem, MCRole *aRole);
    
    /**
     * 选择了anItem。按下和放开手指都在anItem的范围内时执行。
     */
    virtual void controllerDidSelectItem(MCBattleController *aBattleController, MCItem *anItem);
    
    /**
     * 拖动地图
     */
    virtual void controllerDidDragMap(MCBattleController *aBattleController, const CCPoint &anOffset);
    
    void controllerDidAttach(MCBattleController *aBattleController);
    
    MCRole *selectedRole();
    
protected:
    MCBattleController *controller_;
    
    /* 人物间不在相撞~~~~ */
//    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB);
//    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB, const CCPoint &anOffset);
};

#endif /* defined(__Military_Confrontation__MCObjectLayer__) */
