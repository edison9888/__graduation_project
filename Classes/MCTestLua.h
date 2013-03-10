//
//  MCTestLua.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestLua__
#define __Military_Confrontation__MCTestLua__

#include "MCTestLayer.h"

USING_NS_CC;

class MCRoleEntity;

class MCTestLua : public MCTestLayer {
public:
    bool init();
    
    static CCScene *scene()
    {
        CCScene *scene = CCScene::create();
        
        MCTestLua * l = new MCTestLua;
        
        l->init();
        l->autorelease();
        scene->addChild(l);
        
        return scene;
    }
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void moveMenu(const CCPoint &delta);
    
    void onEnter();
    
    void runScript(CCObject *anObject);
    
    CCMenu *menu_;
};

#endif /* defined(__Military_Confrontation__MCTestLua__) */
