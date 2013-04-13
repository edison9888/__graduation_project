//
//  MCOreManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-20.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "JsonBox.h"
#include "MCOreManager.h"

static MCOreManager *__shared_ore_manager = NULL;

const char *kMCOresFilepath = "O000.jpkg";


MCOreManager::MCOreManager()
{
    ores_ = CCDictionary::create();
    ores_->retain();
}

MCOreManager::~MCOreManager()
{
    CC_SAFE_RELEASE(ores_);
}

MCOreManager *
MCOreManager::sharedOreManager()
{
    if (__shared_ore_manager == NULL) {
        __shared_ore_manager = new MCOreManager;
        __shared_ore_manager->loadAllOres();
    }
    
    return __shared_ore_manager;
}

MCOre *
MCOreManager::oreForObjectId(mc_object_id_t anObjectId) const
{
    return (MCOre *) ores_->objectForKey(MCObjectIdToDickKey(anObjectId));
}

MCOre *
MCOreManager::defaultOre() const
{
    mc_object_id_t o_id = {
        'O', '0', '0', '1'
    };
    
    return (MCOre *) ores_->objectForKey(MCObjectIdToDickKey(o_id));
}

/**
 * load all flags
 */
void
MCOreManager::loadAllOres()
{
    JsonBox::Value v;
    JsonBox::Object root;
    JsonBox::Object::iterator oresIterator;
    JsonBox::Object oreValueRoot;
    const char *c_str_ore_id;
    mc_object_id_t ore_id;
    MCOre *ore;
    CCString *ccstring;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCOresFilepath);
    if (pstrFileContent) {
        v.loadFromString(pstrFileContent->getCString());
    }
#else
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCOresFilepath).c_str());
#endif
    root = v.getObject();
    
    for (oresIterator = root.begin(); oresIterator != root.end(); ++oresIterator) {
        c_str_ore_id = oresIterator->first.c_str();
        oreValueRoot = oresIterator->second.getObject();
        ore_id.class_ = c_str_ore_id[0];
        ore_id.sub_class_ = c_str_ore_id[1];
        ore_id.index_ = c_str_ore_id[2];
        ore_id.sub_index_ = c_str_ore_id[3];
        ore = new MCOre;
        ore->autorelease();
        ore->setID(ore_id);
        ccstring = CCString::create(oreValueRoot["name"].getString().c_str());
        ore->setName(ccstring);
        ccstring->retain();
        ccstring = CCString::create(oreValueRoot["description"].getString().c_str());
        ore->setDescription(ccstring);
        ccstring->retain();
        ore->setDamage(oreValueRoot["damage"].getInt());
        JsonBox::Value criticalHit = oreValueRoot["critical-hit"];
        ore->setCriticalHit(criticalHit.isInteger() ? criticalHit.getInt() : criticalHit.getDouble());
        ore->setWeaponDexterity(oreValueRoot["weapon-dexterity"].getInt());
        ore->setArmorDexterity(oreValueRoot["armor-dexterity"].getInt());
        ore->setArmorCheckPenalty(oreValueRoot["armor-check-penalty"].getInt());
        ore->setPrice(oreValueRoot["price"].getInt());
        ores_->setObject(ore, MCObjectIdToDickKey(ore_id));
    }
    
    /* 设置等级关联 */
    ore_id.class_ = 'O';
    ore_id.sub_class_ = '0';
    ore_id.index_ = '0';
    ore_id.sub_index_ = ores_->count() + '0';
    mc_dict_key_t key = MCObjectIdToDickKey(ore_id);
    MCOre *tail = (MCOre *) ores_->objectForKey(key--);
    tail->setNextLevel(NULL);
    for (mc_index_t i = 1; i < ores_->count(); ++i) {
        ore = (MCOre *) ores_->objectForKey(key--);
        ore->setNextLevel(tail);
        tail = ore;
    }
}
