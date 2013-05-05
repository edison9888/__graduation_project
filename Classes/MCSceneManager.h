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
private:
    void loadSceneListFile();
    
private:
    CCDictionary *scenes_; /* 以mc_object_id_t为key */
    CCDictionary *scenePackages_; /* 以mc_object_id_t为key */
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
     * 移动场景
     */
    virtual void moveSceneToLocation(const CCPoint &aLocation, bool adjusted = false) = 0;
    
    /**
     * 地图的默认偏移位置
     */
    virtual CCPoint getDefaultLocation() const = 0;
    
    virtual MCScene *getScene() = 0;
    
    virtual MCScenePackage *getScenePackage() const = 0;
    
    virtual void pauseScene() {}
    virtual void resumeScene() {}
};

#endif /* defined(__Military_Confrontation__MCSceneManager__) */
