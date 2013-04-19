//
//  MCHero.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCHero.h"
#include "MCAI.h"

static MCHero *__shared_hero = NULL;
const char *kMCHeroFacePath = "faces/x-000.png";
const char *kMCHeroSpriteSheetPath = "spritesheets/x-000";


MCHero::~MCHero()
{
}

bool
MCHero::init()
{
    CCString *face = CCString::create(kMCHeroFacePath);
    setFace(face);
    face->retain();
    loadSpriteSheet(kMCHeroSpriteSheetPath);
    
    roleType_ = MCRole::MCHero;
    roleRace_ = MCTerrans;
    mc_object_id_t o_id = {
        'X', '0', '0', '0'
    };
    setID(o_id);
    name_ = CCString::create("男猪脚");
    name_->retain();
    setHP(100);
    setMaxHP(100);
    setPP(100);
    setMaxPP(100);
    
    exhausted_ = false;
    exhaustion_ = 20;
    tired_ = 50;
    
    roleState_ = 0;
    spriteSheet_ = NULL;
    defaultDialogue_ = NULL;
    
    return true;
}

MCHero *
MCHero::sharedHero()
{
    if (__shared_hero == NULL) {
        __shared_hero = new MCHero;
        if (__shared_hero && __shared_hero->init()) {
        } else {
            CC_SAFE_DELETE(__shared_hero);
            __shared_hero = NULL;
        }
    }
    
    return __shared_hero;
}

MCRoleEntity *
MCHero::getEntity()
{
    MCRoleEntity *roleEntity = MCRole::getEntity();
    
    /* AI */
    if (ai_ == NULL) {
        ai_ = MCAI::create();
        ai_->retain();
        ai_->bind(this);
    }
    
    return roleEntity;
}


/**
 * 在主角面前的人物
 * 判断当前视野中的人物，然后返回最近的那个
 */
MCRole *
MCHero::roleOfFront()
{
    CCDictionary *roles = ai_->getRolesInVision();
    CCDictElement *elem;
    MCAIRole *role;
    MCRole *nearest = NULL;
    CCPoint roleCenter = getEntity()->getOBB().center;
    float minLength = 10000; /* 一个灰常大的值 */
    float length;
    
    CCDICT_FOREACH(roles, elem) {
        role = dynamic_cast<MCAIRole *>(elem->getObject());
        /* 对敌人表示无视 */
        if (role->role->getRoleType() == MCRole::MCEnemy) {
            continue;
        }
        length = ccpLength(ccpSub(role->role->getEntity()->getOBB().center, roleCenter));
        if (length < minLength) {
            nearest = role->role;
            minLength = length;
        }
    }
    
    return nearest;
}

/**
 * 死亡状态下回调
 */
void
MCHero::died()
{
    MCRoleEntity *roleEntity = getEntity();
    roleEntity->removeFromParentAndCleanup(false);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCRoleDiedNotification);
}
