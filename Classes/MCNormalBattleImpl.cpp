//
//  MCNormalBattleImpl.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCNormalBattleImpl.h"

static MCNormalBattleImpl *__shread_battle_imple_instance = NULL;

bool
MCNormalBattleImpl::_init()
{
    return true;
}

MCNormalBattleImpl *
MCNormalBattleImpl::sharedBattleImplInstance()
{
    if (__shread_battle_imple_instance == NULL) {
        __shread_battle_imple_instance = new MCNormalBattleImpl;
        if (__shread_battle_imple_instance && __shread_battle_imple_instance->_init()) {
            __shread_battle_imple_instance->autorelease();
        } else {
            delete __shread_battle_imple_instance;
            __shread_battle_imple_instance = NULL;
        }
    }
    
    return __shread_battle_imple_instance;
}

