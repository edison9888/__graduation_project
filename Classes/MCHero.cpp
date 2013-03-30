//
//  MCHero.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCHero.h"

static MCHero *__shared_hero = NULL;
const char *kMCHeroFacePath = "faces/x-000.png";
const char *kMCHeroSpriteSheetPath = "spritesheets/x-000";


MCHero::~MCHero()
{
    CC_SAFE_DELETE(viewport_); /* 非CCObject子类 */
}

bool
MCHero::init()
{
    CCString *face = CCString::create(kMCHeroFacePath);
    setFace(face);
    face->retain();
    loadSpriteSheet(kMCHeroSpriteSheetPath);
    
    mc_object_id_t o_id = {
        'X', '0', '0', '0'
    };
    setID(o_id);
    setHP(100);
    setMaxHP(100);
    setPP(100);
    setMaxPP(100);
    
    viewport_ = MCViewport::create(getEntity());
    
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
