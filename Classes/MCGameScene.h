//
//  MCGameScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCGameScene__
#define __Military_Confrontation__MCGameScene__

#include "MCControllerLayer.h"
#include "MCScenePackage.h"

class MCGameScene : public CCScene, public MCControllerDelegate {
    
public:
    MCGameScene()
    : controller_(MCControllerLayer::create()),
        objects_(NULL),
        viewport_(NULL),
        background_(NULL) { }
    
    /* 从场景包加载场景初始化 */
    bool initWithScenePackage(MCScenePackage *aPackage);
    
    /* 从场景包加载场景 */
    static MCGameScene *createWithScenePackage(MCScenePackage *aPackage);
    
    void onEnter();
    void onExit();
    
    void controllerMoveUp(MCControllerDelegate *sender);
    void controllerMoveDown(MCControllerDelegate *sender);
    void controllerMoveLeft(MCControllerDelegate *sender);
    void controllerMoveRight(MCControllerDelegate *sender);
    
    void controllerMove(MCControllerDelegate *sender, const CCPoint &delta){};
    
protected:
    
private:
    
    MCControllerLayer *controller_; /* 控制层 */
    CCLayer *objects_; /* 对象层 */
    CCLayer *viewport_; /* 调试用的视角层 */
    CCLayer *background_; /* 背景层 */
    
    MCGameScene *east_; /* 东部地图 */
    MCGameScene *west_; /* 西部地图 */
    MCGameScene *south_; /* 南部地图 */
    MCGameScene *north_; /* 北部地图 */
};

#endif /* defined(__Military_Confrontation__MCGameScene__) */
