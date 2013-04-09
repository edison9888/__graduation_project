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
#include "MCDice.h"
#include "MCScript.h"

static const char *kMCNPCResourceFilePath = "N000.jpkg";
static const char *kMCEnemyResourceFilePath = "E400.jpkg";
static const char *kMCSpriteSheetBaseDirectory = "spritesheets";

/* shadow */
const char *kMCShadow = "kMCShadow";
static const char *kMCShadowFilepath = "UI/shadow.png";
const char *kMCSelectedShadow = "kMCSelectedShadow";
static const char *kMCSelectedShadowFilepath = "UI/selected_shadow.png";

static MCRoleManager *__shared_role_manager = NULL;


//static CCSpriteFrame *
static CCSprite *
createMaskedSprite(CCSprite *mask, CCSprite *source)
{
    CCSize maskContentSize = mask->getContentSize();
    CCSize sourceContentSize = source->getContentSize();
    CCSize contentSizeInPixels = mask->getTexture()->getContentSizeInPixels();
    CCRenderTexture *rt = CCRenderTexture::create(contentSizeInPixels.width,
                                                  contentSizeInPixels.height);
    
    mask->setPosition(ccp(maskContentSize.width / 2,
                          maskContentSize.height / 2));
    source->setPosition(ccp(sourceContentSize.width / 2,
                            sourceContentSize.height / 2));
    
    ccBlendFunc blendFunc1 = { GL_ONE, GL_ZERO };
    ccBlendFunc blendFunc2 = { GL_DST_ALPHA, GL_ZERO };
    
    mask->setBlendFunc(blendFunc1);
    source->setBlendFunc(blendFunc2);
    
    rt->begin();
    mask->visit();
    source->visit();
    rt->end();
    
    CCSprite *retval = CCSprite::createWithTexture(rt->getSprite()->getTexture());
    retval->setFlipY(true);
    return retval;
}

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
    loadEnemyData();
    
    /* tags: #frame #cache */
    /* 加载进缓存图片 */
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
    CCRect kMCShadowFrame = CCRectMake(0,
                                       0,
                                       32 / contentScaleFactor,
                                       12 / contentScaleFactor);
    
    CCSpriteFrame *frame = CCSpriteFrame::create(kMCShadowFilepath, kMCShadowFrame);
    cache->addSpriteFrame(frame, kMCShadow);
    
    frame = CCSpriteFrame::create(kMCSelectedShadowFilepath, kMCShadowFrame);
    cache->addSpriteFrame(frame, kMCSelectedShadow);
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
    JsonBox::Object npcs;
    JsonBox::Object::iterator npcsIterator;
    JsonBox::Object npcObject;
    MCNPC *role;
    const char *c_str_object_id;
    mc_object_id_t object_id;
    CCString *ccstring;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCNPCResourceFilePath);
    if (pstrFileContent) {
        v.loadFromString(pstrFileContent->getCString());
    }
#else
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(kMCNPCResourceFilePath));
#endif

    npcs = v.getObject();
    for (npcsIterator = npcs.begin(); npcsIterator != npcs.end(); ++npcsIterator) {
        c_str_object_id = npcsIterator->first.c_str();
        object_id.class_ = c_str_object_id[0];
        object_id.sub_class_ = c_str_object_id[1];
        object_id.index_ = c_str_object_id[2];
        object_id.sub_index_ = c_str_object_id[3];
        npcObject = npcsIterator->second.getObject();
        role = new MCNPC;
        CCAssert(role != NULL, "内存不足！");
        ccstring = CCString::create(npcObject["face"].getString().c_str());
        role->setFace(ccstring);
        ccstring->retain();
        ccstring = CCString::create(npcObject["name"].getString().c_str());
        role->setName(ccstring);
        ccstring->retain();
        ccstring = CCString::create(npcObject["sprite-sheet"].getString().c_str());
        role->setSpriteSheet(ccstring);
        ccstring->retain();
        if (npcObject["trigger"].isString()) { /* 暂时只有NPC有脚本 */
            MCScript *trigger = MCScriptMaker::createScript(npcObject["trigger"].getString().c_str());
            role->setTrigger(trigger);
            trigger->retain();
        }
        ccstring = CCString::create(npcObject["description"].getString().c_str());
        role->setDescription(ccstring);
        ccstring->retain();
        ccstring = CCString::create(npcObject["default-dialogue"].getString().c_str());
        role->setDefaultDialogue(ccstring);
        ccstring->retain();
        role->setID(object_id);
        role->init();
        role->autorelease();
        npcs_->setObject(role, MCObjectIdToDickKey(object_id));
    }
}

/*
 "E310": {
 "PP": 80,
 "AC": 14,
 "critical-hit": 3,
 "skills": {
 "A": null,
 "D": null,
 "B": null,
 "C": null
 },
 "critical-hit-visible": {
 "min": 19,
 "max": 20,
 "dice": {
 "count": 1,
 "size": 20
 }
 },
 "effect-check": null,
 "armor-check-penalty": 0,
 "AI": "AI03",
 "critical-hit-invisible": {
 "min": 6,
 "max": 12,
 "dice": {
 "count": 1,
 "size": 20
 }
 },
 "sprite-sheet": "spritesheets/e-310",
 "damage": {
 "count": 1,
 "size": 8
 },
 "damage-bonus": 9,
 "distance": 1,
 "effect": null,
 "name": "粽发侠客",
 "HP": 80
 }
*/
void
MCRoleManager::loadEnemyData()
{
    JsonBox::Value v;
    JsonBox::Object enemies;
    JsonBox::Object::iterator enemiesIterator;
    MCDiceMaker *diceMaker = MCDiceMaker::sharedDiceMaker();
    CCString *ccstring;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCEnemyResourceFilePath);
    if (pstrFileContent) {
        v.loadFromString(pstrFileContent->getCString());
    }
#else
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(kMCEnemyResourceFilePath));
#endif
    
    enemies = v.getObject();
    
    for (enemiesIterator = enemies.begin();
         enemiesIterator != enemies.end();
         ++enemiesIterator) {
        const char *c_str_m_id = enemiesIterator->first.c_str();
        mc_object_id_t m_id = {
            c_str_m_id[0],
            c_str_m_id[1],
            c_str_m_id[2],
            c_str_m_id[3]
        };
        JsonBox::Object mercenaryObject = enemiesIterator->second.getObject();
        MCEnemy *enemy = new MCEnemy;
        /* AI String */
        const char *c_str_ai_id = enemiesIterator->first.c_str();
        mc_object_id_t ai_id = {
            c_str_ai_id[0],
            c_str_ai_id[1],
            c_str_ai_id[2],
            c_str_ai_id[3]
        };
            //todo: set ai
        enemy->init(ai_id.index_ == '0' ? MCTerrestrial : MCFlying);
        enemy->autorelease();
        enemy->setID(m_id);
        /* name String */
        ccstring = CCString::create(mercenaryObject["name"].getString().c_str());
        enemy->setName(ccstring);
        ccstring->retain();
        /* face String */
        ccstring = CCString::create(mercenaryObject["face"].getString().c_str());
        enemy->setFace(ccstring);
        ccstring->retain();
        /* build sprite sheet file path */
        ccstring = CCString::createWithFormat("%s/%c-%s",
                                              kMCSpriteSheetBaseDirectory,
                                              c_str_m_id[0] | 32,
                                              c_str_m_id + 1);
        enemy->setSpriteSheet(ccstring);
        ccstring->retain();
        /* HP Integer */
        enemy->setHP(mercenaryObject["HP"].getInt());
        enemy->setMaxHP(enemy->getHP());
        /* PP Integer */
        enemy->setPP(mercenaryObject["PP"].getInt());
        enemy->setMaxPP(enemy->getPP());
        /* AC Integer */
        enemy->setAC(mercenaryObject["AC"].getInt());
        /* armor-check-penalty Integer */
        enemy->setArmorCheckPenalty(mercenaryObject["armor-check-penalty"].getInt());
        /* damage Object */
        /* damage.count Integer */
        /* damage.size Integer */
        JsonBox::Object damage = mercenaryObject["damage"].getObject();
        enemy->setDamage(diceMaker->diceWithType(MCMakeDiceType(damage["count"].getInt(),
                                                                    damage["size"].getInt())));
        /* damage-bonus Integer */
        enemy->setDamageBonus(mercenaryObject["damage-bonus"].getInt());
        /* critical-hit-visible Object */
        /* critical-hit-visible.min Integer */
        /* critical-hit-visible.max Integer */
        /* critical-hit-visible.dice Object */
        /* critical-hit-visible.dice.count Integer */
        /* critical-hit-visible.dice.size Integer */
        JsonBox::Object diceRange = mercenaryObject["critical-hit-visible"].getObject();
        JsonBox::Object diceRangeDice = diceRange["dice"].getObject();
        enemy->criticalHitVisible_.min = diceRange["min"].getInt();
        enemy->criticalHitVisible_.max = diceRange["max"].getInt();
        enemy->criticalHitVisible_.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                             diceRangeDice["size"].getInt());
        /* critical-hit-invisible Object */
        /* critical-hit-invisible.min Integer */
        /* critical-hit-invisible.max Integer */
        /* critical-hit-invisible.dice Object */
        /* critical-hit-invisible.dice.count Integer */
        /* critical-hit-invisible.dice.size Integer */
        diceRange = mercenaryObject["critical-hit-invisible"].getObject();
        diceRangeDice = diceRange["dice"].getObject();
        enemy->criticalHitInvisible_.min = diceRange["min"].getInt();
        enemy->criticalHitInvisible_.max = diceRange["max"].getInt();
        enemy->criticalHitInvisible_.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                               diceRangeDice["size"].getInt());
        /* critical-hit Double */
        enemy->setCriticalHit(mercenaryObject["critical-hit"].getDouble());
        /* distance Integer */
        enemy->setDistance(mercenaryObject["distance"].getInt());
#warning 木有配置技能
        /* effect Integer */
        enemy->setEffect(mercenaryObject["effect"].getInt());
        /* effect-check Object */
        /* effect-check.min Integer */
        /* effect-check.max Integer */
        /* effect-check.dice Object */
        /* effect-check.dice.count Integer */
        /* effect-check.dice.size Integer */
        diceRange = mercenaryObject["effect-check"].getObject();
        diceRangeDice = diceRange["dice"].getObject();
        enemy->effectCheck_.min = diceRange["min"].getInt();
        enemy->effectCheck_.max = diceRange["max"].getInt();
        enemy->effectCheck_.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                      diceRangeDice["size"].getInt());
        /* description String */
        ccstring = CCString::create(mercenaryObject["description"].getString().c_str());
        enemy->setDescription(ccstring);
        ccstring->retain();
        
        enemies_->setObject(enemy, MCObjectIdToDickKey(m_id));
    }
}
