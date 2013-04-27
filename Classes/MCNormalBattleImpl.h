//
//  MCNormalBattleImpl.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCNormalBattleImpl__
#define __Military_Confrontation__MCNormalBattleImpl__

#include "MCBattle.h"

class MCNormalBattleImpl : public MCBattleProtocol {
public:
    MCNormalBattleImpl() { };
    
    bool _init();
    
public:
    static MCNormalBattleImpl *sharedBattleImplInstance();
};

#endif /* defined(__Military_Confrontation__MCNormalBattleImpl__) */
