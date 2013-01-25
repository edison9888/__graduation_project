//
//  MCMutiplayerBattleImpl.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMutiplayerBattleImpl__
#define __Military_Confrontation__MCMutiplayerBattleImpl__

#include "MCBattle.h"

class MCMutiplayerBattleImpl : public MCBattleProtoacol {
public:
    MCMutiplayerBattleImpl() { };
    
    bool _init();
    
public:
    static MCMutiplayerBattleImpl *sharedBattleImplInstance();
};

#endif /* defined(__Military_Confrontation__MCMutiplayerBattleImpl__) */
