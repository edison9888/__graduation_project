//
//  MCSceneManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSceneManager.h"

#include "JsonBox.h"
#include "MCGameScene.h"
#include "MCBattleFieldScene.h"
#include "MCFlagManager.h"
#include "MCTaskManager.h"

const int kMCQueueMaxSize = 4;

static MCScene *
MCSceneMake(MCScenePackageType aScenePackageType) {
    CCAssert(MCUnknownPackage != aScenePackageType, "unknown scene package type");
    MCScene *scene;
    
    if (MCFlagManager::sharedFlagManager()->isTaskStarted()
        && MCBattleFieldScenePackage == aScenePackageType) {
        scene = new MCBattleFieldScene;
    } else {
        scene = new MCGameScene;
    }
    
    return scene;
};

const char *kMCScenesResourceFilePath = "S000.jpkg";
static MCSceneManager *__shared_scene_manager = NULL;

MCSceneManager::MCSceneManager()
{
    scenes_ = CCDictionary::create();
    scenes_->retain();
    scenePackages_ = CCDictionary::create();
    scenePackages_->retain();
}

MCSceneManager::~MCSceneManager()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kMCTaskDidFinishNotification);
    CC_SAFE_RELEASE(scenes_);
    CC_SAFE_RELEASE(scenePackages_);
}

MCSceneManager *
MCSceneManager::sharedSceneManager()
{
    if (__shared_scene_manager == NULL) {
        __shared_scene_manager = new MCSceneManager;
        if (__shared_scene_manager) {
            __shared_scene_manager->loadSceneListFile();
            
            CCNotificationCenter *notificationCenter = CCNotificationCenter::sharedNotificationCenter();
            notificationCenter->addObserver(__shared_scene_manager,
                                            callfuncO_selector(MCSceneManager::taskDidFinish),
                                            kMCTaskDidFinishNotification,
                                            NULL);
        }
    }
    
    return __shared_scene_manager;
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
MCScene *
MCSceneManager::sceneWithObjectId(mc_object_id_t anObjectId)
{
    mc_dict_key_t key = MCObjectIdToDickKey(anObjectId);
    MCScene *scene = (MCScene *) scenes_->objectForKey(key);
    MCScenePackage *scenePackage;
    
    if (scene == NULL) {
        scenePackage = (MCScenePackage *) scenePackages_->objectForKey(key);
        scene = MCSceneMake(scenePackage->getScenePackageType());
        if (scene && scene->initWithScenePackage(scenePackage)) {
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
    MCScene *scene = (MCScene *) scenes_->objectForKey(key);
    if (scene) {
        scenes_->removeObjectForKey(key);
        scene->cleanup();
    }
}

/* 自动清理场景 */
void
MCSceneManager::autoreleaseSceneWithObjectId(mc_object_id_t anObjectId)
{
    bool found = false;
    for (std::vector<mc_object_id_t>::iterator iterator = trash_.begin();
         iterator != trash_.end();
         ++iterator) {
        mc_object_id_t oid = *iterator;
        if (MCObjectIdIsEqualsTo(oid, anObjectId)) {
            found = true;
            break;
        }
    }
    
    if (! found) {
        trash_.push_back(anObjectId);
    }
    if (trash_.size() > kMCQueueMaxSize) {
        cleanupSceneWithObjectId(trash_.front());
        trash_.erase(trash_.begin());
    }
}

/* 自动清理任务场景 */
void
MCSceneManager::autoreleaseTaskSceneWithObjectId(mc_object_id_t anObjectId)
{
    taskTrash_.push_back(anObjectId);
}

/**
 * 任务完成通知
 */
void
MCSceneManager::taskDidFinish(CCObject *obj)
{
    for (std::vector<mc_object_id_t>::iterator iterator = taskTrash_.begin();
         iterator != taskTrash_.end();
         ++iterator) {
        cleanupSceneWithObjectId(*iterator);
    }
    taskTrash_.clear();
}

void
MCSceneManager::loadSceneListFile()
{
    JsonBox::Value in;
    JsonBox::Value v;
    JsonBox::Object packages;
    JsonBox::Object::iterator iter;
    const char *c_str_o_id;
    MCScenePackage *scenaPackage;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCScenesResourceFilePath);
    if (pstrFileContent) {
        in.loadFromString(pstrFileContent->getCString());
    }
#else
    in.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCScenesResourceFilePath).c_str());
#endif
    
    packages = in.getObject();
    
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
        scenaPackage = MCScenePackage::create(iter->second.getString().c_str());
        scenaPackage->setID(o_id);
        scenePackages_->setObject(scenaPackage,
                                  MCObjectIdToDickKey(o_id));
    }
}
