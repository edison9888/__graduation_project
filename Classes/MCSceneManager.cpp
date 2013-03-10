//
//  MCSceneManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSceneManager.h"

#include "JsonBox.h"
#include "MCJSONModifier.h"

static MCSceneManager *__shared_scene_list = NULL;

MCSceneManager::MCSceneManager()
{
    scenes_ = CCDictionary::create();
    scenes_->retain(); // need?
    scenePackages_ = CCDictionary::create();
    scenePackages_->retain(); // need?
}

MCSceneManager::~MCSceneManager()
{
    CC_SAFE_RELEASE(scenes_);
    CC_SAFE_RELEASE(scenePackages_);
}

MCSceneManager *
MCSceneManager::sharedSceneManager()
{
    if (__shared_scene_list == NULL) {
        __shared_scene_list = new MCSceneManager;
    }
    
    return __shared_scene_list;
}

void
MCSceneManager::loadSceneListFile(const char *aFilepath)
{
    JsonBox::Value in;
    JsonBox::Value v;
    JsonBox::Object o;
    JsonBox::Object packages;
    JsonBox::Object::iterator iter;
    const char *c_str_o_id;
    
    in.loadFromFile(aFilepath);
    o = in.getObject();
    
    packages = o["scenes"].getObject();
    for (iter = packages.begin(); iter != packages.end(); ++iter) {
        v = iter->first;
        c_str_o_id = v.getString().c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        /* load package */
        scenePackages_->setObject(MCScenePackage::create(iter->second.getString().c_str()),
                                  MCObjectIdToDickKey(o_id));
    }
}

MCScenePackage *
MCSceneManager::packageWithObjectId(mc_object_id_t anObjectId)
{
    return (MCScenePackage *) scenePackages_->objectForKey(MCObjectIdToDickKey(anObjectId));
}

/*
 * 根据ID生成场景并返回
 * IMPORTANT：场景由MCSceneManager管理，主意内存泄漏！
 */
MCGameScene *
MCSceneManager::sceneWithObjectId(mc_object_id_t anObjectId)
{
    int key = MCObjectIdToDickKey(anObjectId);
    MCGameScene *scene = (MCGameScene *) scenes_->objectForKey(key);
    
    if (scene == NULL) {
        scene = new MCGameScene;
        if (scene && scene->initWithScenePackage((MCScenePackage *) scenePackages_->objectForKey(key))) {
            scenes_->setObject(scene, key);
        } else {
            CC_SAFE_DELETE(scene);
            scene = NULL;
        }
    }
    
    return scene;
}

/*
 * 清除由sceneWithObjectId:方法生成的MCGameScene对象
 */
void
MCSceneManager::cleanupSceneWithObjectId(mc_object_id_t anObjectId)
{
    int key = MCObjectIdToDickKey(anObjectId);
    MCGameScene *scene = (MCGameScene *) scenes_->objectForKey(key);
    if (scene) {
        scenes_->removeObjectForKey(key);
    }
}
