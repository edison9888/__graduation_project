//
//  MCScenePackage.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCScenePackage.h"

MCScenePackage::~MCScenePackage()
{
    CC_SAFE_RELEASE(NPCs_);
    CC_SAFE_RELEASE(monsters_);
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
        package->loadFromFile(aPackagePath);
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
    
    /* objects Object */
    loadObjects(root);
    
    /* dialogues Object */
    loadDialogues(root);
    
    /* background Object */
    object = root["background"].getObject();
    /* background["tmx"] String */
    tmxTiledMapPath_ = CCString::create(object["tmx"].getString());
    /* background["sound"] String */
    backgroundMusicPath_ = CCString::create(object["sound"].getString());
    
    /* maps Object */
    loadMaps(root);
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
        MCNPC *npc = MCNPC::create(o_id);
        NPCs_->setObject(npc, MCObjectIdToDickKey(o_id));
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
        MCMonster *monster = MCMonster::create(o_id);
        monsters_->setObject(monster, MCObjectIdToDickKey(o_id));
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
MCScenePackage::loadMaps(JsonBox::Object &aRoot)
{
    JsonBox::Object maps = aRoot["maps"].getObject();
    JsonBox::Value v;
    JsonBox::Object object;
    const char *c_str_o_id;
    
    /* maps["east"] String/NULL */
    v = maps["east"];
    if (v.isString()) {
        c_str_o_id = v.getString().c_str();
        eastMapId_.class_ = c_str_o_id[0];
        eastMapId_.sub_class_ = c_str_o_id[1];
        eastMapId_.index_ = c_str_o_id[2];
        eastMapId_.sub_index_ = c_str_o_id[3];
    } else { /* NULL */
        memset(&eastMapId_, 0, sizeof(eastMapId_));
    }
    /* maps["west"] String/NULL */
    v = maps["west"];
    if (v.isString()) {
        c_str_o_id = v.getString().c_str();
        westMapId_.class_ = c_str_o_id[0];
        westMapId_.sub_class_ = c_str_o_id[1];
        westMapId_.index_ = c_str_o_id[2];
        westMapId_.sub_index_ = c_str_o_id[3];
    } else { /* NULL */
        memset(&westMapId_, 0, sizeof(westMapId_));
    }
    /* maps["south"] String/NULL */
    v = maps["south"];
    if (v.isString()) {
        c_str_o_id = v.getString().c_str();
        southMapId_.class_ = c_str_o_id[0];
        southMapId_.sub_class_ = c_str_o_id[1];
        southMapId_.index_ = c_str_o_id[2];
        southMapId_.sub_index_ = c_str_o_id[3];
    } else { /* NULL */
        memset(&southMapId_, 0, sizeof(southMapId_));
    }
    /* maps["north"] String/NULL */
    v = maps["north"];
    if (v.isString()) {
        c_str_o_id = v.getString().c_str();
        northMapId_.class_ = c_str_o_id[0];
        northMapId_.sub_class_ = c_str_o_id[1];
        northMapId_.index_ = c_str_o_id[2];
        northMapId_.sub_index_ = c_str_o_id[3];
    } else { /* NULL */
        memset(&northMapId_, 0, sizeof(northMapId_));
    }
}
