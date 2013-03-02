//
//  MCTestGame.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-28.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestGame__
#define __Military_Confrontation__MCTestGame__

#include "MCSplashScene.h"
#include "MCMainMenuScene.h"

class MCTestGame {
public:
    static CCScene *scene() {
        CCDirector::sharedDirector()->pushScene(MCMainMenu::scene());
        return MCSplash::scene();
    }
};

#endif /* defined(__Military_Confrontation__MCTestGame__) */
