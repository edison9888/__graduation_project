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

//#include "MCObjectLayer.h"
#include "MCBackgroundLayer.h"

#include "MCViewportLayer.h"
#include "MCControllerLayer.h"
#include "MCSceneManager.h"
#include "MCDetail.h"
#include "MCConfirm.h"

class MCScene;
class MCCamera;
class MCTrigger;
class MCRoleEntity;
class MCAStar;
class MCObjectLayer;

class MCSceneContext : public CCObject {
    friend class MCScene;
    friend class MCObjectLayer;
public:
    MCSceneContext();
    ~MCSceneContext();
    
    CC_SYNTHESIZE_READONLY(CCArray *, objects_, Objects); /* 场景对象 */
    CC_SYNTHESIZE_READONLY(MCScene *, scene_, Scene);
    CC_SYNTHESIZE_READONLY(bool, inited_, Inited); /* 是否初始化过场景对象 */
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
    CCArray *contextStack_;
};

class MCScene : public CCScene, public MCSceneDelegate, public MCConfirmDelegate {
    friend class MCSceneManager;
    friend class MCSceneController;
public:
    MCScene()
    : controller_(NULL)
    , objects_(NULL)
    , viewport_(NULL)
    , background_(NULL)
    , entranceName_(NULL)
    , trigger_(NULL) { }
    
    ~MCScene();
    
    /**
     * 从场景包加载场景初始化
     */
    bool initWithScenePackage(MCScenePackage *aPackage);
    
    /**
     * 从场景包加载场景
     */
    static MCScene *createWithScenePackage(MCScenePackage *aPackage);
    
    /**
     * 获取地图偏移值
     */
    CCPoint getMapOffset() const;
    
    /**
     * 获取地图
     */
    CCTMXTiledMap *getMap() const;
    
    /**
     * 获取场景大小
     */
    CCSize &getSceneSize() const;
    
    void onEnter();
    void onExit();
    void update(float dt);
    
    virtual void installController() {}
    
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
    
    void moveSceneToLocation(const CCPoint &aLocation, bool adjusted = false);
    
    MCScene *getScene();
    
    inline void pauseScene() {
        pauseSchedulerAndActions();
        controller_->setEnabled(false);
        detailMenu_->setVisible(false);
    }
    
    inline void resumeScene() {
        detailMenu_->setVisible(true);
        controller_->setEnabled(true);
        resumeSchedulerAndActions();
    }
    
    inline void pauseInput() {
        controller_->setEnabled(false);
        detailMenu_->setVisible(false);
    }
    
    inline void resumeInput() {
        controller_->setEnabled(true);
        detailMenu_->setVisible(true);
    }
    
    virtual void showDetail();
    
    /* abort task confirm */
    void showAbortTaskConfirm(const char *aMessage);
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    
protected:
    bool hasEntrance(const char *anEntranceName);
    
    virtual void detailDidHide();
    
    MCControllerLayer *controller_; /* 控制层 */
    MCObjectLayer *objects_; /* 对象层 */
#warning: todo：记得删除调试用视角层
    MCViewportLayer *viewport_; /* 调试用的视角层 */
    MCBackgroundLayer *background_; /* 背景层 */
    
    CCArray *scenes_; /* 场景地图ID数组 */
    CCArray *triggers_; /* 触发器 */
    
    MCDetail *detail_; /* 状态界面 */
    CCMenu *detailMenu_;
    
    MCScript *trigger_; /* 若存在则进入场景后执行 */
    
    /**
     * 若不为空，则人物出现在该入口位置(除非改场景有重生点并且需要重生)。
     * 若为空，
     */
    CC_SYNTHESIZE(CCString *, entranceName_, EntranceName);
    
    CC_SYNTHESIZE_READONLY(MCCamera *, sceneCamera_, SceneCamera);
    CC_SYNTHESIZE_READONLY(CCPoint, defaultLocation_, DefaultLocation);
    CC_SYNTHESIZE_READONLY(MCAStar *, aStar_, AStar); /* A*算法实现 */
    CC_SYNTHESIZE_READONLY(CCDictionary *, entrances_, Entrances);
    CC_SYNTHESIZE_READONLY(MCSceneContext *, context, Context);
    CC_SYNTHESIZE_READONLY(MCScenePackage *, scenePackage_, ScenePackage);
};

#endif /* defined(__Military_Confrontation__MCScene__) */
