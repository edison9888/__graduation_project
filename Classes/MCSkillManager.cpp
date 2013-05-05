//
//  MCSkillManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include <sstream>
using namespace std;

#include "MCSkillManager.h"
#include "JsonBox.h"
#include "MCBase64.h"
#include "MCGameState.h"
#include "MCEffectManager.h"

const char *kMCSkillPackageFilepath = "SXXX.jpkg";

static const char *kMCSkillsKey = "c2tpbGxz"; /* skills的BASE64编码 */

static MCSkillManager *__shared_skill_manager = NULL;

MCSkillManager::MCSkillManager()
{
    swordSkills_ = CCArray::create();
    hammerSkills_ = CCArray::create();
    axeSkills_ = CCArray::create();
    spearSkills_ = CCArray::create();
    bowSkills_ = CCArray::create();
    otherSkills_ = CCArray::create();
    otherSkills_->retain();
    
    skills_ = CCDictionary::create();
    skills_->setObject(swordSkills_, MCSwordSkill);
    skills_->setObject(hammerSkills_, MCHammerSkill);
    skills_->setObject(axeSkills_, MCAxeSkill);
    skills_->setObject(spearSkills_, MCSpearSkill);
    skills_->setObject(bowSkills_, MCBowSkill);
    skills_->retain();
    
    swordProficiency_ = 0;
    hammerProficiency_ = 0;
    axeProficiency_ = 0;
    spearProficiency_ = 0;
    bowProficiency_ = 0;
}

MCSkillManager::~MCSkillManager()
{
    CC_SAFE_RELEASE(otherSkills_);
    CC_SAFE_RELEASE(skills_);
}

MCSkillManager *
MCSkillManager::sharedSkillManager()
{
    if (__shared_skill_manager == NULL) {
        __shared_skill_manager = new MCSkillManager;
        __shared_skill_manager->loadSkills();
    }
    
    return __shared_skill_manager;
}

MCSkill *
MCSkillManager::skillForObjectId(mc_object_id_t anObjectId)
{
    MCSkill *proto = protoSkillForObjectId(anObjectId);
    MCSkill *skill = NULL;
    
    if (proto) {
        skill = dynamic_cast<MCSkill *>(proto->copy());
        if (skill) {
            skill->autorelease();
        }
    }
    
    return skill;
}

MCSkill *
MCSkillManager::protoSkillForObjectId(mc_object_id_t anObjectId)
{
    MCSkillType skillType = anObjectId.sub_class_;
    CCArray *skills;
    CCObject *obj;
    MCSkill *skill;
    
    if (skillType == MCSwordSkill) {
        skills = swordSkills_;
    } else if (skillType == MCHammer) {
        skills = hammerSkills_;
    } else if (skillType == MCAxeSkill) {
        skills = axeSkills_;
    } else if (skillType == MCSpearSkill) {
        skills = spearSkills_;
    } else if (skillType == MCBowSkill) {
        skills = bowSkills_;
    } else {
        skills = otherSkills_;
    }
    
    CCARRAY_FOREACH(skills, obj) {
        skill = dynamic_cast<MCSkill *>(obj);
        mc_object_id_t s_id = skill->id_;
        if (MCObjectIdIsEqualsTo(s_id, anObjectId)) {
            return skill;
        }
    }
    
    return NULL;
}

CCArray *
MCSkillManager::skillsForSkillType(MCSkillType aSkillType)
{
    if (aSkillType == MCSwordSkill) {
        return swordSkills_;
    } else if (aSkillType == MCHammer) {
        return hammerSkills_;
    } else if (aSkillType == MCAxeSkill) {
        return axeSkills_;
    } else if (aSkillType == MCSpearSkill) {
        return spearSkills_;
    } else if (aSkillType == MCBowSkill) {
        return bowSkills_;
    } else {
        return otherSkills_;
    }

    return NULL;
}

void
MCSkillManager::loadData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    string data = userDefault->getStringForKey(kMCSkillsKey, "");
    
    if (data.size() > 0) {
        const char *input = data.c_str();
        char *output;
        mc_size_t len = strlen(input);
        MCBase64Decode((mc_byte_t *) input, len, (mc_byte_t **) &output);
        JsonBox::Value v;
        v.loadFromString(output);
        
        JsonBox::Array proficiency = v.getArray();
        
        swordProficiency_ = proficiency.at(0).getInt();
        hammerProficiency_ = proficiency.at(1).getInt();
        axeProficiency_ = proficiency.at(2).getInt();
        spearProficiency_ = proficiency.at(3).getInt();
        bowProficiency_ = proficiency.at(4).getInt();
        delete []output;
    } else {
        swordProficiency_ = 0;
        hammerProficiency_ = 0;
        axeProficiency_ = 0;
        spearProficiency_ = 0;
        bowProficiency_ = 0;
    }
}

void
MCSkillManager::saveData()
{
    CCUserDefault *userDefault = CCUserDefault::sharedUserDefault();
    JsonBox::Array proficiency;
    
    proficiency.push_back(JsonBox::Value((int) swordProficiency_));
    proficiency.push_back(JsonBox::Value((int) hammerProficiency_));
    proficiency.push_back(JsonBox::Value((int) axeProficiency_));
    proficiency.push_back(JsonBox::Value((int) spearProficiency_));
    proficiency.push_back(JsonBox::Value((int) bowProficiency_));
    JsonBox::Value proficiencyValue(proficiency);
    ostringstream outputStream;
    proficiencyValue.writeToStream(outputStream);
    string data = outputStream.str();
    const char *input = data.c_str();
    char  *output;
    mc_size_t len = strlen(input);
    MCBase64Encode((mc_byte_t *) input, len, (mc_byte_t **) &output);
    userDefault->setStringForKey(kMCSkillsKey, output);
    delete []output;
}

void
MCSkillManager::improveProficiency(MCSkillType aSkillType)
{
    if (aSkillType == MCSwordSkill) {
        if (swordProficiency_ < UINT16_MAX) {
            swordProficiency_ += 1;
        }
    } else if (aSkillType == MCHammer) {
        if (hammerProficiency_ < UINT16_MAX) {
            hammerProficiency_ += 1;
        }
    } else if (aSkillType == MCAxeSkill) {
        if (axeProficiency_ < UINT16_MAX) {
            axeProficiency_ += 1;
        }
    } else if (aSkillType == MCSpearSkill) {
        if (spearProficiency_ < UINT16_MAX) {
            spearProficiency_ += 1;
        }
    } else if (aSkillType == MCBowSkill) {
        if (bowProficiency_ < UINT16_MAX) {
            bowProficiency_ += 1;
        }
    }
}

/*
 字段                  |  字段说明       | 解释
 ---------------------|----------------|---------
 ID                   | 技能ID         | 技能ID为技能字典中技能的key
 name                 | 技能名称        | 技能的名称
 icon                 | 技能图标        | 技能的图标，敌方技能没有图标
 effect-id            | 效果ID         | 效果ID为效果字典中效果的key
 release-time         | 释放所需时间     | 从启动释放到释放完成所需要的时间(此时间完成后才开始技能判定)
 active-time          | 完成所需时间     | 从释放到消失所经过的时间(此时间完成后效果消失)
 cold-time            | 冷却时间        | 到下一次释放技能所需要的等待时间
 distance             | 攻击距离        | 释放中心点到消失位置的距离
 length               | 攻击范围长      | OBB的高
 breadth              | 攻击范围宽      | OBB的宽
 consume              | 体力消耗        | 释放所需要消耗的体力
 required-proficiency | 熟练度          | 使用该技能所需要的熟练度
 revise               | 效果(伤害修正)   | 对对方产生的伤害的倍数
 count                | 连续释放次数     | 技能释放次数
 hp                   | HP影响         | 佣兵被动技能所有，对整个队伍的人物的HP的加成
 pp                   | PP影响         | 佣兵被动技能所有，对整个队伍的人物的PP的加成
 damage-bonus         | 伤害值加成      | 佣兵被动技能所有，对整个队伍的人物的伤害值的加成
 armor-check-penalty  | 防具检定减值加成 | 佣兵被动技能所有，对整个队伍的人物的防具检定减值的加成
 special              | 特殊效果        | 佣兵被动技能所有，具体效果查看特殊效果字典
 effect               | 附带效果        | 状态攻击
 effect-check         | 附带效果判定     | 状态攻击的判断范围
 trigger-type         | 效果触发类型     | 值为0或者1，0表示被动技能，1表示主动技能
 */
void
MCSkillManager::loadSkills()
{
    JsonBox::Value in;
    JsonBox::Object o;
    JsonBox::Object skills;
    JsonBox::Object::iterator skillsIterator;
    CCString *ccstring;
    MCEffectManager *effectManager = MCEffectManager::sharedEffectManager();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCSkillPackageFilepath);
    if (pstrFileContent) {
        in.loadFromString(pstrFileContent->getCString());
    }
#else
    in.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCSkillPackageFilepath));
#endif
    
    skills = in.getObject();
    for (skillsIterator = skills.begin();
         skillsIterator != skills.end();
         ++skillsIterator) {
        const char *c_str_s_id = skillsIterator->first.c_str();
        mc_object_id_t s_id = {
            c_str_s_id[0],
            c_str_s_id[1],
            c_str_s_id[2],
            c_str_s_id[3]
        };
        
        JsonBox::Object skillObject = skillsIterator->second.getObject();
        MCSkill *skill = new MCSkill;
        
        skill->autorelease();
        skill->id_ = s_id;
        /* name */
        ccstring = CCString::create(skillObject["name"].getString());
        skill->name_ = ccstring;
        ccstring->retain();
        /* icon */
        ccstring = CCString::create(skillObject["icon"].getString());
        skill->icon_ = ccstring;
        ccstring->retain();
        /* effect-id */
        if (skillObject["effect-id"].isString()) {
            const char *c_str_e_id = skillObject["effect-id"].getString().c_str();
            mc_object_id_t e_id = {
                c_str_e_id[0],
                c_str_e_id[1],
                c_str_e_id[2],
                c_str_e_id[3]
            };
            skill->effect_ = effectManager->effectForObjectId(e_id);
            skill->effect_->retain();
        } else {
            skill->effect_ = NULL;
        }
        /* release-time */
        skill->releaseTime = skillObject["release-time"].isDouble()
                                ? (float) skillObject["release-time"].getDouble()
                                : (float) skillObject["release-time"].getInt();
        /* active-time */
        skill->activeTime = skillObject["active-time"].isDouble()
                                ? (float) skillObject["active-time"].getDouble()
                                : (float) skillObject["active-time"].getInt();
        /* cold-time */
        skill->coldTime = skillObject["cold-time"].isDouble()
                            ? (float) skillObject["cold-time"].getDouble()
                            : (float) skillObject["cold-time"].getInt();
        /* distance */
        skill->distance = skillObject["distance"].getInt();
        /* length */
        skill->length = skillObject["length"].getInt();
        /* breadth */
        skill->breadth = skillObject["breadth"].getInt();
        /* consume */
        skill->consume = skillObject["consume"].isDouble()
                            ? (float) skillObject["consume"].getDouble()
                            : (float) skillObject["consume"].getInt();
        /* required-proficiency */
        skill->requiredProficiency = skillObject["required-proficiency"].getInt();
        /* revise */
        skill->revise = skillObject["revise"].isDouble()
                            ? (float) skillObject["revise"].getDouble()
                            : (float) skillObject["revise"].getInt();
        /* count */
        skill->count = skillObject["count"].getInt();
        /* hp */
        skill->hp = skillObject["hp"].getInt();
        /* pp */
        skill->pp = skillObject["pp"].isDouble()
                        ? (float) skillObject["pp"].getDouble()
                        : (float) skillObject["pp"].getInt();
        /* damage-bonus */
        skill->damageBonus = skillObject["damage-bonus"].getInt();
        /* armor-check-penalty */
        skill->armorCheckPenalty = skillObject["armor-check-penalty"].getInt();
        /* special */
        skill->special = skillObject["special"].getInt();
        /* effect */
        skill->effect = skillObject["effect"].getInt();
        if (skillObject["effect"].isInteger()) {
            skill->effect = skillObject["effect"].getInt();
            /* effect-check */
            JsonBox::Object diceRange = skillObject["effect-check"].getObject();
            JsonBox::Object diceRangeDice = diceRange["dice"].getObject();
            skill->effectCheck.min = diceRange["min"].getInt();
            skill->effectCheck.max = diceRange["max"].getInt();
            skill->effectCheck.dice = MCMakeDiceType(diceRangeDice["count"].getInt(),
                                                         diceRangeDice["size"].getInt());
        } else {
            skill->effect = MCNormalState;
        }
        /* trigger-type */
        skill->triggerType = skillObject["trigger-type"].getInt();
        /* continuable */
        skill->isContinuable = skillObject["continuable"].getBoolean();
        
        MCSkillType skillType = s_id.sub_class_;
        if (skillType == MCSwordSkill) {
            swordSkills_->addObject(skill);
        } else if (skillType == MCHammer) {
            hammerSkills_->addObject(skill);
        } else if (skillType == MCAxeSkill) {
            axeSkills_->addObject(skill);
        } else if (skillType == MCSpearSkill) {
            spearSkills_->addObject(skill);
        } else if (skillType == MCBowSkill) {
            bowSkills_->addObject(skill);
        } else {
            otherSkills_->addObject(skill);
        }
    }
}
