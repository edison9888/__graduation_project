//
//  MCTestFlagManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestFlagManager__
#define __Military_Confrontation__MCTestFlagManager__

#include "MCTestLayer.h"

USING_NS_CC;

class MCRoleEntity;

class MCTestFlagManager : public MCTestLayer {
public:
    bool init();
    
    static CCScene *scene()
    {
        CCScene *scene = CCScene::create();
    
        MCTestFlagManager * l = new MCTestFlagManager;
    
        l->init();
        l->autorelease();
        scene->addChild(l);
    
        return scene;
    }
};

#endif /* defined(__Military_Confrontation__MCTestFlagManager__) */
