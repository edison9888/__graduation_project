//
//  MCTestOBB.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestOBB__
#define __Military_Confrontation__MCTestOBB__

#include "MCTestLayer.h"
#include "MCOBB.h"
#include "MCRoleEntity.h"

USING_NS_CC;

class MCTestOBB : public MCTestLayer {
    MCRoleEntity *a;
    MCRoleEntity *b;
    
public:
    bool init() {
        if (MCTestLayer::initWithColor(ccc4(64, 64, 64, 255))) {
            a = new MCRoleEntity;
            a->initWithFile("block.png");
            addChild(a);
            b = new MCRoleEntity;
            b->initWithFile("block.png");
            b->setColor(ccc3(10, 20, 240));
            addChild(b);
            
            a->setPosition(ccp(190,220));
            b->setPosition(ccp(200,170));
            
            obbA = a->getOBB();
            obbB = b->getOBB();
            
            return true;
        }
    
        return false;
    }

    
    static CCScene *scene() {
        CCScene *scene = CCScene::create();
        
        MCTestOBB * l = new MCTestOBB;
    
        l->init();
        l->autorelease();
        scene->addChild(l);
    
        return scene;
    }
    
    void onEnter() {
        MCTestLayer::onEnter();
        schedule(schedule_selector(MCTestOBB::update));
    }
    void onExit() {
        unschedule(schedule_selector(MCTestOBB::update));
        MCTestLayer::onExit();
    }
    void update(float dt) {
        a->setAnchorPoint(ccp(0.5, 0.5));
        a->setRotation(a->getRotation() + 1);
        b->setAnchorPoint(ccp(0.5, 0.5));
        b->setRotation(b->getRotation() + -1);
        
        obbA.setup(a->getPosition(), 72, 24, CC_DEGREES_TO_RADIANS(a->getRotation()));
        obbB.setup(b->getPosition(), 72, 24, CC_DEGREES_TO_RADIANS(b->getRotation()));
        
        if (obbA.collidesWith(obbB)) {
            b->setOpacity(64);
        } else {
            b->setOpacity(255);
        }
    }
    
private:
    MCOBB obbA;
    MCOBB obbB;
};

#endif /* defined(__Military_Confrontation__MCTestOBB__) */
