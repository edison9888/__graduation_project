//
//  MCTestDetail.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestDetail__
#define __Military_Confrontation__MCTestDetail__

#include "MCGameScene.h"
#include "MCHero.h"
#include "MCTestLayer.h"
#include "MCSceneManager.h"
#include "MCGameState.h"
#include "MCMercenaryManager.h"
#include "MCBackpack.h"
#include "MCTaskManager.h"
#include "MCDetail.h"
#include "MCEquipmentManager.h"

class MCTestDetail : public MCTestLayer {
public:
    
    static CCScene *scene() {
        MCGameState::sharedGameState()->load();
        MCBackpack::sharedBackpack()->setMoney(10000);
        
        mc_object_id_t m_id = {
            'M', '0', '0', '2'
        };
        MCBackpack::sharedBackpack()->buyHealthPotion(20);
        MCBackpack::sharedBackpack()->buyPhysicalPotion(5);
        
        mc_object_id_t t_id = {
            'T', '4', '0', '7'
        };
        MCTask *task = MCTaskManager::sharedTaskManager()->taskWithObjectId(t_id);
        MCTaskManager::sharedTaskManager()->acceptTask(task);
        MCTaskManager::sharedTaskManager()->startCurrentTask();
        
        m_id.sub_class_ = '1';
        m_id.index_ = '0';
        m_id.sub_index_ = '1';
        CCScene *scene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);
        mc_ssize_t ret;
        
        MCHero::sharedHero()->setRoleState(MCParalysisState | MCPoisonedState);
        MCEquipmentItem *warhammer = MCEquipmentManager::sharedEquipmentManager()->getWarhammer();
        MCEquipmentManager::sharedEquipmentManager()->changeWeaponTo(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(warhammer);
        ret = MCEquipmentManager::sharedEquipmentManager()->levelUp(MCEquipmentManager::sharedEquipmentManager()->getShinGuard());
        
        scene->init();
        MCTestDetail *s = new MCTestDetail;
        s->scene_ = dynamic_cast<MCScene *>(scene);
        s->initWithColor(ccc4(0, 0, 0, 0));
        scene->addChild(s);
        
        return scene;
    }
    
    MCScene *scene_;
    
    void onEnter() {
        MCTestLayer::onEnter();
        scene_->showDetail();
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

#endif /* defined(__Military_Confrontation__MCTestDetail__) */
