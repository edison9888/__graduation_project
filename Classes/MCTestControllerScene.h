//
//  MCTestControllerScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestControllerScene__
#define __Military_Confrontation__MCTestControllerScene__

#include <cocos2d.h>
#include "MCControllerLayer.h"

#include "MCGeometry.h"

USING_NS_CC;

class MCTestControllerLayer : public CCLayerColor, public MCControllerDelegate {
    
public:
    bool init();
    
    static CCScene *scene()
    {
        CCScene *scene = CCScene::create();
        
        MCTestControllerLayer * l1 = new MCTestControllerLayer;
        
        l1->init();
        scene->addChild(l1);
        
        MCControllerLayer *layer = MCControllerLayer::create();
        layer->setDelegate(l1);
        scene->addChild(layer);
        
        return scene;
    }
    
    void controllerMoveUp(MCControllerDelegate *sender);
    void controllerMoveDown(MCControllerDelegate *sender);
    void controllerMoveLeft(MCControllerDelegate *sender);
    void controllerMoveRight(MCControllerDelegate *sender);
    
    void onEnter();
    void onExit();
    void update(float dt);
    MCPoint tileCoordForPosition(MCPoint pos);
    
private:
    void moveTo(MCControlButtonCode aCode);
    void verticallyMove(MCControlButtonCode aCode);
    void horizontallyMove(MCControlButtonCode aCode);
    
    CCSprite *hero_;
    CCTMXTiledMap *map_;
    CCTMXLayer *metaLayer_;
    CCActionInterval* flyAction_;
    
    CCAction *heroAction_;
    CCAction *mapAction_;
    
    MCSize winSize;
    MCSize mapSize;
    MCSize tileSize;
    float contentScaleFactor;
    int mapWidth;
    int mapHeight;
    int winWidth;
    int winHeight;
    int blockWidth;
    int blockHeight;
    
    MCPoint offsetUp;
    MCPoint offsetDown;
    MCPoint offsetLeft;
    MCPoint offsetRight;
};

#endif /* defined(__Military_Confrontation__MCTestControllerScene__) */
