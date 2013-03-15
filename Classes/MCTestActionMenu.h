//
//  MCTestActionMenu.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestActionMenu__
#define __Military_Confrontation__MCTestActionMenu__

#include "MCTestLayer.h"
#include "MCActionMenu.h"
#include "MCSceneManager.h"
#include "MCGameScene.h"
#include "MCHero.h"

class MCTestActionMenu : public MCTestLayer {
public:
    bool init();
    
    static CCScene *scene() {
        mc_object_id_t m_id = {
            'M', '0', '0', '1'
        };
        MCGameScene *scene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);
        
        MCTestActionMenu *s = new MCTestActionMenu;
        
        CCArray *actionGroup = CCArray::create();
        actionGroup->retain();
        MCMenuItemAction *action;
        CCString *label;
        
        action = new MCMenuItemAction;
        action->retain();
        action->setID(1);
        label = CCString::create("移动");
        label->retain();
        action->setLabel(label);
        action->setTarget(s);
        action->setMenuHandler(menu_selector(MCTestActionMenu::action1));
        actionGroup->addObject(action);
        
        action = new MCMenuItemAction;
        action->retain();
        action->setID(2);
        label = CCString::create("技能");
        label->retain();
        action->setLabel(label);
        action->setTarget(s);
        action->setMenuHandler(menu_selector(MCTestActionMenu::action2));
        actionGroup->addObject(action);
        
        action = new MCMenuItemAction;
        action->retain();
        action->setID(3);
        label = CCString::create("道具");
        label->retain();
        action->setLabel(label);
        action->setTarget(s);
        action->setMenuHandler(menu_selector(MCTestActionMenu::action3));
        actionGroup->addObject(action);
        
        action = new MCMenuItemAction;
        action->retain();
        action->setID(4);
        label = CCString::create("休息");
        label->retain();
        action->setLabel(label);
        action->setTarget(s);
        action->setMenuHandler(menu_selector(MCTestActionMenu::action4));
        actionGroup->addObject(action);
        
//        s->actionFixedMenu = MCActionMenu::create(MCFixedMenu);
//        CCLog("fixed: %p", s->actionFixedMenu);
//        s->actionFixedMenu->addActionGroup(1, actionGroup);
//        s->actionFixedMenu->setActiveActionGroup(1);
//        s->actionFixedMenu->attach(scene);
//        s->actionFloatMenu = MCActionMenu::create(MCFixedMenu);
//        CCLog("float: %p", s->actionFloatMenu);
        s->initWithColor(ccc4(0, 0, 0, 0));
//        scene->actionMenu_->addActionGroup(1, actionGroup);
//        scene->actionMenu_->setActiveActionGroup(1);
//        scene->actionMenu_->attach(scene);
        scene->addChild(s);
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
        
        return scene;
    }
    
    void action1(CCObject *obj) {
        CCLog("action1");
    }
    void action2(CCObject *obj) {
        CCLog("action2");
    }
    void action3(CCObject *obj) {
        CCLog("action3");
    }
    void action4(CCObject *obj) {
        CCLog("action4");
    }
    
    void onExit() {
        MCTestLayer::onExit();
    }
    
//    MCActionMenu *actionFixedMenu;
//    MCActionMenu *actionFloatMenu;
};

#endif /* defined(__Military_Confrontation__MCTestActionMenu__) */
