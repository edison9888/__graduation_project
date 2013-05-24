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
#include "MCEffectManager.h"
#include "MCSkillManager.h"

static const char *kMCMercernaryNames[] = {
    "Aaron",
    "Abbott",
    "Abel",
    "Abner",
    "Abraham",
    "Adair",
    "Adam",
    "Addison",
    "Adolph",
    "Adonis",
    "Adrian",
    "Ahern",
    "Alan",
    "Albert",
    "Aldrich",
    "Alexander",
    "Alfred",
    "Alger",
    "Algernon",
    "Allen",
    "Alston",
    "Alva",
    "Alvin",
    "Alvis",
    "Amos",
    "Andre",
    "Andrew",
    "Andy",
    "Angelo",
    "Ansel",
    "Antoine",
    "Antonio",
    "Antony",
    "Archer",
    "Archibald",
    "Aries",
    "Arlen",
    "Armand",
    "Armstrong",
    "Arno",
    "Arnold",
    "Arthur",
    "Arvin",
    "Asa",
    "Ashburn",
    "Atwood",
    "Aubrey",
    "Augus",
    "August",
    "Augustine",
    "Avery",
    "Baldwin",
    "Bancroft",
    "Bard",
    "Barlow",
    "Barnett",
    "Baron",
    "Barrette",
    "Barry",
    "Bart",
    "Bartholomew",
    "Bartley",
    "Barton",
    "Basil",
    "Beau",
    "Beck",
    "Ben",
    "Benedict",
    "Benjamin",
    "Bennett",
    "Benson",
    "Berg",
    "Berger",
    "Bernard",
    "Bernie",
    "Bert",
    "Bertram",
    "Bevis",
    "Bill",
    "Bing",
    "Bishop",
    "Blair",
    "Blake",
    "Bleacher",
    "Blithe",
    "Bob",
    "Booth",
    "Borg",
    "Boris",
    "Bowen",
    "Boyce",
    "Boyd",
    "Bradley",
    "Brady",
    "Brandon",
    "Breton",
    "Brian",
    "Broderick",
    "Brook",
    "Bruce",
    "Bruno",
    "Buck",
    "Burgess",
    "Burke",
    "Burn",
    "Burton",
    "Byron",
    "Caesar",
    "Calvin",
    "Carey",
    "Carl",
    "Carr",
    "Carter",
    "Cash",
    "Cecil",
    "Cedric",
    "Chad",
    "Chapman",
    "Charles",
    "Chester",
    "Chinning",
    "Chisel",
    "Christ",
    "Christian",
    "Christopher",
    "Clare",
    "Clarence",
    "Clark",
    "Claude",
    "Clement",
    "Cleveland",
    "Cliff",
    "Clifford",
    "Clyde",
    "Colbert",
    "Colby",
    "Colin",
    "Conrad",
    "Corey",
    "Cornelius",
    "Cornell",
    "Craig",
    "Curitis",
    "Cyril",
    "Dana",
    "Daniel",
    "Darcy",
    "Darnell",
    "Darren",
    "Dave",
    "David",
    "Dean",
    "Dempsey",
    "Dennis",
    "Derrick",
    "Devin",
    "Dick",
    "Dominic",
    "Don",
    "Donahue",
    "Donald",
    "Douglas",
    "Drew",
    "Duke",
    "Duncan",
    "Dunn",
    "Dwight",
    "Dylan",
    "Earl",
    "Ed",
    "Eden",
    "Edgar",
    "Edison",
    "Edmund",
    "Edward",
    "Edwiin",
    "Egbert",
    "Eli",
    "Elijah",
    "Elliot",
    "Ellis",
    "Elmer",
    "Elroy",
    "Elton",
    "Elvis",
    "Emmanuel",
    "Enoch",
    "Eric",
    "Ernest",
    "Eugene",
    "Evan",
    "Everley",
    "Fabian",
    "Felix",
    "Ferdinand",
    "Fitch",
    "Fitzgerald",
    "Ford",
    "Francis",
    "Frank",
    "Franklin",
    "Frederic",
    "Gabriel",
    "Gale",
    "Gary",
    "Gavin",
    "Gene",
    "Geoff",
    "Geoffrey",
    "George",
    "Gerald",
    "Gilbert",
    "Giles",
    "Glenn",
    "Goddard",
    "Godfery",
    "Gordon",
    "Greg",
    "Gregary",
    "Griffith",
    "Grover",
    "Gustave",
    "Guy",
    "Haley",
    "Hamiltion",
    "Hardy",
    "Harlan",
    "Harley",
    "Harold",
    "Harriet",
    "Harry",
    "Harvey",
    "Hayden",
    "Heather",
    "Henry",
    "Herbert",
    "Herman",
    "Hilary",
    "Hiram",
    "Hobart",
    "Hogan",
    "Horace",
    "Howar",
    "Hubery",
    "Hugh",
    "Hugo",
    "Humphrey",
    "Hunter",
    "Hyman",
    "Ian",
    "Ingram",
    "Ira",
    "Isaac",
    "Isidore",
    "Ivan",
    "Ives",
    "Jack",
    "Jacob",
    "Jared",
    "Jason",
    "Jay",
    "Jeff",
    "Jeffrey",
    "Jeremy",
    "Jerome",
    "Jerry",
    "Jesse",
    "Jim",
    "Jo",
    "John",
    "Jonas",
    "Jonathan",
    "Joseph",
    "Joshua",
    "Joyce",
    "Julian",
    "Julius",
    "Justin",
    "Keith",
    "Kelly",
    "Ken",
    "Kennedy",
    "Kenneth",
    "Kent",
    "Kerr",
    "Kevin",
    "Kim",
    "King",
    "Kirk",
    "Kyle",
    "Lambert",
    "Lance",
    "Larry",
    "Lawrence",
    "Leif",
    "Len",
    "Lennon",
    "Leo",
    "Leonard",
    "Leopold",
    "Les",
    "Lester",
    "Levi",
    "Lewis",
    "Lionel",
    "Lou",
    "Louis",
    "Lucien",
    "Luther",
    "Lyle",
    "Lyndon",
    "Lynn",
    "Magee",
    "Malcolm",
    "Mandel",
    "Marcus",
    "Marico",
    "Mark",
    "Marlon",
    "Marsh",
    "Marshall",
    "Martin",
    "Marvin",
    "Matt",
    "Matthew",
    "Maurice",
    "Max",
    "Maximilian",
    "Maxwell",
    "Meredith",
    "Merle",
    "Michael",
    "Michell",
    "Mick",
    "Mike",
    "Miles",
    "Milo",
    "Monroe",
    "Montague",
    "Moore",
    "Morgan",
    "Mortimer",
    "Morton",
    "Moses",
    "Murphy",
    "Murray",
    "Myron",
    "Nat",
    "Nathan",
    "Nathaniel",
    "Neil",
    "Nelson",
    "Newman",
    "Nicholas",
    "Nick",
    "Nigel",
    "Noah",
    "Noel",
    "Norman",
    "Norton",
    "Ogden",
    "Oliver",
    "Omar",
    "Orville",
    "Osborn",
    "Oscar",
    "Osmond",
    "Oswald",
    "Otis",
    "Otto",
    "Owen",
    "Paddy",
    "Page",
    "Parker",
    "Patrick",
    "Paul",
    "Payne",
    "Perry",
    "Pete",
    "Peter",
    "Phil",
    "Philip",
    "Porter",
    "Prescott",
    "Primo",
    "Quennel",
    "Quentin",
    "Quincy",
    "Quinn",
    "Quintion",
    "Rachel",
    "Ralap",
    "Randolph",
    "Raymond",
    "Reg",
    "Regan",
    "Reginald",
    "Reuben",
    "Rex",
    "Richard",
    "Robert",
    "Robin",
    "Rock",
    "Rod",
    "Roderick",
    "Rodney",
    "Ron",
    "Ronald",
    "Rory",
    "Roy",
    "Rudolf",
    "Rupert",
    "Ryan",
    "Sam",
    "Sampson",
    "Samuel",
    "Sandy",
    "Saxon",
    "Scott",
    "Sean",
    "Sebastian",
    "Sid",
    "Sidney",
    "Silvester",
    "Simon",
    "Solomon",
    "Spencer",
    "Stan",
    "Stanford",
    "Stanley",
    "Stev",
    "Steven",
    "Steward",
    "Tab",
    "Taylor",
    "Ted",
    "Ternence",
    "Theobald",
    "Theodore",
    "Thomas",
    "Tiffany",
    "Tim",
    "Timothy",
    "Tobias",
    "Toby",
    "Todd",
    "Tom",
    "Tony",
    "Tracy",
    "Troy",
    "Truman",
    "Tyler",
    "Tyrone",
    "Ulysses",
    "Upton",
    "Uriah",
    "Valentine",
    "Valentine",
    "Verne",
    "Vic",
    "Victor",
    "Vincent",
    "Virgil",
    "Vito",
    "Vivian",
    "Wade",
    "Walker",
    "Walter",
    "Ward",
    "Warner",
    "Wayne",
    "Webb",
    "Webster",
    "Wendell",
    "Werner",
    "Wilbur",
    "Will",
    "William",
    "Willie",
    "Winfred",
    "Winston",
    "Woodrow",
    "Wordsworth",
    "Wright",
    "Wythe",
    "Xavier",
    "Yale",
    "Yehudi",
    "York",
    "Yves",
    "Zachary",
    "Zebulon",
    "Ziv"
};

const char *
MCMercenaryManagerChoiceMercenaryName()
{
    return kMCMercernaryNames[rand() % sizeof(kMCMercernaryNames) / sizeof(const char *)];
}

#if MC_DEBUG_SAVEDATA == 1
static const char *kMCMercenariesKey = "mercenaries";
#else
static const char *kMCMercenariesKey = "bWVyY2VuYXJpZXM"; /* mercenaries的BASE64编码没有最后的= */
#endif
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
    MCEffectManager *effectManager = MCEffectManager::sharedEffectManager();
    MCSkillManager *skillManager = MCSkillManager::sharedSkillManager();
    
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
        const char *c_str_o_id = mercenariesIterator->first.c_str();
        mc_object_id_t m_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        JsonBox::Object mercenaryObject = mercenariesIterator->second.getObject();
        MCMercenary *mercenary;
        
        if (m_id.sub_class_ == '9') {
            mercenary = new MCMercenary;
        } else if (m_id.sub_class_ == '8') {
            mercenary = new MCNervousMercenary;
        } else {
            CCAssert(false, "数据错误！");
            continue;
        }
        
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
                                              c_str_o_id[0] | 32,
                                              c_str_o_id + 1);
        mercenary->setSpriteSheet(ccstring);
        ccstring->retain();
        /* effect-id */
        c_str_o_id = mercenaryObject["effect-id"].getString().c_str();
        mc_object_id_t e_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        MCEffect *effect = effectManager->effectForObjectId(e_id);
        mercenary->setAttackEffect(effect);
        effect->retain();
        /* cost Integer */
        mercenary->cost_ = mercenaryObject["cost"].getInt();
        /* HP Integer */
        mercenary->setHP(mercenaryObject["HP"].getInt());
        mercenary->setMaxHP(mercenary->getHP());
        /* dying Integer 胆怯佣兵独有 */
        if (mercenary->mercenaryType_ == MCMercenary::MCNervousMercenary) {
            dynamic_cast<MCNervousMercenary *>(mercenary)->setDying(mercenaryObject["dying"].getInt());
        }
        /* PP Integer */
        mercenary->setPP(mercenaryObject["PP"].getInt());
        mercenary->setMaxPP(mercenary->getPP());
        /* consume Double */
        mercenary->setConsume(mercenaryObject["consume"].isDouble()
                              ? (float) mercenaryObject["consume"].getDouble()
                              : (float) mercenaryObject["consume"].getInt());
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
        float floatValue = mercenaryObject["critical-hit"].isDouble()
                            ? (float) mercenaryObject["critical-hit"].getDouble()
                            : (float) mercenaryObject["critical-hit"].getInt();
        mercenary->setCriticalHit(floatValue);
        /* distance Integer */
        mercenary->setDistance(mercenaryObject["distance"].getInt());
        
        /* skills Object */
        JsonBox::Object skillsObject = mercenaryObject["skills"].getObject();
        if (skillsObject["A"].isString()) {
            const char *c_str_s_id = skillsObject["A"].getString().c_str();
            mc_object_id_t s_id = {
                c_str_s_id[0],
                c_str_s_id[1],
                c_str_s_id[2],
                c_str_s_id[3]
            };
            mercenary->skills_->addObject(skillManager->skillForObjectId(s_id));
        }
        if (skillsObject["B"].isString()) {
            const char *c_str_s_id = skillsObject["B"].getString().c_str();
            mc_object_id_t s_id = {
                c_str_s_id[0],
                c_str_s_id[1],
                c_str_s_id[2],
                c_str_s_id[3]
            };
            mercenary->skills_->addObject(skillManager->skillForObjectId(s_id));
        }if (skillsObject["C"].isString()) {
            const char *c_str_s_id = skillsObject["C"].getString().c_str();
            mc_object_id_t s_id = {
                c_str_s_id[0],
                c_str_s_id[1],
                c_str_s_id[2],
                c_str_s_id[3]
            };
            mercenary->skills_->addObject(skillManager->skillForObjectId(s_id));
        }
        if (skillsObject["D"].isString()) {
            const char *c_str_s_id = skillsObject["D"].getString().c_str();
            mc_object_id_t s_id = {
                c_str_s_id[0],
                c_str_s_id[1],
                c_str_s_id[2],
                c_str_s_id[3]
            };
            mercenary->skills_->addObject(skillManager->skillForObjectId(s_id));
        }
        
        /* effect Integer */
        if (mercenaryObject["effect"].isInteger()) {
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
        } else {
            mercenary->setEffect(MCNormalState);
        }
        /* description String */
        ccstring = CCString::create(mercenaryObject["description"].getString().c_str());
        mercenary->setDescription(ccstring);
        ccstring->retain();
        /* action-effect String */
        ccstring = CCString::create(mercenaryObject["action-effect"].getString().c_str());
        mercenary->setActionEffect(ccstring);
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
MCMercenaryManager::dismiss(MCMercenary *aMercenary)
{
    aMercenary->died();
    hired_->removeRole(aMercenary);
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
        JsonBox::Object mercenaryObject;
        
        mercenaryObject["id"] = JsonBox::Value(c_str_o_id);
        mercenaryObject["name"] = JsonBox::Value(mercenary->getName()->getCString());
        mercenaries.push_back(mercenaryObject);
    }
    JsonBox::Value mercenariesValue(mercenaries);
    ostringstream outputStream;
    mercenariesValue.writeToStream(outputStream);
    string data = outputStream.str();
#if MC_DEBUG_SAVEDATA == 1
    const char *output = data.c_str();
#else
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
#endif
    userDefault->setStringForKey(kMCMercenariesKey, output);
#if MC_DEBUG_SAVEDATA != 1
    delete []output;
#endif
}

void
MCMercenaryManager::loadData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    string data = userDefault->getStringForKey(kMCMercenariesKey, "");
    
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
        
        JsonBox::Array mercenaries = v.getArray();
        JsonBox::Array::iterator mercenariesIterator;
        for (mercenariesIterator = mercenaries.begin();
             mercenariesIterator != mercenaries.end();
             ++mercenariesIterator) {
            JsonBox::Object mercenaryObject = mercenariesIterator->getObject();
            const char *c_str_o_id = mercenaryObject["id"].getString().c_str();
            mc_object_id_t o_id = {
                c_str_o_id[0],
                c_str_o_id[1],
                c_str_o_id[2],
                c_str_o_id[3]
            };
            MCMercenary *mercenary = mercenaryForObjectId(o_id);
            CCString *name = CCString::create(mercenaryObject["name"].getString());
            mercenary->setName(name);
            name->retain();
            hired_->addRole(mercenary);
        }
#if MC_DEBUG_SAVEDATA != 1
        delete []output;
#endif
    }
}
