//
//  MCHero.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCHero.h"

static MCHero *__shared_hero = NULL;

bool
MCHero::init()
{
    setRoleType(MCHeroType);
    
    return true;
}

CCSprite *
MCHero::entity()
{
    CCSprite *entity = CCSprite::create();
    
    return entity;
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