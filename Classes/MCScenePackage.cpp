//
//  MCScenePackage.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCScenePackage.h"
#include "MCRoleManager.h"
#include "MCEntrance.h"

MCScenePackage::~MCScenePackage()
{
    CC_SAFE_RELEASE(NPCs_);
    CC_SAFE_RELEASE(monsters_);
    CC_SAFE_RELEASE(scenes_);
    
    CC_SAFE_RELEASE(tmxTiledMapPath_);
    CC_SAFE_RELEASE(backgroundMusicPath_);

    MCObject::~MCObject();
}

bool
MCScenePackage::init()
{
    do {
        NPCs_ = CCDictionary::create();
        CC_BREAK_IF(! NPCs_);
        NPCs_->retain();
        
        monsters_ = CCDictionary::create();
        CC_BREAK_IF(! monsters_);
        monsters_->retain();
        
        scenes_ = CCDictionary::create();
        CC_BREAK_IF(! monsters_);
        scenes_->retain();
        
        tmxTiledMapPath_ = NULL;
        backgroundMusicPath_ = NULL;
        
        return true;
    } while (0);
    
    return false;
}

MCScenePackage *
MCScenePackage::create(const char *aPackagePath)
{
    MCScenePackage *package = new MCScenePackage;
    
    if (package && package->init()) {
        package->loadFromFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(aPackagePath));
        package->autorelease();
        return package;
    } else {
        CC_SAFE_DELETE(package);
    }
    
    return NULL;
}

MCNPC *
MCScenePackage::NPCForObjectId(mc_object_id_t anObjectId)
{
    return (MCNPC *) NPCs_->objectForKey(MCObjectIdToDickKey(anObjectId));
}

MCMonster *
MCScenePackage::monsterForObjectId(mc_object_id_t anObjectId)
{
    return (MCMonster *) monsters_->objectForKey(MCObjectIdToDickKey(anObjectId));
}

void
MCScenePackage::loadFromFile(const char *aPackagePath)
{
    JsonBox::Value in;
    JsonBox::Object root;
    JsonBox::Object object;
    const char *c_str_o_id;
    
    in.loadFromFile(aPackagePath);
    root = in.getObject();
    
    /* ID String */
    c_str_o_id = root["id"].getString().c_str();
    mc_object_id_t o_id = {
        c_str_o_id[0],
        c_str_o_id[1],
        c_str_o_id[2],
        c_str_o_id[3]
    };
    setID(o_id);
    
    /* name String */
    name_ = CCString::create(root["name"].getString().c_str());
    name_->retain();
    
    /* objects Object */
    loadObjects(root);
    
    /* dialogues Object */
    loadDialogues(root);
    
    /* background Object */
    object = root["background"].getObject();
    /* background["tmx"] String */
    tmxTiledMapPath_ = CCString::create(object["tmx"].getString());
    tmxTiledMapPath_->retain();
    /* background["sound"] String */
    backgroundMusicPath_ = CCString::create(object["sound"].getString());
    backgroundMusicPath_->retain();
    
    /* scenes Object */
    loadScenes(root);
}

void
MCScenePackage::loadObjects(JsonBox::Object &aRoot)
{
    JsonBox::Object objects = aRoot["objects"].getObject();
    JsonBox::Value v;
    JsonBox::Array array;
    JsonBox::Array::iterator arrayIter;
    JsonBox::Object::iterator objectIter;
    const char *c_str_o_id;
    MCRoleManager *roleManager = MCRoleManager::sharedRoleManager();

    /* objects["NPCs"] Array */
    array = objects["NPCs"].getArray();
    /* load NPCs */
    for (arrayIter = array.begin(); arrayIter != array.end(); ++arrayIter) {
        v = *arrayIter;
        c_str_o_id = v.getString().c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        MCNPC *npc = roleManager->NPCForObjectId(o_id);
        if (npc) {
            NPCs_->setObject(npc, MCObjectIdToDickKey(o_id));
        }
    }
    /* objects["monsters"] Array */
    array = objects["monsters"].getArray();
    /* load monsters */
    for (arrayIter = array.begin(); arrayIter != array.end(); ++arrayIter) {
        v = *arrayIter;
        c_str_o_id = v.getString().c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        MCMonster *monster = roleManager->monsterForObjectId(o_id);
        if (monster) {
            monsters_->setObject(monster, MCObjectIdToDickKey(o_id));
        }
    }
}

void
MCScenePackage::loadDialogues(JsonBox::Object &aRoot)
{
    JsonBox::Object dialogues = aRoot["dialogues"].getObject();
    JsonBox::Value v;
    JsonBox::Object object;
//    const char *c_str_o_id;
}

void
MCScenePackage::loadScenes(JsonBox::Object &aRoot)
{
    JsonBox::Object scenes = aRoot["scenes"].getObject();
    JsonBox::Object::iterator objectIterator;
    MCEntrance *entrance;
    const char *c_str_o_id;
    
    for (objectIterator = scenes.begin(); objectIterator != scenes.end(); ++objectIterator) {
        entrance = new MCEntrance;
        entrance->setName(CCString::create(objectIterator->first.c_str()));
        c_str_o_id = objectIterator->second.getString().c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        entrance->setID(o_id);
        scenes_->setObject(entrance, entrance->getName()->getCString());
    }
}
