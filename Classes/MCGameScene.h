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

class MCGameScene;
class MCTrigger;

class MCGameSceneContext : public CCObject {
    friend class MCGameScene;
public:
    CC_SYNTHESIZE_READONLY(MCGameScene *, scene_, Scene);
};

class MCGameScene : public CCScene, public MCControllerDelegate {
    
public:
    MCGameScene()
    : controller_(MCControllerLayer::create()),
        objects_(NULL),
        viewport_(NULL),
        background_(NULL),
        isAtWar_(false) { }
    
    /**
     * 从场景包加载场景初始化 
     */
    bool initWithScenePackage(MCScenePackage *aPackage);
    
    /**
     * 从场景包加载场景 
     */
    static MCGameScene *createWithScenePackage(MCScenePackage *aPackage);
    
    void onEnter();
    void onExit();
    
    /**
     * 控制器回调 
     */
    void controllerMove(MCControllerDelegate *sender, const CCPoint &delta);
    
    /**
     * 是否在战斗状态
     */
    inline bool isAtWar() {
        return isAtWar_;
    }
    
    /**
     * 安装触发器
     */
    void installTrigger(MCTrigger *aTrigger);
    
    /**
     * 卸载触发器
     */
    void uninstallTrigger(MCTrigger *aTrigger);
    
    /**
     * 移动到场景
     * aSceneId(in): 场景ID
     * anEntranceName(in): 场景入口名
     */
    void gotoScene(mc_object_id_t aSceneId, const char *anEntranceName);

protected:

private:
    MCControllerLayer *controller_; /* 控制层 */
    CCLayer *objects_; /* 对象层 */
#warning todo：记得删除调试用视角层
    CCLayer *viewport_; /* 调试用的视角层 */
    CCLayer *background_; /* 背景层 */
    
    CCArray *scenes_; /* 场景地图ID数组 */
    CCArray *triggers_; /* 触发器 */
    
    bool isAtWar_;
    
    CC_SYNTHESIZE_READONLY(MCGameSceneContext *, context, Context);
};

#endif /* defined(__Military_Confrontation__MCGameScene__) */
