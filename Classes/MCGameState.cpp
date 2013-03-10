//
//  MCGameState.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCGameState.h"

static MCGameState *__shared_game_state = NULL;

MCGameState::MCGameState()
{
    
}

MCGameState::~MCGameState()
{
    
}

MCGameState *
MCGameState::sharedGameState()
{
    if (__shared_game_state == NULL) {
        __shared_game_state = new MCGameState;
    }
    
    return __shared_game_state;
}

/**
 * 保存checkpoint
 */
void
MCGameState::saveCheckPoint()
{
    
}

/**
 * 读取checkpoint
 */
void
MCGameState::loadCheckPoint()
{
    
}

/**
 * 中断游戏
 */
void
MCGameState::interrupt()
{
    
}

/**
 * 读取中断的游戏
 */
void
MCGameState::loadInterruption()
{
    
}