//
//  MCEquipmentManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include <string.h>
#include <sstream>
using namespace std;

#include "JsonBox.h"
#include "MCBase64.h"
#include "MCEquipmentManager.h"
#include "MCItemManager.h"
#include "MCGameState.h"

#if MC_DEBUG_SAVEDATA == 1
static const char *kMCEquipmentManagerKey = "equipment-manager";
static const char *kMCCurrentWeaponKey = "current-weapon";
static const char *kMCEquipmentItemsKey = "equipment-items";
#else
static const char *kMCEquipmentManagerKey = "ZXF1aXBtZW50LW1hbmFnZXI"; /* equipment-manager的BASE64编码没有最后的= */
static const char *kMCCurrentWeaponKey = "Y3VycmVudC13ZWFwb24"; /* current-weapon的BASE64编码没有最后的= */
static const char *kMCEquipmentItemsKey = "ZXF1aXBtZW50LWl0ZW1z"; /* equipment-items的BASE64编码 */
#endif

static MCEquipmentManager *__shared_equipment_manager = NULL;

/* 装备 */
mc_object_id_t equipmentsOID[] = {
    {'W', '1', '1', '0'},
    {'W', '1', '2', '0'},
    {'W', '1', '3', '0'},
    {'W', '2', '1', '0'},
    {'W', '2', '2', '0'},
    {'W', '3', '1', '0'},
    {'W', '3', '2', '0'},
    {'W', '3', '3', '0'},
    {'W', '4', '1', '0'},
    {'W', '4', '2', '0'},
    {'W', '4', '3', '0'},
    {'W', '5', '1', '0'},
    {'W', '5', '2', '0'},
    
    {'H', '0', '0', '1'},
    {'A', '0', '0', '1'},
    {'B', '0', '0', '1'}
};

mc_object_id_t
MCEquipmentObjectItem(MCEquipmentIndex anIndex)
{
    CCAssert(anIndex >= 0 && anIndex <= kMCShinGuard, "out of index!");
    
    return equipmentsOID[anIndex];
}

MCEquipmentManager::MCEquipmentManager()
{
    currentWeapon_ = NULL;
    weapons_ = CCArray::create();
    weapons_->retain();
    armors_ = CCArray::create();
    armors_->retain();
}

MCEquipmentManager::~MCEquipmentManager()
{
    CC_SAFE_RELEASE(armors_);
    CC_SAFE_RELEASE(weapons_);
    /* 装备 */
    /* 数据就按这个顺序储存 */
    /* 武器 */
    /* 剑类 */
    CC_SAFE_RELEASE(dagger_); /* 短剑 */
    CC_SAFE_RELEASE(sword_); /* 长剑 */
    CC_SAFE_RELEASE(greatsword_); /* 巨剑 */
    /* 锤类 */
    CC_SAFE_RELEASE(warhammer_); /* 轻型战锤 */
    CC_SAFE_RELEASE(heavyDutyHammer_); /* 重型战锤 */
    /* 斧类 */
    CC_SAFE_RELEASE(handAxe_); /* 手斧 */
    CC_SAFE_RELEASE(warAxe_); /* 战斧 */
    CC_SAFE_RELEASE(greataxe_); /* 巨斧 */
    /* 枪矛类 */
    CC_SAFE_RELEASE(spear_); /* 长枪 */
    CC_SAFE_RELEASE(lance_); /* 长矛 */
    CC_SAFE_RELEASE(giantSickle_); /* 巨镰 */
    /* 弓弩类 */
    CC_SAFE_RELEASE(shortbow_); /* 短弓 */
    CC_SAFE_RELEASE(longbow_); /* 长弓 */
    
    /* 防具 */
    CC_SAFE_RELEASE(helmet_); /* 头盔 */
    CC_SAFE_RELEASE(armor_); /* 铠甲 */
    CC_SAFE_RELEASE(shinGuard_); /* 护胫 */
}

MCEquipmentManager *
MCEquipmentManager::sharedEquipmentManager()
{
    if (__shared_equipment_manager == NULL) {
        __shared_equipment_manager = new MCEquipmentManager;
    }
    
    return __shared_equipment_manager;
}

mc_ssize_t
MCEquipmentManager::levelUp(MCEquipmentItem *anEquipment)
{
    MCOre *currentOre = anEquipment->getOre();
    MCOre *nextLevelOre = currentOre->getNextLevel();
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    mc_price_t money = backpack->getMoney();
    
    if (nextLevelOre) {
        if (nextLevelOre->getPrice() + anEquipment->getPrice() > money) {
            return kMCNotEnoughMoney;
        }
        money -= (nextLevelOre->getPrice() + anEquipment->getPrice());
        backpack->setMoney(money);
        
        anEquipment->setOre(nextLevelOre);
        /* 重置计算 */
        anEquipment->ac_ = -1;
        anEquipment->armorCheckPenalty_ = -1;
        anEquipment->attackCheck_ = -1;
        anEquipment->damage_ = -1;
        anEquipment->criticalHit_ = -1;
        anEquipment->damageScore_ = -1;
        
        return kMCHandleSucceed;
    }
    
    return kMCFullLevel;
}

void
MCEquipmentManager::saveData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    
    saveEquipmentItems();
    
    JsonBox::Object equipmentManager;
    
    /* 储存当前使用的武器 */
    char o_id_buffer[5] = {0};
    char *c_str_o_id = o_id_buffer;
    mc_object_id_t o_id = currentWeapon_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    equipmentManager[kMCCurrentWeaponKey] = JsonBox::Value(c_str_o_id);
    
    JsonBox::Value equipmentManagerValue(equipmentManager);
    ostringstream outputStream;
    equipmentManagerValue.writeToStream(outputStream);
    string data = outputStream.str();
#if MC_DEBUG_SAVEDATA == 1
    const char *output = data.c_str();
#else
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
    userDefault->setStringForKey(kMCEquipmentManagerKey, output);
#if MC_DEBUG_SAVEDATA != 1
    delete []output;
#endif
}

void
MCEquipmentManager::loadData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    MCEquipmentItem *weapon = NULL;
    
    loadEquipmentItems();
    
    string data = userDefault->getStringForKey(kMCEquipmentManagerKey, "");
    if (MCGameState::sharedGameState()->isSaveFileExists() && data.size() > 0) {
#if MC_DEBUG_SAVEDATA == 1
        const char *output = data.c_str();
#else
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
        JsonBox::Value v;
        v.loadFromString(output);
        
        JsonBox::Object backpack;
        /* 加载当前使用的武器 */
        const char *c_str_o_id = backpack[kMCCurrentWeaponKey].getString().c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCSword])) {
            weapon = sword_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCGreatsword])) {
            weapon = greatsword_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCWarhammer])) {
            weapon = warhammer_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCHeavyDutyHammer])) {
            weapon = heavyDutyHammer_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCHandAxe])) {
            weapon = handAxe_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCWarAxe])) {
            weapon = warAxe_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCGreataxe])) {
            weapon = greataxe_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCSpear])) {
            weapon = spear_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCLance])) {
            weapon = lance_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCGiantSickle])) {
            weapon = giantSickle_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCShortbow])) {
            weapon = shortbow_;
        } else if (MCObjectIdIsEqualsTo(o_id, equipmentsOID[kMCLongbow])) {
            weapon = longbow_;
        }
    }
    if (weapon == NULL) {
        weapon = dagger_;
    }
    currentWeapon_ = weapon;
}

void
MCEquipmentManager::saveEquipmentItems()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    JsonBox::Object equipmentItems;
    
    /* 装备 */
    char o_id_buffer[5] = {0};
    char ore_id_buffer[5] = {0};
    char *c_str_o_id = o_id_buffer;
    char *c_str_ore_id = ore_id_buffer;
    /* dagger_ */
    mc_object_id_t o_id = dagger_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    mc_object_id_t ore_id = dagger_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* sword_ */
    o_id = sword_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = sword_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* greatsword_ */
    o_id = greatsword_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = greatsword_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* warhammer_ */
    o_id = warhammer_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = warhammer_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* heavyDutyHammer_ */
    o_id = heavyDutyHammer_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = heavyDutyHammer_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* handAxe_ */
    o_id = handAxe_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = handAxe_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* warAxe_ */
    o_id = warAxe_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = warAxe_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* greataxe_ */
    o_id = greataxe_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = greataxe_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* spear_ */
    o_id = spear_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = spear_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* lance_ */
    o_id = lance_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = lance_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* giantSickle_ */
    o_id = giantSickle_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = giantSickle_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* shortbow_ */
    o_id = shortbow_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = shortbow_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* longbow_ */
    o_id = longbow_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = longbow_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* helmet_ */
    o_id = helmet_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = helmet_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* armor_ */
    o_id = armor_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = armor_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* shinGuard_ */
    o_id =shinGuard_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = shinGuard_->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    
    JsonBox::Value effectiveItemsValue(equipmentItems);
    ostringstream outputStream;
    effectiveItemsValue.writeToStream(outputStream);
    string data = outputStream.str();
#if MC_DEBUG_SAVEDATA == 1
    const char *output = data.c_str();
#else
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
    userDefault->setStringForKey(kMCEquipmentItemsKey, output);
#if MC_DEBUG_SAVEDATA != 1
    delete []output;
#endif
}

void
MCEquipmentManager::loadEquipmentItems()
{
    MCItemManager *itemManager = MCItemManager::sharedItemManager();
    MCOreManager *oreManager = MCOreManager::sharedOreManager();
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    string data = userDefault->getStringForKey(kMCEquipmentItemsKey, "");
    
    dagger_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCDagger]);
    weapons_->addObject(dagger_);
    sword_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCSword]);
    weapons_->addObject(sword_);
    greatsword_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCGreatsword]);
    weapons_->addObject(greatsword_);
    warhammer_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCWarhammer]);
    weapons_->addObject(warhammer_);
    heavyDutyHammer_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCHeavyDutyHammer]);
    weapons_->addObject(heavyDutyHammer_);
    handAxe_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCHandAxe]);
    weapons_->addObject(handAxe_);
    warAxe_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCWarAxe]);
    weapons_->addObject(warAxe_);
    greataxe_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCGreataxe]);
    weapons_->addObject(greataxe_);
    spear_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCSpear]);
    weapons_->addObject(spear_);
    lance_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCLance]);
    weapons_->addObject(lance_);
    giantSickle_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCGiantSickle]);
    weapons_->addObject(giantSickle_);
    shortbow_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCShortbow]);
    weapons_->addObject(shortbow_);
    longbow_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCLongbow]);
    weapons_->addObject(longbow_);
    helmet_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCHelmet]);
    armors_->addObject(helmet_);
    armor_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCArmor]);
    armors_->addObject(armor_);
    shinGuard_ = itemManager->equipmentItemForObjectId(equipmentsOID[kMCShinGuard]);
    armors_->addObject(shinGuard_);
    
    if (MCGameState::sharedGameState()->isSaveFileExists() && data.size() > 0) {
#if MC_DEBUG_SAVEDATA == 1
        const char *output = data.c_str();
#else
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
        JsonBox::Value v;
        v.loadFromString(output);
        
        JsonBox::Object equipmentItems;
        JsonBox::Object::iterator equipmentItemsIterator;
        for (equipmentItemsIterator = equipmentItems.begin();
             equipmentItemsIterator != equipmentItems.end();
             ++equipmentItemsIterator) {
            const char *c_str_o_id = equipmentItemsIterator->first.c_str();
            mc_object_id_t o_id = {
                c_str_o_id[0],
                c_str_o_id[1],
                c_str_o_id[2],
                c_str_o_id[3]
            };
            const char *c_str_ore_id = equipmentItemsIterator->second.getString().c_str();
            mc_object_id_t ore_id = {
                c_str_ore_id[0],
                c_str_ore_id[1],
                c_str_ore_id[2],
                c_str_ore_id[3]
            };
            itemManager->equipmentItemForObjectId(o_id)->setOre(oreManager->oreForObjectId(ore_id));
        }
#if MC_DEBUG_SAVEDATA != 1
        delete []output;
#endif
    }
}
