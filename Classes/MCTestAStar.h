//
//  MCTestAStar.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestAStar__
#define __Military_Confrontation__MCTestAStar__

#include "MCTestLayer.h"
#include "MCAStar.h"
#include "MCGameScene.h"
#include "MCHero.h"
#include "MCSceneManager.h"

class MCTestAStar : public MCTestLayer {
public:
    
    static CCScene *scene() {
        mc_object_id_t m_id = {
            'M', '0', '0', '1'
        };
        CCScene *scene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);
        
        scene->init();
        MCTestAStar *l = new MCTestAStar;
        l->initWithColor(ccc4(0, 0, 0, 0));
        
        scene->addChild(l);
        
        return scene;
    }
    
    void onEnter() {
        MCTestLayer::onEnter();
//        MCSceneContext *c = MCSceneContextManager::sharedSceneContextManager()->currentContext();
        MCRoleEntity *e = MCHero::sharedHero()->getEntity();
        e->setPosition(ccp(100, 100));
        e->findPath(ccp(450, 432));
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};


#endif /* defined(__Military_Confrontation__MCTestAStar__) */
