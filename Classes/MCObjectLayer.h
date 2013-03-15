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
    
    CREATE_FUNC(MCObjectLayer);
    
    void setTMXTiledMap(CCTMXTiledMap *aMap);
    
#warning debug rect
    void draw(void) {
        CCLayer::draw();
        CCRect r = hero_->getAABB();
        CCPoint realPosition = ccpAdd(r.origin, hero_->getPosition());
        CCPoint *ps = new CCPoint[2];
        ps[0] = realPosition;
        ps[1] = realPosition;
        ps[1].x += r.size.width;
        ps[1].y += r.size.height;
        ccDrawSolidRect(ps[0],
                        ps[1],
                        ccc4f(0.2, 0.4, 0.9, 0.8));
    }
    
    void onEnter();
    void onExit();
    
    void loadEntrancesFromScenePackage(MCScenePackage *aScenePackage);
    
    /**
     * 控制器回调
     */
    void controllerMove(MCControllerDelegate *sender, const CCPoint &delta);
    
protected:
    CCPoint viewLocationToTiledMapLocation(const CCPoint &aViewLocation);
    void moveTo(const CCPoint &delta);
    
    void detectsCollidesWithEntrances(const CCPoint &heroMaybeMoveToPositionAtMapForCheck);
    bool detectsCollision(const CCPoint &heroMaybeMoveToPositionAtMapForCheck);
    bool detectsCollidesWithSemiTransparents(const CCPoint &heroMaybeMoveToPositionAtMapForCheck);
    bool detectsCollidesWithBarriers(const CCPoint &heroMaybeMoveToPositionAtMapForCheck);
    bool detectsCollidesWithNPCs(const CCPoint &heroMaybeMoveToPositionAtMapForCheck);
    bool detectsCollidesWithMonsters(const CCPoint &heroMaybeMoveToPositionAtMapForCheck);
    bool detectsCollidesWithMercenaries(const CCPoint &heroMaybeMoveToPositionAtMapForCheck);
    
private:
    MCRoleEntity *hero_;
    CCArray *npcs_;
    CCArray *monsters_;
    CCArray *mercenaries_;
    CCTMXTiledMap *map_;
    CCTMXLayer *metaLayer_;
    
    CCArray *barriers_;          /* 障碍物 */
    CCArray *semiTransparents_;  /* 半透明 */
    CCArray *entrances_;         /* 入口 */
    
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


#endif /* defined(__Military_Confrontation__MCObjectLayer__) */
