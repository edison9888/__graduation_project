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
#include "MCEnemy.h"

const char *kMCNPCResourceFilePath = "npcs.npkg";

static MCRoleManager *__shared_role_manager = NULL;

MCRoleManager::MCRoleManager()
{
    npcs_ = CCDictionary::create();
    npcs_->retain();
    enemies_ = CCDictionary::create();
    enemies_->retain();
}

MCRoleManager::~MCRoleManager()
{
    CC_SAFE_RELEASE(enemies_);
    CC_SAFE_RELEASE(npcs_);
}

MCRoleManager *
MCRoleManager::sharedRoleManager()
{
    if (__shared_role_manager == NULL) {
        __shared_role_manager = new MCRoleManager;
        if (__shared_role_manager) {
            __shared_role_manager->loadData();
        }
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
    MCRole *role = (MCRole *) metaRoleForObjectId(anObjectId)->copy();
    
    if (role && role->init()) {
        role->autorelease();
        role->loadSpriteSheet();
    } else {
        CC_SAFE_DELETE(role);
        role = NULL;
    }
    
    return role;
}

MCNPC *
MCRoleManager::NPCForObjectId(mc_object_id_t anObjectId)
{
    MCNPC *npc = (MCNPC *) metaNPCForObjectId(anObjectId)->copy();
    
    if (npc && npc->init()) {
        npc->autorelease();
        npc->loadSpriteSheet();
    } else {
        CC_SAFE_DELETE(npc);
        npc = NULL;
    }
    
    return npc;
}

MCEnemy *
MCRoleManager::enemyForObjectId(mc_object_id_t anObjectId)
{
    MCEnemy *enemy = (MCEnemy *) metaEnemyForObjectId(anObjectId)->copy();
    
    if (enemy && enemy->MCRole::init()) {
        enemy->autorelease();
        enemy->loadSpriteSheet();
    } else {
        CC_SAFE_DELETE(enemy);
        enemy = NULL;
    }
    
    return enemy;
}

MCRole *
MCRoleManager::metaRoleForObjectId(mc_object_id_t anObjectId)
{
    MCRole *role;
    
    role = (MCRole *) metaEnemyForObjectId(anObjectId);
    if (role == NULL) {
        role = (MCRole *) metaNPCForObjectId(anObjectId);
    }
    
    return role;
}

MCNPC *
MCRoleManager::metaNPCForObjectId(mc_object_id_t anObjectId)
{
    MCNPC *npc;
    
    npc = (MCNPC *) npcs_->objectForKey(MCObjectIdToDickKey(anObjectId));
    
    return npc;
}

MCEnemy *
MCRoleManager::metaEnemyForObjectId(mc_object_id_t anObjectId)
{
    MCEnemy *enemy;
    
    enemy = (MCEnemy *) enemies_->objectForKey(MCObjectIdToDickKey(anObjectId));
    
    return enemy;
}

void
MCRoleManager::loadNPCData()
{
    JsonBox::Value v;
    JsonBox::Array npcs;
    JsonBox::Array::iterator npcsIterator;
    JsonBox::Object npcObject;
    MCNPC *role;
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
        role = new MCNPC;
        CCAssert(role != NULL, "内存不足！");
        ccstring = CCString::create(npcObject["face"].getString().c_str());
        role->setFace(ccstring);
        ccstring = CCString::create(npcObject["name"].getString().c_str());
        role->setName(ccstring);
        ccstring = CCString::create(npcObject["spritesheet"].getString().c_str());
        role->setSpriteSheet(ccstring);
        ccstring = CCString::create(npcObject["description"].getString().c_str());
        role->setDescription(ccstring);
        role->setID(object_id);
        role->init();
        role->autorelease();
    //warning: 需要retain？
        role->retain();
        npcs_->setObject(role, MCObjectIdToDickKey(object_id));
    }
}

void
MCRoleManager::loadMonsterData()
{
}
