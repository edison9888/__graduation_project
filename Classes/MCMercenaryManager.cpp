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
#include "MCDice.h"
#include "MCHero.h"
#include "MCGameState.h"

static const char *kMCMercenariesKey = "bWVyY2VuYXJpZXM"; /* mercenaries的BASE64编码没有最后的= */
static const char *kMCMercenariesFilepath = "M000.jpkg";
static const char *kMCSpriteSheetBaseDirectory = "spritesheets";

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
        __shared_mercenary_manager->loadMercenaries();
        __shared_mercenary_manager->loadData();
    }
    
    return __shared_mercenary_manager;
}

void
MCMercenaryManager::loadMercenaries()
{
    JsonBox::Value v;
    JsonBox::Object mercenaries;
    JsonBox::Object::iterator mercenariesIterator;
    MCDiceMaker *diceMaker = MCDiceMaker::sharedDiceMaker();
    CCString *ccstring;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCMercenariesFilepath);
    if (pstrFileContent) {
        v.loadFromString(pstrFileContent->getCString());
    }
#else
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCMercenariesFilepath).c_str());
#endif

    mercenaries = v.getObject();

    for (mercenariesIterator = mercenaries.begin();
         mercenariesIterator != mercenaries.end();
         ++mercenariesIterator) {
        const char *c_str_m_id = mercenariesIterator->first.c_str();
        mc_object_id_t m_id = {
            c_str_m_id[0],
            c_str_m_id[1],
            c_str_m_id[2],
            c_str_m_id[3]
        };
        JsonBox::Object mercenaryObject = mercenariesIterator->second.getObject();
        MCMercenary *mercenary = new MCMercenary;
        mercenary->init();
        mercenary->autorelease();
        mercenary->setID(m_id);
        /* name String */
        ccstring = CCString::create(mercenaryObject["name"].getString().c_str());
        mercenary->setName(ccstring);
        ccstring->retain();
        /* face String */
        ccstring = CCString::create(mercenaryObject["face"].getString().c_str());
        mercenary->setFace(ccstring);
        ccstring->retain();
        /* build sprite sheet file path */
        ccstring = CCString::createWithFormat("%s/%c-%s",
                                              kMCSpriteSheetBaseDirectory,
                                              c_str_m_id[0] | 32,
                                              c_str_m_id + 1);
        mercenary->setSpriteSheet(ccstring);
        ccstring->retain();
        /* AI String */
        const char *c_str_ai_id = mercenariesIterator->first.c_str();
        mc_object_id_t ai_id = {
            c_str_ai_id[0],
            c_str_ai_id[1],
            c_str_ai_id[2],
            c_str_ai_id[3]
        };
            //todo: set ai
        /* cost Integer */
        mercenary->cost_ = mercenaryObject["cost"].getInt();
        /* HP Integer */
        mercenary->setHP(mercenaryObject["HP"].getInt());
        mercenary->setMaxHP(mercenary->getHP());
        /* dying Integer */
        mercenary->setDying(mercenaryObject["dying"].getInt());
        /* PP Integer */
        mercenary->setPP(mercenaryObject["PP"].getInt());
        mercenary->setMaxPP(mercenary->getPP());
        /* exhaustion Integer */
        mercenary->setExhaustion(mercenaryObject["exhaustion"].getInt());
        /* tired Integer */
        mercenary->setTired(mercenaryObject["tired"].getInt());
        /* dexterity Integer */
        mercenary->setDexterity(mercenaryObject["dexterity"].getInt());
        /* AC Integer */
        mercenary->setAC(mercenaryObject["AC"].getInt());
        /* armor-check-penalty Integer */
        mercenary->setArmorCheckPenalty(mercenaryObject["armor-check-penalty"].getInt());
        /* damage Object */
        /* damage.count Integer */
        /* damage.size Integer */
        JsonBox::Object damage = mercenaryObject["damage"].getObject();
        mercenary->setDamage(diceMaker->diceWithType(MCMakeDiceType(damage["count"].getInt(),
                                                                    damage["size"].getInt())));
        /* damage-bonus Integer */
        mercenary->setDamageBonus(mercenaryObject["damage-bonus"].getInt());
        /* critical-hit-visible Object */
        /* critical-hit-visible.min Integer */
        /* critical-hit-visible.max Integer */
        /* critical-hit-visible.dice Object */
        /* critical-hit-visible.dice.count Integer */
        /* critical-hit-visible.dice.size Integer */
        JsonBox::Object diceRange = mercenaryObject["critical-hit-visible"].getObject();
        JsonBox::Object diceRangeDice = diceRange["dice"].getObject();
        mercenary->criticalHitVisible_.min = diceRange["min"].getInt();
        mercenary->criticalHitVisible_.max = diceRange["max"].getInt();
        mercenary->criticalHitVisible_.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                             diceRangeDice["size"].getInt());
        /* critical-hit-invisible Object */
        /* critical-hit-invisible.min Integer */
        /* critical-hit-invisible.max Integer */
        /* critical-hit-invisible.dice Object */
        /* critical-hit-invisible.dice.count Integer */
        /* critical-hit-invisible.dice.size Integer */
        diceRange = mercenaryObject["critical-hit-invisible"].getObject();
        diceRangeDice = diceRange["dice"].getObject();
        mercenary->criticalHitInvisible_.min = diceRange["min"].getInt();
        mercenary->criticalHitInvisible_.max = diceRange["max"].getInt();
        mercenary->criticalHitInvisible_.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                               diceRangeDice["size"].getInt());
        /* critical-hit Double */
        mercenary->setCriticalHit(mercenaryObject["critical-hit"].getDouble());
        /* distance Integer */
        mercenary->setDistance(mercenaryObject["distance"].getInt());
#warning 木有配置技能
        /* effect Integer */
        mercenary->setEffect(mercenaryObject["effect"].getInt());
        /* effect-check Object */
        /* effect-check.min Integer */
        /* effect-check.max Integer */
        /* effect-check.dice Object */
        /* effect-check.dice.count Integer */
        /* effect-check.dice.size Integer */
        diceRange = mercenaryObject["effect-check"].getObject();
        diceRangeDice = diceRange["dice"].getObject();
        mercenary->effectCheck_.min = diceRange["min"].getInt();
        mercenary->effectCheck_.max = diceRange["max"].getInt();
        mercenary->effectCheck_.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                      diceRangeDice["size"].getInt());
        /* description String */
        ccstring = CCString::create(mercenaryObject["description"].getString().c_str());
        mercenary->setDescription(ccstring);
        ccstring->retain();
        
        mercenaries_->setObject(mercenary, MCObjectIdToDickKey(m_id));
    }
}

MCMercenary *
MCMercenaryManager::mercenaryForObjectId(mc_object_id_t anObjectId)
{
    MCMercenary *mercenary = (MCMercenary *) protoMercenaryForObjectId(anObjectId)->copy();

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
MCMercenaryManager::protoMercenaryForObjectId(mc_object_id_t anObjectId)
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
    MCMercenary *mercenary = mercenaryForObjectId(anObjectId);
    
    
    if (hired_->isFull()) {
        return kMCFullTeam;
    }
    
    if (backpack->spend(mercenary->cost_)) {
        hired_->addRole(mercenary);
    } else {
        return kMCNotEnoughMoney;
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
    hired->removeObject(MCHero::sharedHero());
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
    
    if (MCGameState::sharedGameState()->isSaveFileExists() && data.size() > 0) {
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
