//
//  MCGameOverScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCGameOverScene.h"
#include "MCGameState.h"
#include "MCSceneController.h"

bool
MCGameOver::init()
{
    if (CCLayerColor::initWithColor(ccc4(24, 24, 24, 255))) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();

        CCLabelTTF *gameover = CCLabelTTF::create("Game Over", "", 64 / contentScaleFactor);
        addChild(gameover);
        gameover->setPosition(ccp(winSize.width / 2,
                                  winSize.height / 2 + gameover->getContentSize().height));
        
        CCMenu *menu = CCMenu::create();
        addChild(menu);
        
        CCMenuItemLabel *menuItem;
        CCLabelTTF *label;
        
        label = CCLabelTTF::create("继续游戏", "", 32 / contentScaleFactor);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCGameOver::continueGame));
        menu->addChild(menuItem);
        
        label = CCLabelTTF::create("退出游戏", "", 32 / contentScaleFactor);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCGameOver::exit));
        menu->addChild(menuItem);
        
        menu->alignItemsVertically();
        menu->setPosition(ccp(winSize.width / 2,
                              winSize.height / 2 - label->getContentSize().height));
        
        return true;
    }
    
    return false;
}

void
MCGameOver::confirmDidClickYesButton(MCConfirm *aConfirm)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    exit(0);
#endif
}

void
MCGameOver::continueGame(CCObject *anObject)
{
    MCGameState::sharedGameState()->load();
    MCSceneController::sharedSceneController()->loadSpawnScene(0.0f);
}

void
MCGameOver::exit(CCObject *anObject)
{
    MCConfirm::confirm(this, this, "确定退出？");
}
