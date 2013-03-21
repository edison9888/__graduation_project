//
//  MCMercenaryManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include <sstream>
using namespace std;

#include "JsonBox.h"
#include "MCBase64.h"
#include "MCBackpack.h"
#include "MCMercenaryManager.h"

const char *kMCMercenariesKey = "bWVyY2VuYXJpZXM"; /* mercenaries的BASE64编码没有最后的= */

static MCMercenaryManager *__shared_mercenary_manager = NULL;

MCMercenaryManager::MCMercenaryManager()
{
    hired_ = MCTeam::sharedTeam();
    mercenaries_ = CCDictionary::create();
    mercenaries_->retain();
}

MCMercenaryManager::~MCMercenaryManager()
{
    CC_SAFE_RELEASE(mercenaries_);
}

MCMercenaryManager *
MCMercenaryManager::sharedMercenaryManager()
{
    if (__shared_mercenary_manager == NULL) {
        __shared_mercenary_manager = new MCMercenaryManager;
        __shared_mercenary_manager->loadData();
    }
    
    return __shared_mercenary_manager;
}

MCMercenary *
MCMercenaryManager::mercenaryForObjectId(mc_object_id_t anObjectId)
{
    MCMercenary *mercenary = (MCMercenary *) metaMercenaryForObjectId(anObjectId)->copy();
    
    if (mercenary && mercenary->MCRole::init()) {
        mercenary->autorelease();
        mercenary->loadSpriteSheet();
    } else {
        CC_SAFE_DELETE(mercenary);
        mercenary = NULL;
    }
    
    return mercenary;
}

MCMercenary *
MCMercenaryManager::metaMercenaryForObjectId(mc_object_id_t anObjectId)
{
    return (MCMercenary *) mercenaries_->objectForKey(MCObjectIdToDickKey(anObjectId));
}

/**
 * 已雇佣将返回kMCHired
 */
mc_ssize_t
MCMercenaryManager::hire(mc_object_id_t anObjectId)
{
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    MCMercenary *mercenary = (MCMercenary *) mercenaries_->objectForKey(MCObjectIdToDickKey(anObjectId));
    
    if (mercenary->cost_ > backpack->getMoney()) {
        return kMCNotEnoughMoney;
    }
    
    if (hired_->hasRole(mercenary)) {
        return kMCHired;
    }

    return kMCHandleSucceed;
}

/**
 * 解雇
 */
void
MCMercenaryManager::dismiss(mc_object_id_t anObjectId)
{
    MCMercenary *mercenary = (MCMercenary *) mercenaries_->objectForKey(MCObjectIdToDickKey(anObjectId));
    
    hired_->removeRole(mercenary);
}

void
MCMercenaryManager::saveData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    JsonBox::Array mercenaries;
    CCObject *obj;
    MCMercenary *mercenary;
    char o_id_buffer[5] = {0};
    char *c_str_o_id = o_id_buffer;
    
    CCArray *hired = hired_->getRoles();
    CCARRAY_FOREACH(hired, obj) {
        mercenary = (MCMercenary *) obj;
        mc_object_id_t o_id = mercenary->getID();
        o_id_buffer[0] = o_id.class_;
        o_id_buffer[1] = o_id.sub_class_;
        o_id_buffer[2] = o_id.index_;
        o_id_buffer[3] = o_id.sub_index_;
        mercenaries.push_back(JsonBox::Value(c_str_o_id));
    }
    JsonBox::Value mercenariesValue(mercenaries);
    ostringstream outputStream;
    mercenariesValue.writeToStream(outputStream);
    string data = outputStream.str();
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
    userDefault->setStringForKey(kMCMercenariesKey, output);
    delete []output;
}

void
MCMercenaryManager::loadData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    string data = userDefault->getStringForKey(kMCMercenariesKey, "");
    
    if (data.size() > 0) {
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
        JsonBox::Value v;
        v.loadFromString(output);
        
        JsonBox::Array mercenaries = v.getArray();
        JsonBox::Array::iterator mercenariesIterator;
        for (mercenariesIterator = mercenaries.begin();
             mercenariesIterator != mercenaries.end();
             ++mercenariesIterator) {
            const char *c_str_o_id = mercenariesIterator->getString().c_str();
            mc_object_id_t o_id = {
                c_str_o_id[0],
                c_str_o_id[1],
                c_str_o_id[2],
                c_str_o_id[3]
            };
            hired_->addRole(mercenaryForObjectId(o_id));
        }
        delete []output;
    }
}
