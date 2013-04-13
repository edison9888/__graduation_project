//
//  MCTestIssuingTaskUI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestIssuingTaskUI__
#define __Military_Confrontation__MCTestIssuingTaskUI__

#include "MCTestLayer.h"
#include "MCAStar.h"
#include "MCGameScene.h"
#include "MCHero.h"
#include "MCSceneManager.h"
#include "MCScript.h"
#include "MCIssuingTaskUI.h"
#include "MCBackpack.h"

class MCTestIssuingTaskUI : public MCTestLayer {
public:
    
    static CCScene *scene() {
        mc_object_id_t m_id = {
            'M', '0', '0', '5'
        };
        CCScene *scene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);
        
        scene->init();
        MCTestIssuingTaskUI *l = new MCTestIssuingTaskUI;
        l->initWithColor(ccc4(0, 0, 0, 0));
        
        scene->addChild(l);
        
        return scene;
    }
    
    void onEnter() {
        MCTestLayer::onEnter();
        MCBackpack::sharedBackpack()->setMoney(100);
        MCHero::sharedHero()->getEntity()->setPosition(ccp(720, 400));
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

class MCTestTradingPropsUI : public MCTestLayer {
public:
    
    static CCScene *scene() {
        mc_object_id_t m_id = {
            'M', '0', '0', '3'
        };
        CCScene *scene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);
        
        scene->init();
        MCTestTradingPropsUI *l = new MCTestTradingPropsUI;
        l->initWithColor(ccc4(0, 0, 0, 0));
        
        scene->addChild(l);
        
        return scene;
    }
    
    void onEnter() {
        MCTestLayer::onEnter();
        MCBackpack::sharedBackpack()->setMoney(100);
        MCHero::sharedHero()->getEntity()->setPosition(ccp(400, 200));
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

class MCTestEnhancingEquipmentUI : public MCTestLayer {
public:
    
    static CCScene *scene() {
        mc_object_id_t m_id = {
            'M', '0', '0', '3'
        };
        CCScene *scene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);
        
        scene->init();
        MCTestEnhancingEquipmentUI *l = new MCTestEnhancingEquipmentUI;
        l->initWithColor(ccc4(0, 0, 0, 0));
        
        scene->addChild(l);
        
        return scene;
    }
    
    void onEnter() {
        MCTestLayer::onEnter();
        MCBackpack::sharedBackpack()->setMoney(100);
        MCHero::sharedHero()->getEntity()->setPosition(ccp(100, 440));
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

class MCTestHiringMercenaryUI : public MCTestLayer {
public:
    
    static CCScene *scene() {
        mc_object_id_t m_id = {
            'M', '0', '0', '5'
        };
        CCScene *scene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);
        
        scene->init();
        MCTestHiringMercenaryUI *l = new MCTestHiringMercenaryUI;
        l->initWithColor(ccc4(0, 0, 0, 0));
        
        scene->addChild(l);
        
        return scene;
    }
    
    void onEnter() {
        MCTestLayer::onEnter();
        MCBackpack::sharedBackpack()->setMoney(100);
        MCHero::sharedHero()->getEntity()->setPosition(ccp(250, 400));
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

#endif /* defined(__Military_Confrontation__MCTestIssuingTaskUI__) */
