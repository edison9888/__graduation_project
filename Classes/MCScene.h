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

#include "MCBackgroundLayer.h"

#if MC_DEBUG_NON_VISUAL_OBJECTS == 1
#include "MCViewportLayer.h"
#endif

#include "MCControllerLayer.h"
#include "MCSceneManager.h"
#include "MCPlayerInfo.h"
#include "MCConfirm.h"

class MCScene;
class MCCamera;
class MCTrigger;
class MCRoleEntity;
class MCAStar;
class MCObjectLayer;
class MCMezzanine;

class MCSceneContext : public CCObject {
    friend class MCScene;
    friend class MCObjectLayer;
public:
    MCSceneContext();
    ~MCSceneContext();
    
    void enemyWasDied(MCRole *aRole); /* 敌方人物挂了 */
        
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
    friend class MCSceneContext;
    friend class MCSceneManager;
    friend class MCSceneController;
public:
    MCScene()
    : controller_(NULL)
    , objects_(NULL)
    , mezzanine_(NULL)
#if MC_DEBUG_NON_VISUAL_OBJECTS == 1
    , viewport_(NULL)
#endif
    , background_(NULL)
    , entranceName_(NULL)
    , trigger_(NULL)
    , context_(NULL)
    , abortTaskConfirm_(NULL) { }
    
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
     * 移动到场景
     * aSceneId(in): 场景ID
     * anEntranceName(in): 场景入口名
     */
    void gotoScene(mc_object_id_t aSceneId, const char *anEntranceName, bool isInternal = false);
    
    void moveSceneToLocation(const CCPoint &aLocation, bool adjusted = false);
    
    MCScene *getScene();
    
    inline void pauseScene() {
        pauseSchedulerAndActions();
        controller_->setEnabled(false);
        playerInfoMenu_->setVisible(false);
    }
    
    inline void resumeScene() {
        playerInfoMenu_->setVisible(true);
        controller_->setEnabled(true);
        resumeSchedulerAndActions();
    }
    
    inline void pauseInput() {
        controller_->setEnabled(false);
        playerInfoMenu_->setVisible(false);
    }
    
    inline void resumeInput() {
        controller_->setEnabled(true);
        playerInfoMenu_->setVisible(true);
    }
    
    inline MCMezzanine *mezzanine() {
        return mezzanine_;
    }
    
    virtual void showPlayerInfo();
    
    virtual void detectsCollidesWithSemiTransparents(MCRole *aRole);
    
    /* abort task confirm */
    void showAbortTaskConfirm(const char *aMessage);
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    void confirmDidClickNoButton(MCConfirm *aConfirm);
    
protected:
    bool hasEntrance(const char *anEntranceName);
    
    virtual void playerInfoDidHide();
    
    MCControllerLayer *controller_; /* 控制层 */
    MCObjectLayer *objects_; /* 对象层 */
    MCMezzanine *mezzanine_; /* 夹层世界 */
#if MC_DEBUG_NON_VISUAL_OBJECTS == 1
    MCViewportLayer *viewport_; /* 调试用的视角层 */
#endif
    MCBackgroundLayer *background_; /* 背景层 */
    
    CCArray *scenes_; /* 场景地图ID数组 */
    
    MCPlayerInfo *playerInfo_; /* 状态界面 */
    CCMenu *playerInfoMenu_;
    
    MCScript *trigger_; /* 若存在则进入场景后执行 */
    
    MCConfirm *abortTaskConfirm_;
    
    /**
     * 若不为空，则人物出现在该入口位置(除非改场景有重生点并且需要重生)。
     * 若为空，
     */
    CC_SYNTHESIZE(CCString *, entranceName_, EntranceName);
    
    CC_SYNTHESIZE_READONLY(MCCamera *, sceneCamera_, SceneCamera);
    CC_SYNTHESIZE_READONLY(CCPoint, defaultLocation_, DefaultLocation);
    CC_SYNTHESIZE_READONLY(MCAStar *, aStar_, AStar); /* A*算法实现 */
    CC_SYNTHESIZE_READONLY(CCDictionary *, entrances_, Entrances);
    CC_SYNTHESIZE_READONLY(MCSceneContext *, context_, Context);
    CC_SYNTHESIZE_READONLY(MCScenePackage *, scenePackage_, ScenePackage);
};

#endif /* defined(__Military_Confrontation__MCScene__) */
