//
//  MCTestbed.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-28.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTestbed.h"
#include "MCRoleManager.h"
#include "MCEffectManager.h"

#include "MCTestLua.h"
#include "MCTestGame.h"
#include "MCTestManagers.h"
#include "MCTestRoleManager.h"
#include "MCTestGameScene.h"
#include "MCTestViewport.h"
#include "MCTestOBB.h"
#include "MCTestRoleBaseInfo.h"
#include "MCTestAStar.h"
#include "MCTestBattleController.h"
#include "MCTestParticle.h"
#include "MCTestDetail.h"
#include "MCTestUI.h"
#include "MCTestEffect.h"

const CCPoint curPos = ccp(0, 0);

typedef CCScene *(MCCreateFunction)();

struct __mc_test_scene {
    const char *name;
    MCCreateFunction *createFunction;
};

class __mc_quit : public CCLayer {
public:
    static CCScene *scene() {
        CCScene *s = CCScene::create();
        
        s->addChild(__mc_quit::create());
        
        return s;
    }
    
    bool init() { return true; }
    
    CREATE_FUNC(__mc_quit);

    void onEnter() {
        CCLayer::onEnter();
        CCDirector::sharedDirector()->end();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        exit(0);
#endif
    }
};

static struct __mc_test_scene __test_scenes[] = {
    {"进入游戏", MCTestGame::scene},
    {"跳过启动界面", MCTestMenuScene::scene},
//    {"Test Detail", MCTestDetail::scene},
//    {"Test A*", MCTestAStar::scene},
//    {"Test Particle", MCTestParticle::scene},
    {"测试游戏场景", MCTestGameScene::scene},
    {"测试战斗场景", MCTestBattleController::scene},
//    {"Test Issuing Task UI", MCTestIssuingTaskUI::scene},
//    {"Test Trading Props UI", MCTestTradingPropsUI::scene},
//    {"Test Enhancing Equipment UI", MCTestEnhancingEquipmentUI::scene},
//    {"Test Hiring Mercenary UI", MCTestHiringMercenaryUI::scene},
//    {"Test Effect", MCTestEffect::scene},
//    {"Test Lua", MCTestLua::scene},
//    {"Test Flag Manager", MCTestFlagManager::scene},
//    {"Test Task Manager", MCTestTaskManager::scene},
//    {"Test Backpack ", MCTestBackpack::scene},
//    {"Test Role Manager", MCTestRoleManager::scene},
//    {"Test Viewport", MCTestViewport::scene},
//    {"Test OBB", MCTestOBB::scene},
//    {"Test Role Base Info", MCTestRoleBaseInfo::scene},
    {"退出", __mc_quit::scene}
};

bool
MCTestbed::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        size_t count = sizeof(__test_scenes) / sizeof(struct __mc_test_scene);
        CCMenu *menu = CCMenu::create();
        float lineSpace = 64.0f / contentScaleFactor;
        menu_ = menu;
        addChild(menu);
        
        for (int i = 0; i < count; ++i) {
            CCLabelTTF *label = CCLabelTTF::create(__test_scenes[i].name,
                                                   "",
                                                   48.0f / contentScaleFactor);
            CCMenuItemLabel *item = CCMenuItemLabel::create(label, this, menu_selector(MCTestbed::testbedDidSelectScene));
            item->setUserData((void *)__test_scenes[i].createFunction);
            menu->addChild(item, i + 10000);
            item->setPosition(winSize.width / 2, (winSize.height - (i + 1) * lineSpace));
        }
        
        menu->setContentSize(CCSizeMake(winSize.width, (count + 1) * lineSpace));
        menu->setPosition(curPos);
        
        setTouchEnabled(true);
        
        /* 加载资源 */
        MCRoleManager::sharedRoleManager();
        
        return true;
    }
    
    return false;
}

void
MCTestbed::testbedDidSelectScene(CCObject *pSender)
{
    CCMenuItem *item = (CCMenuItem *)pSender;
    MCCreateFunction *createFunction = (MCCreateFunction *) item->getUserData();
    CCScene *scene = createFunction();
    CCDirector::sharedDirector()->pushScene(scene);
}

void
MCTestbed::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    CCPoint delta = touch->getDelta();
    
    moveMenu(delta);
}

void
MCTestbed::moveMenu(const CCPoint &delta) {
    CCPoint current = menu_->getPosition();
    float newY = current.y + delta.y;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    size_t count = sizeof(__test_scenes) / sizeof(struct __mc_test_scene);
    float lineSpace = 64.0f / CC_CONTENT_SCALE_FACTOR();
    
    if (newY < 0)
        newY = 0;
    
    if( newY > ((count + 1) * lineSpace - winSize.height))
        newY = ((count + 1) * lineSpace - winSize.height);
    
    menu_->setPosition(current.x, newY);
}
