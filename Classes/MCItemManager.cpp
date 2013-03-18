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

#warning debug, should delete ".json"
const char *kMCEquipmentItemFilepath = "equipments.epkg.json";
const char *kMCEffectiveItemFilepath = "effectives.epkg.json";

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
    
    if (equipmentItem && equipmentItem->init()) {
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
    
    if (effectiveItem && effectiveItem->init()) {
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
    JsonBox::Value v;
    JsonBox::Object root;
    
    v.loadFromFile(kMCEquipmentItemFilepath);
    root = v.getObject();
}

void
MCItemManager::loadEffectiveItems()
{
    
}