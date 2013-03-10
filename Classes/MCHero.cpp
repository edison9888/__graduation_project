//
//  MCHero.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCHero.h"

static MCHero *__shared_hero = NULL;
const char *kMCHeroSpriteSheetPath = "spritesheets/m-19"; //29、22、20、8、13、19

bool
MCHero::init()
{
    setRoleType(MCHeroType);
    setRoleRace(MCTerrans);
    loadSpriteSheet(kMCHeroSpriteSheetPath);
    
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
