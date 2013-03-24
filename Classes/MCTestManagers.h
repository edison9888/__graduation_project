//
//  MCTestManagers.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestFlagManager__
#define __Military_Confrontation__MCTestFlagManager__

#include "MCTestLayer.h"

USING_NS_CC;

#include "MCTaskManager.h"
#include "MCBackpack.h"
#include "MCGameState.h"

#define MCTestManagerScene(layer) \
static CCScene *scene() { \
    CCScene *scene = CCScene::create(); \
    layer * l = new layer; \
    l->init(); \
    l->autorelease(); \
    scene->addChild(l); \
    return scene; \
} \

class MCRoleEntity;

class MCTestFlagManager : public MCTestLayer {
public:
    bool init();
    
    static CCScene *scene() {
        CCScene *scene = CCScene::create();
    
        MCTestFlagManager * l = new MCTestFlagManager;
    
        l->init();
        l->autorelease();
        scene->addChild(l);
    
        return scene;
    }
};

class MCTestTaskManager : public MCTestLayer {
public:
    bool init() {
        if (MCTestLayer::initWithColor(ccc4(64, 64, 64, 255))) {
            MCTaskManager::sharedTaskManager()->saveData();
            MCTaskManager::sharedTaskManager()->loadData();
            return true;
        }
        
        return false;
    }
 
    MCTestManagerScene(MCTestTaskManager);
};

class MCTestBackpack : public MCTestLayer {
public:
    bool init() {
        if (MCTestLayer::initWithColor(ccc4(64, 64, 64, 255))) {
//            MCGameState::sharedGameState()->save();
            
            return true;
        }
        
        return false;
    }
    
    MCTestManagerScene(MCTestBackpack);
};

#endif /* defined(__Military_Confrontation__MCTestFlagManager__) */
