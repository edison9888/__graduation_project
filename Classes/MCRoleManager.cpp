//
//  MCRoleManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "JsonBox.h"
#include "MCRoleManager.h"
#include "MCNPC.h"
#include "MCMonster.h"

const char *kMCNPCResourceFilePath = "npcs.npkg";

static MCRoleManager *__shared_role_manager = NULL;

MCRoleManager::MCRoleManager()
{
    npcs_ = CCDictionary::create();
    npcs_->retain();
    monsters_ = CCDictionary::create();
    monsters_->retain();
}

MCRoleManager::~MCRoleManager()
{
    CC_SAFE_RELEASE(monsters_);
    CC_SAFE_RELEASE(npcs_);
}

MCRoleManager *
MCRoleManager::sharedRoleManager()
{
    if (__shared_role_manager == NULL) {
        __shared_role_manager = new MCRoleManager;
    }
    
    return __shared_role_manager;
}

void
MCRoleManager::loadData()
{
    loadNPCData();
    loadMonsterData();
}

MCRole *
MCRoleManager::roleForObjectId(mc_object_id_t anObjectId)
{
    MCRole *role;
    
    role = (MCRole *) monsterForObjectId(anObjectId);
    if (role == NULL) {
        role = (MCRole *) NPCForObjectId(anObjectId);
    }
    
    return role;
}

MCNPC *
MCRoleManager::NPCForObjectId(mc_object_id_t anObjectId)
{
    MCNPC *npc;
    
    npc = (MCNPC *) npcs_->objectForKey(MCObjectIdToDickKey(anObjectId));
    
    return npc;
}

MCMonster *
MCRoleManager::monsterForObjectId(mc_object_id_t anObjectId)
{
    MCMonster *monster;
    
    monster = (MCMonster *) monsters_->objectForKey(MCObjectIdToDickKey(anObjectId));
    
    return monster;
}

void
MCRoleManager::loadNPCData()
{
    JsonBox::Value v;
    JsonBox::Array npcs;
    JsonBox::Array::iterator npcsIterator;
    JsonBox::Object npcObject;
    MCNPC *npc;
    const char *c_str_object_id;
    mc_object_id_t object_id;
    CCString *ccstring;
    
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(kMCNPCResourceFilePath));
    npcs = v.getArray();
    for (npcsIterator = npcs.begin(); npcsIterator != npcs.end(); ++npcsIterator) {
        npcObject = npcsIterator->getObject();
        c_str_object_id = npcObject["id"].getString().c_str();
        object_id.class_ = c_str_object_id[0];
        object_id.sub_class_ = c_str_object_id[1];
        object_id.index_ = c_str_object_id[2];
        object_id.sub_index_ = c_str_object_id[3];
        npc = MCNPC::create(object_id);
        ccstring = CCString::create(npcObject["face"].getString().c_str());
        npc->setFace(ccstring);
        ccstring = CCString::create(npcObject["name"].getString().c_str());
        npc->setName(ccstring);
        ccstring = CCString::create(npcObject["spritesheet"].getString().c_str());
        npc->setSpriteSheet(ccstring);
        ccstring = CCString::create(npcObject["description"].getString().c_str());
        npc->setDescription(ccstring);
        npcs_->setObject(npc, MCObjectIdToDickKey(object_id));
    }
}

void
MCRoleManager::loadMonsterData()
{
}
