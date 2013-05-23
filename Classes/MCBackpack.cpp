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
#include "MCGameState.h"

MCDefineConstantString(kMCHealthPotionIcon);
MCDefineConstantString(kMCPhysicalPotionIcon);
MCDefineConstantString(kMCFireballWideIcon);
MCDefineConstantString(kMCFireballDamageIcon);
MCDefineConstantString(kMCCurseWideIcon);
MCDefineConstantString(kMCCurseDamageIcon);
MCDefineConstantString(kMCParalysisWideIcon);
MCDefineConstantString(kMCParalysisDamageIcon);
MCDefineConstantString(kMCFogWideIcon);
MCDefineConstantString(kMCFogDamageIcon);
MCDefineConstantString(kMCFlashWideIcon);
MCDefineConstantString(kMCFlashDamageIcon);

    //warning: 木有测试过

const mc_size_t kMCItemMax = 99;
#if MC_DEBUG_SAVEDATA == 1
static const char *kMCMoneyKey = "money";
static const char *kMCBackpackKey = "backpack";
static const char *kMCEffectiveItemsKey = "effective-items";
static const char *kMCZero = "0";
#else
static const char *kMCMoneyKey = "bW9uZXk"; /* money的BASE64编码没有最后的= */
static const char *kMCBackpackKey = "YmFja3BhY2s"; /* backpack的BASE64编码没有最后的= */
static const char *kMCEffectiveItemsKey = "ZWZmZWN0aXZlLWl0ZW1z"; /* effective-items的BASE64编码 */
static const char *kMCZero = "MA=="; /* 0的BASE64编码没有最后的== */
#endif

static MCBackpack *__shared_backpack = NULL;
static bool __icon_loaded = false;

/* 道具 */
mc_object_id_t effectiveItemsOID[] = {
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
    {'T', '0', '5', '2'}
};

mc_object_id_t
MCEffectiveItemObjectItem(MCEffectiveItemIndex anIndex)
{
    CCAssert(anIndex >= 0 && anIndex <= kMCFlashDamage, "out of index!");
    
    return effectiveItemsOID[anIndex];
}

MCBackpack::MCBackpack()
{
    money_ = 0;
    
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
}

MCBackpack *
MCBackpack::sharedBackpack()
{
    if (__shared_backpack == NULL) {
        __shared_backpack = new MCBackpack;
    }
    
    return __shared_backpack;
}

/**
 * 返回是否花费成功
 */
bool
MCBackpack::spend(mc_price_t money)
{
    if (money_ >= money) {
        money_ -= money;
        return true;
    }
    
    return false;
}

/**
 * 清除数据
 */
void
MCBackpack::erase()
{
    delete __shared_backpack;
    __shared_backpack = NULL;
}

void
MCBackpack::reload()
{
//    loadData();
}

void
MCBackpack::saveData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    
    saveEffectiveItems();
    
    JsonBox::Object backpack;
    
    /* 储存金钱 */
    backpack[kMCMoneyKey] = JsonBox::Value(money_);
    
    JsonBox::Value backpackValue(backpack);
    ostringstream outputStream;
    backpackValue.writeToStream(outputStream);
    string data = outputStream.str();
#if MC_DEBUG_SAVEDATA == 1
    const char *output = data.c_str();
#else
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
    userDefault->setStringForKey(kMCBackpackKey, output);
#if MC_DEBUG_SAVEDATA != 1
    delete []output;
#endif
}

void
MCBackpack::loadData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    
    loadEffectiveItems();
    
    if (! __icon_loaded) {
        loadIcons();
    }
    
    if (MCGameState::sharedGameState()->isSaveFileExists()) {
        string data = userDefault->getStringForKey(kMCBackpackKey, kMCZero);
        if (data.size() > 0) {
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
            
            JsonBox::Object backpack = v.getObject();
            /* 加载金钱 */
            money_ = backpack[kMCMoneyKey].getInt();
        }
    } else {
        money_ = 0;
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
#if MC_DEBUG_SAVEDATA == 1
    const char *output = data.c_str();
#else
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
    userDefault->setStringForKey(kMCEffectiveItemsKey, output);
#if MC_DEBUG_SAVEDATA != 1
    delete []output;
#endif
}

void
MCBackpack::loadEffectiveItems()
{
    MCItemManager *itemManager = MCItemManager::sharedItemManager();
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    string data = userDefault->getStringForKey(kMCEffectiveItemsKey, "");
    
    if (MCGameState::sharedGameState()->isSaveFileExists()
        && data.size() > 0) {
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
#if MC_DEBUG_SAVEDATA != 1
        delete []output;
#endif
    } else {
        healthPotion_->count = 0;
        physicalPotion_->count = 0;
        fireballTrapWide_->count = 0;
        fireballTrapDamage_->count = 0;
        curseTrapWide_->count = 0;
        curseTrapDamage_->count = 0;
        paralysisTrapWide_->count = 0;
        paralysisTrapDamage_->count = 0;
        fogTrapWide_->count = 0;
        fogTrapDamage_->count = 0;
        flashTrapWide_->count = 0;
        flashTrapDamage_->count = 0;
    }
    
    /* load items */
    healthPotion_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCHealthPotion]);
    healthPotion_->item->retain();
    physicalPotion_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCPhysicalPotion]);
    physicalPotion_->item->retain();
    fireballTrapWide_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCFireballWide]);
    fireballTrapWide_->item->retain();
    fireballTrapDamage_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCFireballDamage]);
    fireballTrapDamage_->item->retain();
    curseTrapWide_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCCurseWide]);
    curseTrapWide_->item->retain();
    curseTrapDamage_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCCurseDamage]);
    curseTrapDamage_->item->retain();
    paralysisTrapWide_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCParalysisWide]);
    paralysisTrapWide_->item->retain();
    paralysisTrapDamage_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCParalysisDamage]);
    paralysisTrapDamage_->item->retain();
    fogTrapWide_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCFogWide]);
    fogTrapWide_->item->retain();
    fogTrapDamage_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCFogDamage]);
    fogTrapDamage_->item->retain();
    flashTrapWide_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCFlashWide]);
    flashTrapWide_->item->retain();
    flashTrapDamage_->item = itemManager->effectiveItemForObjectId(effectiveItemsOID[kMCFlashDamage]);
    flashTrapDamage_->item->retain();
}

void
MCBackpack::loadIcons()
{
    /* 道具图片加进缓存 */
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *pobFrame;
    float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
    CCRect iconRect = CCRectMake(0, 0, 64 / contentScaleFactor, 64 / contentScaleFactor);
    
    pobFrame = CCSpriteFrame::create(healthPotion_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCHealthPotionIcon);
    pobFrame = CCSpriteFrame::create(physicalPotion_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCPhysicalPotionIcon);
    
    /* fireball */
    pobFrame = CCSpriteFrame::create(fireballTrapWide_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCFireballWideIcon);
    pobFrame = CCSpriteFrame::create(fireballTrapDamage_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCFireballDamageIcon);
    /* curse */
    pobFrame = CCSpriteFrame::create(curseTrapWide_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCCurseWideIcon);
    pobFrame = CCSpriteFrame::create(curseTrapDamage_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCCurseDamageIcon);
    /* paralysis */
    pobFrame = CCSpriteFrame::create(paralysisTrapWide_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCParalysisWideIcon);
    pobFrame = CCSpriteFrame::create(paralysisTrapDamage_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCParalysisDamageIcon);
    /* fog */
    pobFrame = CCSpriteFrame::create(fogTrapWide_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCFogWideIcon);
    pobFrame = CCSpriteFrame::create(fogTrapDamage_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCFogDamageIcon);
    /* flash */
    pobFrame = CCSpriteFrame::create(flashTrapWide_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCFlashWideIcon);
    pobFrame = CCSpriteFrame::create(flashTrapDamage_->item->getIcon()->getCString(),
                                     iconRect);
    cache->addSpriteFrame(pobFrame, kMCFlashDamageIcon);
    
    __icon_loaded = true;
}
