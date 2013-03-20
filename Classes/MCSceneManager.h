//
//  MCSceneManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSceneManager__
#define __Military_Confrontation__MCSceneManager__

#include "MCScenePackage.h"

class MCScene;

enum {
    MCPushScene     = 0,
    MCPopScene      = 1,
    MCReplaceScene  = 2
};
typedef mc_enum_t MCChangeSceneMethod;

class MCSceneManager : public CCObject {
private:
    MCSceneManager();
    
public:
    ~MCSceneManager();
    
    static MCSceneManager *sharedSceneManager();
    
    MCScenePackage *packageWithObjectId(mc_object_id_t anObjectId);
    
    /*
     * 根据ID生成场景并返回
     * IMPORTANT：场景由MCSceneManager管理，注意内存泄漏！
     */
    MCScene *sceneWithObjectId(mc_object_id_t anObjectId);
    
    /*
     * 清除由sceneWithObjectId:方法生成的MCGameScene对象
     */
    void cleanupSceneWithObjectId(mc_object_id_t anObjectId);
    
    /**
     * 切换当前场景为aNewScene
     */
    void changeScene(MCScene *aNewScene, const char *anEntranceName, MCChangeSceneMethod method = MCReplaceScene);
    
    /**
     * 切换当前场景为ID为anObjectId的场景
     */
    void changeSceneWithObjectId(mc_object_id_t anObjectId, const char *anEntranceName, MCChangeSceneMethod method = MCReplaceScene);
private:
    void loadSceneListFile();
    
private:
    CCDictionary *scenes_; /* 以mc_object_id_t为key */
    CCDictionary *scenePackages_; /* 以mc_object_id_t为key */
    
    MCScene *lastScene_;
    MCScene *currentScene_;
};

class MCSceneDelegate {
public:
    /**
     * 移动到场景
     * aSceneId(in): 场景ID
     * anEntranceName(in): 场景入口名
     */
    virtual void gotoScene(mc_object_id_t aSceneId, const char *anEntranceName, bool isInternal = false) = 0;
    
    /**
     * 从内部场景(比如房子、商店)出去
     */
    virtual void goOut() = 0;
    
    virtual MCScenePackage *getScenePackage() const = 0;
};

#endif /* defined(__Military_Confrontation__MCSceneManager__) */
