//
//  MCBattle.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCBattle.h"
#include "MCNormalBattleImpl.h"
#include "MCMutiplayerBattleImpl.h"

static MCBattle *__shared_battle = NULL;

bool
MCBattle::_init()
{
    return true;
}

MCBattle *
MCBattle::sharedBattle()
{
    if (__shared_battle == NULL) {
        __shared_battle = new MCBattle;
        if (__shared_battle && __shared_battle->_init()) {
            __shared_battle->autorelease();
        } else {
            delete __shared_battle;
            __shared_battle = NULL;
        }
    }
    
    return __shared_battle;
}

MCBattleProtoacol *
MCBattle::battleWithType(MCBattleType type)
{
    if (type == MCNormalBattle) {
        return MCNormalBattleImpl::sharedBattleImplInstance();
    } else if (type == MCMutiplayerBattle) {
        return MCMutiplayerBattleImpl::sharedBattleImplInstance();
    }
    
    return NULL;
}

