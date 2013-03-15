//
//  MCScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-15.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCScene__
#define __Military_Confrontation__MCScene__

#include "MCScenePackage.h"

#include "MCControllerLayer.h"
#include "MCObjectLayer.h"
#include "MCBackgroundLayer.h"

#include "MCViewportLayer.h"

class MCTrigger;
class MCRoleEntity;
class MCActionMenu;

class MCSceneContext : public CCObject {
    friend class MCScene;
public:
    CC_SYNTHESIZE_READONLY(MCScene *, scene_, Scene);
};

class MCSceneContextManager {
private:
    MCSceneContextManager();
public:
    ~MCSceneContextManager();
    
    static MCSceneContextManager *sharedSceneContextManager();
    
    void pushContext(MCSceneContext *aContext);
    void popContext();
    MCSceneContext *currentContext();
    
private:
    CCArray* contextStack_;
};

class MCScene : public CCScene, public MCSceneDelegate {
    friend class MCSceneManager;
public:
    MCScene()
    : controller_(MCControllerLayer::create()),
    objects_(NULL),
    viewport_(NULL),
    background_(NULL),
    isInternalScene_(false) { }
    
    /**
     * 从场景包加载场景初始化
     */
    bool initWithScenePackage(MCScenePackage *aPackage);
    
    /**
     * 从场景包加载场景
     */
    static MCScene *createWithScenePackage(MCScenePackage *aPackage);
    
    void onEnter();
    void onExit();
    
    /**
     * 是否为内部场景(如房子、商店)
     */
    inline bool isInternalScene() {
        return isInternalScene_;
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
    void gotoScene(mc_object_id_t aSceneId, const char *anEntranceName, bool isInternal = false);
    
    /**
     * 从内部场景(比如房子、商店)出去
     */
    void goOut();
    
protected:
    bool hasEntrance(const char *anEntranceName);
    
    MCControllerLayer *controller_; /* 控制层 */
    MCObjectLayer *objects_; /* 对象层 */
#warning todo：记得删除调试用视角层
    MCViewportLayer *viewport_; /* 调试用的视角层 */
    MCBackgroundLayer *background_; /* 背景层 */
    
    CCArray *scenes_; /* 场景地图ID数组 */
    CCArray *triggers_; /* 触发器 */
    
    bool isInternalScene_;
    
    CC_PROPERTY_READONLY(CCDictionary *, entrances_, Entrances);
    CC_SYNTHESIZE_READONLY(MCSceneContext *, context, Context);
    CC_SYNTHESIZE_READONLY(MCScenePackage *, scenePackage_, ScenePackage);
};

#endif /* defined(__Military_Confrontation__MCScene__) */
