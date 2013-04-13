//
//  MCItemManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCType.h"
#include "MCItemManager.h"
#include "MCEquipmentItem.h"
#include "MCEffectiveItem.h"
#include "JsonBox.h"
#include "MCBackpack.h"
#include "MCOreManager.h"

static const char *kMCEquipmentItemWeaponFilepath = "E001.jpkg";
static const char *kMCEquipmentItemArmorFilepath = "E002.jpkg";
static const char *kMCEffectiveItemFilepath = "E100.jpkg";

static MCItemManager *__shared_item_manager = NULL;

MCItemManager::MCItemManager()
{
    equipmentItems_ = CCDictionary::create();
    equipmentItems_->retain();
    effectiveItems_ = CCDictionary::create();
    effectiveItems_->retain();
}

MCItemManager::~MCItemManager()
{
    CC_SAFE_RELEASE(equipmentItems_);
    CC_SAFE_RELEASE(effectiveItems_);
}

MCItemManager *
MCItemManager::sharedItemManager()
{
    if (__shared_item_manager == NULL) {
        __shared_item_manager = new MCItemManager;
        __shared_item_manager->loadAllItems();
    }
    
    return __shared_item_manager;
}

/* copied data */
MCItem *
MCItemManager::itemForObjectId(mc_object_id_t anObjectId)
{
    MCItem *item = (MCItem *) protoItemForObjectId(anObjectId)->copy();
    
    if (item && item->init()) {
        item->autorelease();
    } else {
        CC_SAFE_DELETE(item);
        item = NULL;
    }
    
    return item;
}

MCEquipmentItem *
MCItemManager::equipmentItemForObjectId(mc_object_id_t anObjectId)
{
    MCEquipmentItem *meta = protoEquipmentItemForObjectId(anObjectId);
    MCEquipmentItem *equipmentItem = dynamic_cast<MCEquipmentItem *>(meta->copy());

    if (equipmentItem) {
        equipmentItem->autorelease();
    } else {
        CC_SAFE_DELETE(equipmentItem);
        equipmentItem = NULL;
    }
    
    return equipmentItem;
}

MCEffectiveItem *
MCItemManager::effectiveItemForObjectId(mc_object_id_t anObjectId)
{
    MCEffectiveItem *meta = protoEffectiveItemForObjectId(anObjectId);
    MCEffectiveItem *effectiveItem = dynamic_cast<MCEffectiveItem *>(meta->copy());
    
    if (effectiveItem) {
        effectiveItem->autorelease();
    } else {
        CC_SAFE_DELETE(effectiveItem);
        effectiveItem = NULL;
    }
    
    return effectiveItem;
}

/* metadata */
MCItem *
MCItemManager::protoItemForObjectId(mc_object_id_t anObjectId)
{
    MCItem *item;
    
    item = equipmentItemForObjectId(anObjectId);
    if (item == NULL) {
        item = effectiveItemForObjectId(anObjectId);
    }
    
    return item;
}

MCEquipmentItem *
MCItemManager::protoEquipmentItemForObjectId(mc_object_id_t anObjectId)
{
    MCEquipmentItem *equipmentItem;

    equipmentItem = (MCEquipmentItem *) equipmentItems_->objectForKey(MCObjectIdToDickKey(anObjectId));
    
    return equipmentItem;
}

MCEffectiveItem *
MCItemManager::protoEffectiveItemForObjectId(mc_object_id_t anObjectId)
{
    MCEffectiveItem *effectiveItem;
    
    effectiveItem = (MCEffectiveItem *) effectiveItems_->objectForKey(MCObjectIdToDickKey(anObjectId));
    
    return effectiveItem;
}

void
MCItemManager::loadAllItems()
{
    loadEquipmentItems();
    loadEffectiveItems();
}

void
MCItemManager::loadEquipmentItems()
{
    JsonBox::Value weapon;
    JsonBox::Value armor;
    JsonBox::Object root;
    JsonBox::Object::iterator rootIterator;
    MCOreManager *oreManager = MCOreManager::sharedOreManager();
    
    /* 读取武器 */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCEquipmentItemWeaponFilepath);
    if (pstrFileContent) {
        weapon.loadFromString(pstrFileContent->getCString());
    }
#else
    weapon.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCEquipmentItemWeaponFilepath).c_str());
#endif
    
    root = weapon.getObject();
    for (rootIterator = root.begin(); rootIterator != root.end(); ++rootIterator) {
        const char *c_str_o_id = rootIterator->first.c_str();
        JsonBox::Object object = rootIterator->second.getObject();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        MCEquipmentItem *item = MCEquipmentItem::create(MCEquipment::MCWeapon);
        CCString *ccstring;
        
        item->setID(o_id);
        ccstring = CCString::create(object["name"].getString().c_str());
        item->setName(ccstring);
        ccstring->retain();
        ccstring = CCString::create(object["icon"].getString().c_str());
        item->setIcon(ccstring);
        ccstring->retain();
        item->setPrice(object["price"].getInt());
        JsonBox::Object damage = object["damage"].getObject();
        MCWeapon *equipment = dynamic_cast<MCWeapon *>(item->equipment_);
        equipment->damage = MCMakeDiceType(damage["count"].getInt(), damage["size"].getInt());
        equipment->criticalHit = object["critical-hit"].getInt();
        JsonBox::Object diceRange = object["critical-hit-visible"].getObject();
        JsonBox::Object diceRangeDice = diceRange["dice"].getObject();
        equipment->criticalHitVisible.min = diceRange["min"].getInt();
        equipment->criticalHitVisible.max = diceRange["max"].getInt();
        equipment->criticalHitVisible.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                              diceRangeDice["size"].getInt());
        diceRange = object["critical-hit-invisible"].getObject();
        diceRangeDice = diceRange["dice"].getObject();
        equipment->criticalHitInvisible.min = diceRange["min"].getInt();
        equipment->criticalHitInvisible.max = diceRange["max"].getInt();
        equipment->criticalHitInvisible.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                              diceRangeDice["size"].getInt());
        equipment->distance = object["distance"].getInt();
        if (object["effect"].isInteger()) {
            equipment->effect = object["effect"].getInt();
            diceRange = object["effect-check"].getObject();
            diceRangeDice = diceRange["dice"].getObject();
            equipment->effectCheck.min = diceRange["min"].getInt();
            equipment->effectCheck.max = diceRange["max"].getInt();
            equipment->effectCheck.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                          diceRangeDice["size"].getInt());
        }
        equipment->dexterity = object["dexterity"].getInt();
        /* 读取默认矿石，加载背包的时候更新为正确矿石 */
        item->ore_ = oreManager->defaultOre();
        equipmentItems_->setObject(item, MCObjectIdToDickKey(o_id));
    }
    
    /* 读取防具 */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    pstrFileContent = CCString::createWithContentsOfFile(kMCEquipmentItemArmorFilepath);
    if (pstrFileContent) {
        armor.loadFromString(pstrFileContent->getCString());
    }
#else
    armor.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCEquipmentItemArmorFilepath).c_str());
#endif
    
    root = armor.getObject();
    for (rootIterator = root.begin(); rootIterator != root.end(); ++rootIterator) {
        const char *c_str_o_id = rootIterator->first.c_str();
        JsonBox::Object object = rootIterator->second.getObject();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        MCEquipmentItem *item = MCEquipmentItem::create(MCEquipment::MCArmor);
        CCString *ccstring;
        
        item->setID(o_id);
        ccstring = CCString::create(object["name"].getString().c_str());
        item->setName(ccstring);
        ccstring->retain();
        ccstring = CCString::create(object["description"].getString().c_str());
        item->setDescription(ccstring);
        ccstring->retain();
        ccstring = CCString::create(object["icon"].getString().c_str());
        item->setIcon(ccstring);
        ccstring->retain();
        item->setPrice(object["price"].getInt());
        MCArmor *equipment = dynamic_cast<MCArmor *>(item->equipment_);
        equipment->defense = object["defense"].getInt();
        equipment->dexterity = object["dexterity"].getInt();
        equipment->armorCheckPenalty = object["armor-check-penalty"].getInt();
        /* 读取默认矿石，加载背包的时候更新为正确矿石 */
        item->ore_ = oreManager->defaultOre();
        equipmentItems_->setObject(item, MCObjectIdToDickKey(o_id));
    }
}

void
MCItemManager::loadEffectiveItems()
{
    JsonBox::Value v;
    JsonBox::Object root;
    JsonBox::Object::iterator rootIterator;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCEffectiveItemFilepath);
    if (pstrFileContent) {
        v.loadFromString(pstrFileContent->getCString());
    }
#else
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCEffectiveItemFilepath).c_str());
#endif
    
    root = v.getObject();
    for (rootIterator = root.begin(); rootIterator != root.end(); ++rootIterator) {
        const char *c_str_o_id = rootIterator->first.c_str();
        JsonBox::Object object = rootIterator->second.getObject();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        MCEffectiveItem *item = MCEffectiveItem::create();
        CCString *ccstring;
        
        item->setID(o_id);
        ccstring = CCString::create(object["name"].getString().c_str());
        item->setName(ccstring);
        ccstring->retain();
        ccstring = CCString::create(object["description"].getString().c_str());
        item->setDescription(ccstring);
        ccstring->retain();
        ccstring = CCString::create(object["icon"].getString().c_str());
        item->setIcon(ccstring);
        ccstring->retain();
        ccstring = CCString::create(object["path"].getString().c_str());
        item->setPath(ccstring);
        ccstring->retain();
        item->setPrice(object["price"].getInt());
        item->setRadius(object["radius"].getInt());
        item->effect_.hp = object["hp"].getInt();
        item->effect_.pp = object["pp"].getInt();
        item->effect_.positive_state = object["positive-state"].getInt();
        item->effect_.negative_state = object["negative-state"].getInt();
        item->effect_.lasting_time = object["lasting-time"].getDouble();
        effectiveItems_->setObject(item, MCObjectIdToDickKey(o_id));
    }
}
