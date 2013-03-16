//
//  MCBattleFieldScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-15.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBattleFieldScene__
#define __Military_Confrontation__MCBattleFieldScene__

#include "MCGameScene.h"

class MCBattleFieldScene : public MCGameScene {
public:
    void installController();
    
    void dragScene(const CCPoint &anOffset);
};

#endif /* defined(__Military_Confrontation__MCBattleFieldScene__) */
