//
//  MCItemManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCItemManager.h"
#include "MCEquipmentItem.h"
#include "MCEffectiveItem.h"
#include "JsonBox.h"
#include "MCOreManager.h"

const char *kMCEquipmentItemWeaponFilepath = "E001.jpkg";
const char *kMCEquipmentItemArmorFilepath = "E002.jpkg";
const char *kMCEffectiveItemFilepath = "E100.jpkg";

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
    MCItem *item = (MCItem *) metaItemForObjectId(anObjectId)->copy();
    
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
    MCEquipmentItem *equipmentItem = (MCEquipmentItem *) metaEquipmentItemForObjectId(anObjectId)->copy();

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
    MCEffectiveItem *effectiveItem = (MCEffectiveItem *) metaEffectiveItemForObjectId(anObjectId)->copy();
    
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
MCItemManager::metaItemForObjectId(mc_object_id_t anObjectId)
{
    MCItem *item;
    
    item = equipmentItemForObjectId(anObjectId);
    if (item == NULL) {
        item = effectiveItemForObjectId(anObjectId);
    }
    
    return item;
}

MCEquipmentItem *
MCItemManager::metaEquipmentItemForObjectId(mc_object_id_t anObjectId)
{
    MCEquipmentItem *equipmentItem;

    equipmentItem = (MCEquipmentItem *) equipmentItems_->objectForKey(MCObjectIdToDickKey(anObjectId));
    
    return equipmentItem;
}

MCEffectiveItem *
MCItemManager::metaEffectiveItemForObjectId(mc_object_id_t anObjectId)
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
    weapon.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(kMCEquipmentItemWeaponFilepath));
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
        MCEquipmentItem *item = MCEquipmentItem::create(MCWeapon);
        CCString *ccstring;
        
        item->setID(o_id);
        ccstring = CCString::create(object["name"].getString().c_str());
        item->setName(ccstring);
        ccstring->retain();
        ccstring = CCString::create(object["icon"].getString().c_str());
        item->setIcon(ccstring);
        ccstring->retain();
        item->setPrice(object["price"].getInt());
        item->equipment_.equipment.weapon.damage = MCMakeDiceType(object["count"].getInt(), object["size"].getInt());
        JsonBox::Object diceRange = object["critical-hit-visible"].getObject();
        JsonBox::Object diceRangeDice = diceRange["dice"].getObject();
        item->equipment_.equipment.weapon.critical_hit_visible.min = diceRange["min"].getInt();
        item->equipment_.equipment.weapon.critical_hit_visible.max = diceRange["max"].getInt();
        item->equipment_.equipment.weapon.critical_hit_visible.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                                                     diceRangeDice["size"].getInt());
        diceRange = object["critical-hit-invisible"].getObject();
        diceRangeDice = diceRange["dice"].getObject();
        item->equipment_.equipment.weapon.critical_hit_invisible.min = diceRange["min"].getInt();
        item->equipment_.equipment.weapon.critical_hit_invisible.max = diceRange["max"].getInt();
        item->equipment_.equipment.weapon.critical_hit_invisible.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                                                       diceRangeDice["size"].getInt());
        item->equipment_.equipment.weapon.critical_hit = object["critical-hit"].getDouble();
        item->equipment_.equipment.weapon.distance = object["distance"].getInt();
        if (object["effect"].isInteger()) {
            item->equipment_.equipment.weapon.effect = object["effect"].getInt();
            diceRange = object["effect-check"].getObject();
            diceRangeDice = diceRange["dice"].getObject();
            item->equipment_.equipment.weapon.effect_check.min = diceRange["min"].getInt();
            item->equipment_.equipment.weapon.effect_check.max = diceRange["max"].getInt();
            item->equipment_.equipment.weapon.effect_check.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                                                 diceRangeDice["size"].getInt());
        }
        item->equipment_.equipment.weapon.dexterity = object["dexterity"].getInt();
        item->equipment_.type = MCWeapon;
        /* 读取默认矿石，加载背包的时候更新为正确矿石 */
        item->ore_ = oreManager->defaultOre();
        equipmentItems_->setObject(item, MCObjectIdToDickKey(o_id));
    }
    
    /* 读取防具 */
    armor.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(kMCEquipmentItemArmorFilepath));
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
        MCEquipmentItem *item = MCEquipmentItem::create(MCArmor);
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
        item->setPrice(object["price"].getInt());
        item->equipment_.equipment.armor.defense = object["defense"].getInt();
        item->equipment_.equipment.armor.dexterity = object["dexterity"].getInt();
        item->equipment_.equipment.armor.armor_check_penalty = object["armor-check-penalty"].getInt();
        item->equipment_.type = MCArmor;
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
    
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(kMCEffectiveItemFilepath));
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
