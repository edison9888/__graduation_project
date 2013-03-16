//
//  MCScenePackage.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCScenePackage__
#define __Military_Confrontation__MCScenePackage__

#include "MCNPC.h"
#include "MCEnemy.h"

#include "JsonBox.h"

enum {
    MCUnknownPackage            = 0,                /* 未知场景包类型，直接终结游戏 */
    MCGameScenePackage          = MCMakeEnum(0),    /* 对应MCGameScene */
    MCBattleFieldScenePackage   = MCMakeEnum(1)     /* 对应MCBattleFieldScene */
};
typedef mc_enum_t MCScenePackageType;

class MCScenePackage : public MCObject {
public:
    ~MCScenePackage();
    bool init();
    
    static MCScenePackage *create(const char *aPackagePath);
    
    MCRole *objectForObjectId(mc_object_id_t anObjectId);
    
private:
    void loadFromFile(const char *aPackagePath); /* 从场景包文件加载元数据 */
    void loadObjects(JsonBox::Object &aRoot); /* 加载对象元数据 */
    void loadScenes(JsonBox::Object &aRoot); /* 加载地图元数据 */
    
    CC_SYNTHESIZE_READONLY(MCScenePackageType, scenePackageType_, ScenePackageType);
    CC_SYNTHESIZE_READONLY(CCDictionary *, objects_, Objects) /* object，根据地图类型不同而不同 */
    
    CC_SYNTHESIZE_READONLY(CCString *, tmxTiledMapPath_, TMXTiledMapPath); /* 贴图地图路径 */
    CC_SYNTHESIZE_READONLY(CCString *, backgroundMusicPath_, BackgroundMusicPath); /* 背景音乐文件路径，NULL为无音乐 */
    
    CC_SYNTHESIZE_READONLY(CCDictionary *, scenes_, Scenes); /* 场景入口MCEntrance, Key为入口的名字 */
};

#endif /* defined(__Military_Confrontation__MCScenePackage__) */
