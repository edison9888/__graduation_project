//
//  MCMutiplayerBattleImpl.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCMutiplayerBattleImpl.h"

static MCMutiplayerBattleImpl *__shread_battle_imple_instance = NULL;

bool
MCMutiplayerBattleImpl::_init()
{
    return true;
}

MCMutiplayerBattleImpl *
MCMutiplayerBattleImpl::sharedBattleImplInstance()
{
    if (__shread_battle_imple_instance == NULL) {
        __shread_battle_imple_instance = new MCMutiplayerBattleImpl;
        if (__shread_battle_imple_instance && __shread_battle_imple_instance->_init()) {
            __shread_battle_imple_instance->autorelease();
        } else {
            delete __shread_battle_imple_instance;
            __shread_battle_imple_instance = NULL;
        }
    }
    
    return __shread_battle_imple_instance;
}
