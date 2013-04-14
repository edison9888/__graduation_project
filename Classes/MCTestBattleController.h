//
//  MCTestBattleController.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestBattleController__
#define __Military_Confrontation__MCTestBattleController__

#include "MCGameScene.h"
#include "MCHero.h"
#include "MCTestLayer.h"
#include "MCSceneManager.h"
#include "MCGameState.h"
#include "MCMercenaryManager.h"
#include "MCBackpack.h"
#include "MCTaskManager.h"

class MCTestBattleController : public MCTestLayer {
public:
    
    static CCScene *scene() {
        MCGameState::sharedGameState()->load();
        MCBackpack::sharedBackpack()->setMoney(10000);
        
        mc_object_id_t m_id = {
            'M', '0', '0', '2'
        };

        mc_ssize_t ret;
        m_id.sub_class_ = '9';
        m_id.sub_index_ = '1';
        ret = MCMercenaryManager::sharedMercenaryManager()->hire(m_id);
        MCRole *role = MCTeam::sharedTeam()->lastRole();
        role->setHP(40);
        role->setPP(40);
        ret = MCMercenaryManager::sharedMercenaryManager()->hire(m_id);
        role = MCTeam::sharedTeam()->lastRole();
        role->setHP(60);
        role->setPP(60);
        m_id.sub_index_ = '2';
        ret = MCMercenaryManager::sharedMercenaryManager()->hire(m_id);
        role = MCTeam::sharedTeam()->lastRole();
        role->setHP(80);
        role->setPP(40);
        m_id.sub_class_ = '8';
        ret = MCMercenaryManager::sharedMercenaryManager()->hire(m_id);
        role = MCTeam::sharedTeam()->lastRole();
        role->setHP(80);
        role->setPP(40);
        ret = MCMercenaryManager::sharedMercenaryManager()->hire(m_id);
        role = MCTeam::sharedTeam()->lastRole();
        role->setHP(80);
        role->setPP(40);
        
        MCBackpack::sharedBackpack()->buyFireballTrapWide(2);
        MCBackpack::sharedBackpack()->buyHealthPotion(20);
        MCBackpack::sharedBackpack()->buyPhysicalPotion(5);
        
        mc_object_id_t t_id = {
            'T', '1', '0', '1'
        };
        MCTask *task = MCTaskManager::sharedTaskManager()->taskWithObjectId(t_id);
        MCTaskManager::sharedTaskManager()->acceptTask(task);
        MCTaskManager::sharedTaskManager()->startCurrentTask();
        
        m_id.sub_class_ = '1';
        m_id.index_ = '0';
        m_id.sub_index_ = '1';
        CCScene *scene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);

        scene->init();
        MCTestBattleController *s = new MCTestBattleController;
        s->initWithColor(ccc4(0, 0, 0, 0));
        scene->addChild(s);
        
        return scene;
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

#endif /* defined(__Military_Confrontation__MCTestBattleController__) */
