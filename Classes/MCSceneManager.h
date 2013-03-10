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
#include "MCGameScene.h"

class MCSceneManager : public CCObject {
private:
    MCSceneManager();
    
public:
    ~MCSceneManager();
    
    static MCSceneManager *sharedSceneManager();
    
    void loadSceneListFile(const char *aFilepath);
    
    MCScenePackage *packageWithObjectId(mc_object_id_t anObjectId);
    
    /*
     * 根据ID生成场景并返回
     * IMPORTANT：场景由MCSceneManager管理，注意内存泄漏！
     */
    MCGameScene *sceneWithObjectId(mc_object_id_t anObjectId);
    
    /*
     * 清除由sceneWithObjectId:方法生成的MCGameScene对象
     */
    void cleanupSceneWithObjectId(mc_object_id_t anObjectId);
    
private:
    CCDictionary *scenes_; /* 以mc_object_id_t为key */
    CCDictionary *scenePackages_; /* 以mc_object_id_t为key */
};

#endif /* defined(__Military_Confrontation__MCSceneManager__) */
