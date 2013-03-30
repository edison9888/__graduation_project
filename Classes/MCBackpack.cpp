//
//  MCBackpack.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include <string.h>
#include <sstream>
using namespace std;

#include "JsonBox.h"
#include "MCBackpack.h"
#include "MCBase64.h"
#include "MCItemManager.h"
#include "MCEffectiveItem.h"

    //warning: 木有测试过

const mc_size_t kMCItemMax = 99;
const char *kMCMoneyKey = "bW9uZXk"; /* money的BASE64编码没有最后的= */
const char *kMCCurrentWeaponKey = "Y3VycmVudC13ZWFwb24"; /* current-weapon的BASE64编码没有最后的= */
const char *kMCBackpackKey = "YmFja3BhY2s"; /* backpack的BASE64编码没有最后的= */
const char *kMCEffectiveItemsKey = "ZWZmZWN0aXZlLWl0ZW1z"; /* effective-items的BASE64编码 */
const char *kMCEquipmentItemsKey = "ZXF1aXBtZW50LWl0ZW1z"; /* equipment-items的BASE64编码 */

static MCBackpack *__shared_backpack = NULL;

mc_object_id_t itemsOID[] = {
    /* 道具 */
    {'P', '0', '0', '1'},
    {'P', '0', '0', '2'},
    {'T', '0', '1', '1'},
    {'T', '0', '1', '2'},
    {'T', '0', '2', '1'},
    {'T', '0', '2', '2'},
    {'T', '0', '3', '1'},
    {'T', '0', '3', '2'},
    {'T', '0', '4', '1'},
    {'T', '0', '4', '2'},
    {'T', '0', '5', '1'},
    {'T', '0', '5', '2'},
    
    /* 装备 */
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

static vector<int>
split(string& str,const char* c)
{
    char *cstr;
    char *p;
    string stdString;
    vector<int> result;
    
    cstr = new char[str.size()+1];
    strcpy(cstr,str.c_str());
    p = strtok(cstr,c);
    while (p != NULL) {
        stdString = p;
        result.push_back(atoi(stdString.c_str()));
        p = strtok(NULL,c);
    }
    
    delete cstr;
    
    return result;
}

mc_object_id_t
itemObjectItem(MCItemIndex anIndex)
{
    CCAssert(anIndex >= 0 && anIndex <= kMCShinGuard, "out of index!");
    
    return itemsOID[anIndex];
}

MCBackpack::MCBackpack()
{
    money_ = 0;
    currentWeapon_ = NULL;
    
    /* 道具 */
    /* 数据就按这个顺序储存 */
    /* 药品 */
    healthPotion_ = new MCBackpackItem;
    physicalPotion_ = new MCBackpackItem;
    /* 陷阱 */
    fireballTrapWide_ = new MCBackpackItem;
    fireballTrapDamage_ = new MCBackpackItem;
    curseTrapWide_ = new MCBackpackItem;
    curseTrapDamage_ = new MCBackpackItem;
    paralysisTrapWide_ = new MCBackpackItem;
    paralysisTrapDamage_ = new MCBackpackItem;
    fogTrapWide_ = new MCBackpackItem;
    fogTrapDamage_ = new MCBackpackItem;
    flashTrapWide_ = new MCBackpackItem;
    flashTrapDamage_ = new MCBackpackItem;
    
    /* 装备 */
    /* 数据就按这个顺序储存 */
    /* 武器 */
    /* 剑类 */
    dagger_ = new MCBackpackItem; /* 短剑 */
    sword_ = new MCBackpackItem; /* 长剑 */
    greatsword_ = new MCBackpackItem; /* 巨剑 */
    /* 锤类 */
    warhammer_ = new MCBackpackItem; /* 轻型战锤 */
    heavyDutyHammer_ = new MCBackpackItem; /* 重型战锤 */
    /* 斧类 */
    handAxe_ = new MCBackpackItem; /* 手斧 */
    warAxe_ = new MCBackpackItem; /* 战斧 */
    greataxe_ = new MCBackpackItem; /* 巨斧 */
    /* 枪矛类 */
    spear_ = new MCBackpackItem; /* 长枪 */
    lance_ = new MCBackpackItem; /* 长矛 */
    giantSickle_ = new MCBackpackItem; /* 巨镰 */
    /* 弓弩类 */
    shortbow_ = new MCBackpackItem; /* 短弓 */
    longbow_ = new MCBackpackItem; /* 长弓 */
    
    /* 防具 */
    helmet_ = new MCBackpackItem; /* 头盔 */
    armor_ = new MCBackpackItem; /* 铠甲 */
    shinGuard_ = new MCBackpackItem; /* 护胫 */
}

MCBackpack::~MCBackpack()
{
    
    /* 道具 */
    /* 数据就按这个顺序储存 */
    /* 药品 */
    delete healthPotion_;
    delete physicalPotion_;
    /* 陷阱 */
    delete fireballTrapWide_;
    delete fireballTrapDamage_;
    delete curseTrapWide_;
    delete curseTrapDamage_;
    delete paralysisTrapWide_;
    delete paralysisTrapDamage_;
    delete fogTrapWide_;
    delete fogTrapDamage_;
    delete flashTrapWide_;
    delete flashTrapDamage_;
    
    /* 装备 */
    /* 数据就按这个顺序储存 */
    /* 武器 */
    /* 剑类 */
    delete dagger_; /* 短剑 */
    delete sword_; /* 长剑 */
    delete greatsword_; /* 巨剑 */
    /* 锤类 */
    delete warhammer_; /* 轻型战锤 */
    delete heavyDutyHammer_; /* 重型战锤 */
    /* 斧类 */
    delete handAxe_; /* 手斧 */
    delete warAxe_; /* 战斧 */
    delete greataxe_; /* 巨斧 */
    /* 枪矛类 */
    delete spear_; /* 长枪 */
    delete lance_; /* 长矛 */
    delete giantSickle_; /* 巨镰 */
    /* 弓弩类 */
    delete shortbow_; /* 短弓 */
    delete longbow_; /* 长弓 */
    
    /* 防具 */
    delete helmet_; /* 头盔 */
    delete armor_; /* 铠甲 */
    delete shinGuard_; /* 护胫 */
}

MCBackpack *
MCBackpack::sharedBackpack()
{
    if (__shared_backpack == NULL) {
        __shared_backpack = new MCBackpack;
        __shared_backpack->loadData();
    }
    
    return __shared_backpack;
}

void
MCBackpack::saveData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    
    saveEffectiveItems();
    saveEquipmentItems();
    
    JsonBox::Object backpack;
    
    /* 储存金钱 */
    backpack[kMCMoneyKey] = JsonBox::Value(money_);
    /* 储存当前使用的武器 */
    char o_id_buffer[5] = {0};
    char *c_str_o_id = o_id_buffer;
    mc_object_id_t o_id = currentWeapon_->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    backpack[kMCCurrentWeaponKey] = JsonBox::Value(c_str_o_id);
    
    JsonBox::Value backpackValue(backpack);
    ostringstream outputStream;
    backpackValue.writeToStream(outputStream);
    string data = outputStream.str();
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
    userDefault->setStringForKey(kMCBackpackKey, output);
    delete []output;
}

void
MCBackpack::loadData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();

    loadEffectiveItems();
    loadEquipmentItems();
    
    string data = userDefault->getStringForKey(kMCBackpackKey, "");
    if (data.size() > 0) {
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
        JsonBox::Value v;
        v.loadFromString(output);
        
        JsonBox::Object backpack;
        /* 加载金钱 */
        money_ = backpack[kMCMoneyKey].getInt();
        /* 加载当前使用的武器 */
        const char *c_str_o_id = backpack[kMCCurrentWeaponKey].getString().c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCDagger])) {
            currentWeapon_ = (MCEquipmentItem *) dagger_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCSword])) {
            currentWeapon_ = (MCEquipmentItem *) sword_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCGreatsword])) {
            currentWeapon_ = (MCEquipmentItem *) greatsword_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCWarhammer])) {
            currentWeapon_ = (MCEquipmentItem *) warhammer_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCHeavyDutyHammer])) {
            currentWeapon_ = (MCEquipmentItem *) heavyDutyHammer_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCHandAxe])) {
            currentWeapon_ = (MCEquipmentItem *) handAxe_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCWarAxe])) {
            currentWeapon_ = (MCEquipmentItem *) warAxe_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCGreataxe])) {
            currentWeapon_ = (MCEquipmentItem *) greataxe_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCSpear])) {
            currentWeapon_ = (MCEquipmentItem *) spear_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCLance])) {
            currentWeapon_ = (MCEquipmentItem *) lance_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCGiantSickle])) {
            currentWeapon_ = (MCEquipmentItem *) giantSickle_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCShortbow])) {
            currentWeapon_ = (MCEquipmentItem *) shortbow_->item;
        } else if (MCObjectIdIsEqualsTo(o_id, itemsOID[kMCLongbow])) {
            currentWeapon_ = (MCEquipmentItem *) longbow_->item;
        }
    }
}


void
MCBackpack::saveEffectiveItems()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    JsonBox::Array effectiveItems;
    
    /* 道具 */
    effectiveItems.push_back(JsonBox::Value((int) healthPotion_->count));
    effectiveItems.push_back(JsonBox::Value((int) physicalPotion_->count));
    effectiveItems.push_back(JsonBox::Value((int) fireballTrapWide_->count));
    effectiveItems.push_back(JsonBox::Value((int) fireballTrapDamage_->count));
    effectiveItems.push_back(JsonBox::Value((int) curseTrapWide_->count));
    effectiveItems.push_back(JsonBox::Value((int) curseTrapDamage_->count));
    effectiveItems.push_back(JsonBox::Value((int) paralysisTrapWide_->count));
    effectiveItems.push_back(JsonBox::Value((int) paralysisTrapDamage_->count));
    effectiveItems.push_back(JsonBox::Value((int) fogTrapWide_->count));
    effectiveItems.push_back(JsonBox::Value((int) fogTrapDamage_->count));
    effectiveItems.push_back(JsonBox::Value((int) flashTrapWide_->count));
    effectiveItems.push_back(JsonBox::Value((int) flashTrapDamage_->count));
    JsonBox::Value effectiveItemsValue(effectiveItems);
    ostringstream outputStream;
    effectiveItemsValue.writeToStream(outputStream);
    string data = outputStream.str();
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
    userDefault->setStringForKey(kMCEffectiveItemsKey, output);
    delete []output;
}

void
MCBackpack::saveEquipmentItems()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    JsonBox::Object equipmentItems;
    
    /* 装备 */
    char o_id_buffer[5] = {0};
    char ore_id_buffer[5] = {0};
    char *c_str_o_id = o_id_buffer;
    char *c_str_ore_id = ore_id_buffer;
    /* dagger_ */
    mc_object_id_t o_id = dagger_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    mc_object_id_t ore_id = ((MCEquipmentItem *) dagger_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* sword_ */
    o_id = sword_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) sword_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* greatsword_ */
    o_id = greatsword_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) greatsword_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* warhammer_ */
    o_id = warhammer_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) warhammer_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* heavyDutyHammer_ */
    o_id = heavyDutyHammer_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) heavyDutyHammer_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* handAxe_ */
    o_id = handAxe_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) handAxe_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* warAxe_ */
    o_id = warAxe_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) warAxe_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* greataxe_ */
    o_id = greataxe_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) greataxe_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* spear_ */
    o_id = spear_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) spear_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* lance_ */
    o_id = lance_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) lance_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* giantSickle_ */
    o_id = giantSickle_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) giantSickle_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* shortbow_ */
    o_id = shortbow_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) shortbow_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* longbow_ */
    o_id = longbow_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) longbow_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* helmet_ */
    o_id = helmet_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) helmet_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* armor_ */
    o_id = armor_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) armor_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    /* shinGuard_ */
    o_id =shinGuard_->item->getID();
    o_id_buffer[0] = o_id.class_;
    o_id_buffer[1] = o_id.sub_class_;
    o_id_buffer[2] = o_id.index_;
    o_id_buffer[3] = o_id.sub_index_;
    ore_id = ((MCEquipmentItem *) shinGuard_->item)->getOre()->getID();
    ore_id_buffer[0] = ore_id.class_;
    ore_id_buffer[1] = ore_id.sub_class_;
    ore_id_buffer[2] = ore_id.index_;
    ore_id_buffer[3] = ore_id.sub_index_;
    equipmentItems[c_str_o_id] = JsonBox::Value(c_str_ore_id);
    
    JsonBox::Value effectiveItemsValue(equipmentItems);
    ostringstream outputStream;
    effectiveItemsValue.writeToStream(outputStream);
    string data = outputStream.str();
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
    userDefault->setStringForKey(kMCEffectiveItemsKey, output);
    delete []output;
}

void
MCBackpack::loadEffectiveItems()
{
    MCItemManager *itemManager = MCItemManager::sharedItemManager();
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    string data = userDefault->getStringForKey(kMCEffectiveItemsKey, "");
    
    if (data.size() > 0) {
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
        JsonBox::Value v;
        v.loadFromString(output);
        
        JsonBox::Array effectiveItems = v.getArray();
        
        healthPotion_->count = effectiveItems.at(0).getInt();
        physicalPotion_->count = effectiveItems.at(1).getInt();
        fireballTrapWide_->count = effectiveItems.at(2).getInt();
        fireballTrapDamage_->count = effectiveItems.at(3).getInt();
        curseTrapWide_->count = effectiveItems.at(4).getInt();
        curseTrapDamage_->count = effectiveItems.at(5).getInt();
        paralysisTrapWide_->count = effectiveItems.at(6).getInt();
        paralysisTrapDamage_->count = effectiveItems.at(7).getInt();
        fogTrapWide_->count = effectiveItems.at(8).getInt();
        fogTrapDamage_->count = effectiveItems.at(9).getInt();
        flashTrapWide_->count = effectiveItems.at(10).getInt();
        flashTrapDamage_->count = effectiveItems.at(11).getInt();
        delete []output;
    }
    
    /* load items */
    healthPotion_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCHealthPotion]);
    physicalPotion_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCPhysicalPotion]);
    fireballTrapWide_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCFireballWide]);
    fireballTrapDamage_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCFireballDamage]);
    curseTrapWide_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCCurseWide]);
    curseTrapDamage_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCCurseDamage]);
    paralysisTrapWide_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCParalysisWide]);
    paralysisTrapDamage_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCParalysisDamage]);
    fogTrapWide_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCFogWide]);
    fogTrapDamage_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCFogDamage]);
    flashTrapWide_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCFlashWide]);
    flashTrapDamage_->item = itemManager->effectiveItemForObjectId(itemsOID[kMCFlashDamage]);
}

void
MCBackpack::loadEquipmentItems()
{
    MCItemManager *itemManager = MCItemManager::sharedItemManager();
    MCOreManager *oreManager = MCOreManager::sharedOreManager();
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    string data = userDefault->getStringForKey(kMCEffectiveItemsKey, "");

    dagger_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCDagger]);
    sword_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCSword]);
    greatsword_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCGreatsword]);
    warhammer_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCWarhammer]);
    heavyDutyHammer_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCHeavyDutyHammer]);
    handAxe_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCHandAxe]);
    warAxe_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCWarAxe]);
    greataxe_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCGreataxe]);
    spear_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCSpear]);
    lance_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCLance]);
    giantSickle_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCGiantSickle]);
    shortbow_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCShortbow]);
    longbow_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCLongbow]);
    helmet_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCHelmet]);
    armor_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCArmor]);
    shinGuard_->item = itemManager->equipmentItemForObjectId(itemsOID[kMCShinGuard]);

    dagger_->count = 1;
    sword_->count = 1;
    greatsword_->count = 1;
    warhammer_->count = 1;
    heavyDutyHammer_->count = 1;
    handAxe_->count = 1;
    warAxe_->count = 1;
    greataxe_->count = 1;
    spear_->count = 1;
    lance_->count = 1;
    giantSickle_->count = 1;
    shortbow_->count = 1;
    longbow_->count = 1;
    helmet_->count = 1;
    armor_->count = 1;
    shinGuard_->count = 1;
    
    if (data.size() > 0) {
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
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
        delete []output;
    }
}
