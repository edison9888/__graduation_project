//
//  MCTestController.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestController__
#define __Military_Confrontation__MCTestController__

#include "MCTestLayer.h"
#include "MCJoypadControllerLayer.h"

#include "MCGeometry.h"

USING_NS_CC;

class MCRoleEntity;

class MCTestController : public MCTestLayer, public MCJoypadControllerDelegate {
    CCLabelTTF *label_;
    void draw();
    CCSprite *sp[4];
    CCArray *barriers_;
    
public:
    bool init();
    
    static CCScene *scene()
    {
        CCScene *scene = CCScene::create();
        
        MCTestController * l1 = new MCTestController;
        
        l1->init();
        scene->addChild(l1);
        
        MCJoypadControllerLayer *layer = MCJoypadControllerLayer::create();
        layer->setDelegate(l1);
        scene->addChild(layer);
        
        return scene;
    }
    
    void controllerMove(MCJoypadControllerDelegate *sender, const CCPoint &delta);
    
    void onEnter();
    void onExit();
    void update(float dt);
    
private:
    CCPoint viewLocationToTiledMapLocation(CCPoint aViewLocation);
    
    void moveTo(const CCPoint &delta);
    
    MCRoleEntity *hero_;
    CCTMXTiledMap *map_;
    CCTMXLayer *metaLayer_;
    CCActionInterval* walkAction_;
    
    CCAction *heroAction_;
    CCAction *mapAction_;
    
    CCSize winSize;
    CCSize mapSize;
    CCSize tileSize;
    CCSize scaledTileSize;
    float contentScaleFactor;
    float blockWidth;
    float blockHeight;
    int mapWidth;
    int mapHeight;
    int winWidth;
    int winHeight;
};

#endif /* defined(__Military_Confrontation__MCTestController__) */
