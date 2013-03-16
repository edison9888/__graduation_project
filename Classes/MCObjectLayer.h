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
#include "MCActionMenu.h"

class MCObjectDataSource {
public:
    virtual CCArray *objects() = 0;
};

class MCObjectLayer : public CCLayer, public MCControllerDelegate, public MCObjectDataSource {
    
public:
    ~MCObjectLayer();
    bool init();
    
    static MCObjectLayer *create(MCScenePackageType aScenePackageType);
    
    void setTMXTiledMap(CCTMXTiledMap *aMap);
    
//#warning debug rect
//    void draw(void) {
//        CCLayer::draw();
//        CCRect r = hero_->getAABB();
//        CCPoint realPosition = ccpAdd(r.origin, hero_->getPosition());
//        CCPoint *ps = new CCPoint[2];
//        ps[0] = realPosition;
//        ps[1] = realPosition;
//        ps[1].x += r.size.width;
//        ps[1].y += r.size.height;
//        ccDrawSolidRect(ps[0],
//                        ps[1],
//                        ccc4f(0.2, 0.4, 0.9, 0.8));
//    }
    
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
    
    MCActionMenu *actionMenu_;
    
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
    
protected:
    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB);
    virtual bool detectsCollidesWithMercenaries(const MCOBB &anOBB, const CCPoint &anOffset);
};

#endif /* defined(__Military_Confrontation__MCObjectLayer__) */
