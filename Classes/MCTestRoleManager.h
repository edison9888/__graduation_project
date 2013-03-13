//
//  MCTestRoleManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestRoleManager__
#define __Military_Confrontation__MCTestRoleManager__

#include "MCTestLayer.h"

class MCTestRoleManager : public MCTestLayer {
public:
    bool init();
    
    static CCScene *scene()
    {
        CCScene *scene = CCScene::create();
    
        MCTestRoleManager * l = new MCTestRoleManager;
    
        l->init();
        l->autorelease();
        scene->addChild(l);
    
        return scene;
    }
};

#endif /* defined(__Military_Confrontation__MCTestRoleManager__) */
