//
//  MCBattleFieldScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-15.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBattleFieldScene__
#define __Military_Confrontation__MCBattleFieldScene__

#include "MCScene.h"

class MCBattleFieldScene : public MCScene {
public:
    MCBattleFieldScene()
    : usingJoypad_(false) {}
    
    void installController();
    
    void showPlayerInfo();
    void playerInfoDidHide();
    
protected:
    bool usingJoypad_;
};

#endif /* defined(__Military_Confrontation__MCBattleFieldScene__) */
