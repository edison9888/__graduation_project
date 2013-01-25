//
//  MCBattle.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBattle__
#define __Military_Confrontation__MCBattle__

#include <cocos2d.h>

USING_NS_CC;

enum {
    MCNormalBattle,
    MCMutiplayerBattle
};
typedef unsigned int MCBattleType;

class MCBattleProtoacol;

class MCBattle : public CCObject {
private:
    MCBattle() { };
    
    bool _init();
    
public:
    static MCBattle *sharedBattle();
    
    MCBattleProtoacol *battleWithType(MCBattleType type);
};

class MCBattleProtoacol : public CCObject {
public:
};

#endif /* defined(__Military_Confrontation__MCBattle__) */
