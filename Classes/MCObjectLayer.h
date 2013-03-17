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

class MCObjectLayer : public CCLayer, public MCControllerDelegate {
    
public:
    ~MCObjectLayer();
    bool init();
    
    static MCObjectLayer *create(MCScenePackageType aScenePackageType);
    
    void setTMXTiledMap(CCTMXTiledMap *aMap);
    
    void onEnter();
    void onExit();
    
    inline CCArray *objects() {
        return objects_;
    }
    
    void loadEntrancesFromScenePackage(MCScenePackage *aScenePackage);
    
protected:
    CCPoint viewLocationToTiledMapLocation(const CCPoint &aViewLocation);
    virtual void moveTo(const CCPoint &delta);
    
    virtual void detectsCollidesWithEntrances(const MCOBB &anOBB);
    virtual void detectsCollidesWithEntrances(const MCOBB &anOBB, const CCPoint &anOffset);
    
    virtual bool detectsCollision(const MCOBB &anOBB);
    virtual bool detectsCollision(const MCOBB &anOBB, const CCPoint &anOffset);
    
    virtual void detectsCollidesWithSemiTransparents(const MCOBB &anOBB);
    virtual void detectsCollidesWithSemiTransparents(const MCOBB &anOBB, const CCPoint &anOffset);
    
    virtual bool detectsCollidesWithBarriers(const MCOBB &anOBB);
    virtual bool detectsCollidesWithBarriers(const MCOBB &anOBB, const CCPoint &anOffset);
    
    virtual bool detectsCollidesWithObjects(const MCOBB &anOBB);
    virtual bool detectsCollidesWithObjects(const MCOBB &anOBB, const CCPoint &anOffset);
    
    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB) { return false; }
    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB, const CCPoint &anOffset) { return false; }
    
//protected:
#warning debug
public:
    MCRoleEntity *hero_;
    CCArray *mercenaries_;
    CCTMXTiledMap *map_;
    CCTMXLayer *metaLayer_;
    
    CCArray *barriers_;          /* 障碍物 */
    CCArray *semiTransparents_;  /* 半透明 */
    CCArray *entrances_;         /* 入口 */
    CCArray *objects_;
    
    CC_SYNTHESIZE(MCSceneDelegate *, sceneDelegate_, SceneDelegate);
    
    CCSize mapSize_;
    CCSize tileSize_;
    CCSize scaledTileSize_;
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
    void controllerMove(MCControllerDelegate *sender, const CCPoint &delta);
    
protected:
};

class MCBattleFieldSceneObjectLayer : public MCObjectLayer {
public:
    void didSelectRole(MCRole *aRole);
    
    virtual void controllerDidSelectRole(MCControllerDelegate *aSender, MCRole *aSelectedRole);
    
    /**
     * 取消了选择的人物。
     */
    virtual void controllerDidUnselectRole(MCControllerDelegate *aSender, MCRole *aSelectedRole);
    
    /**
     * 切换多选模式。
     */
    virtual void controllerDidToggleMultiSelectionMode(MCControllerDelegate *aSender, bool isMultiSelectionMode);
    
    /**
     * 选择全部，若已经全部选择，则全部取消选择。
     */
    virtual void controllerDidSelectAll(MCControllerDelegate *aSender, bool isMultiSelectionMode);
    
    /**
     * 在选择了人物的情况下，指定移动到某个位置(在地图上的)
     */
    virtual void controllerDidPointTo(MCControllerDelegate *aSender, const CCPoint &locationAtMap);
    
    /**
     * 是否允许拖动anItem。按下图标的时候执行。
     */
    virtual bool controllerShouldDragItem(MCControllerDelegate *aSender, MCItem *anItem);
    
    /**
     * 将要开始拖动anItem。按下图标后，首次移动anItem的时候执行。
     */
    virtual void controllerWillDragItem(MCControllerDelegate *aSender, MCItem *anItem);
    
    /**
     * 拖动完anItem，拖动到了人物aRole上，若aRole等于NULL，则表示没有拖动到任何人物上。放开anItem的时候执行。
     */
    virtual void controllerDidFinishDragItem(MCControllerDelegate *aSender, MCItem *anItem, MCRole *aRole);
    
    /**
     * 选择了anItem。按下和放开手指都在anItem的范围内时执行。
     */
    virtual void controllerDidSelectItem(MCControllerDelegate *aSender, MCItem *anItem);
    
protected:
    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB);
    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB, const CCPoint &anOffset);
};

#endif /* defined(__Military_Confrontation__MCObjectLayer__) */
