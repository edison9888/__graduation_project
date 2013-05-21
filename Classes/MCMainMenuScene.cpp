//
//  MCMainMenuScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCMainMenuScene.h"
#include "MCGameState.h"
#include "MCSceneController.h"
#include "MCDungeonMaster.h"
#include "MCDialog.h"

static const float kMCMajorFontSize = 64.0f;
static const float kMCNormalFontSize = 36.0f;

bool
MCMainMenu::init()
{
    if (CCLayerColor::initWithColor(ccc4(24, 24, 24, 255))) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCMenuItemLabel *item;
        CCMenu *menu;
        CCLabelTTF *label;
        MCGameState *gameState = MCGameState::sharedGameState();
        bool isSaveFileExists = gameState->isSaveFileExists();
        
        menu = CCMenu::create();
        
        /* continue */
        if (isSaveFileExists) {
            label = CCLabelTTF::create("continue", "Marker Felt", kMCMajorFontSize);
            label->setColor(ccc3(64, 128, 216));
            item = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCMainMenu::continue_clicked));
            menu->addChild(item);
        }
        
        /* Play Game */
        label = CCLabelTTF::create("PLAY GAME",
                                   "Marker Felt",
                                   isSaveFileExists ? kMCNormalFontSize : kMCMajorFontSize);
        label->setColor(ccc3(64, 128, 216));
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenu::playGame_clicked));
        menu->addChild(item);
#if MC_MULTIPLAYER_SUPPORT == 1
        /* multiplayer */
        label = CCLabelTTF::create("multiplayer", "Marker Felt", kMCNormalFontSize);
        label->setColor(ccc3(64, 128, 216));
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenu::multiplayer_clicked));
        menu->addChild(item);
#endif
        menu->alignItemsVerticallyWithPadding(12);
        addChild(menu);
        
        /* About */
        menu = CCMenu::create();
        label = CCLabelTTF::create("About", "Marker Felt", kMCNormalFontSize);
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenu::about_clicked));
        menu->addChild(item);
        addChild(menu);
        menu->setPosition(ccp(item->getContentSize().width,
                              item->getContentSize().height));
        
        /* Quit */
        menu = CCMenu::create();
        label = CCLabelTTF::create("Quit", "Marker Felt", kMCNormalFontSize);
        label->setColor(ccc3(240, 13, 13));
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenu::quit_clicked));
        menu->addChild(item);
        addChild(menu);
        menu->setPosition(ccp(winSize.width - item->getContentSize().width,
                              item->getContentSize().height));
        
        confirmOperation_ = MCUnknownOperation;
                
        return true;
    }
    
    return false;
}

void
MCMainMenu::confirmDidClickYesButton(MCConfirm *aConfirm)
{
    if (confirmOperation_ == MCStartNewGameOperation) {
        MCDungeonMaster::sharedDungeonMaster()->destroyGameWorld();
        MCGameState::sharedGameState()->load();
        MCSceneController::sharedSceneController()->loadSpawnScene(0.0f);
    } else if (confirmOperation_ == MCQuitOperation) {
        CCDirector::sharedDirector()->end();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        exit(0);
#endif
    }
}

void
MCMainMenu::continue_clicked(CCObject *aSender)    /* 继续游戏 */
{
    MCGameState::sharedGameState()->load();
    MCSceneController::sharedSceneController()->loadSpawnScene(0.0f);
}

void
MCMainMenu::playGame_clicked(CCObject *aSender)    /* 单人游戏 */
{
    confirmOperation_ = MCStartNewGameOperation;
    if (MCGameState::sharedGameState()->isSaveFileExists()) {
        MCConfirm::confirm(this, this, "要开始新游戏吗？");
    } else {
        MCGameState::sharedGameState()->load();
        MCSceneController::sharedSceneController()->loadSpawnScene(0.0f);
    }
}

#if MC_MULTIPLAYER_SUPPORT == 1
void
MCMainMenu::multiplayer_clicked(CCObject *aSender) /* 多人对战 */
{
    
}
#endif

void
MCMainMenu::about_clicked(CCObject *aSender)       /* 关于 */
{
    MCDungeonMaster::sharedDungeonMaster()->speak("僕のゲームへようこそ！");
}

void
MCMainMenu::quit_clicked(CCObject *aSender)        /* 退出 */
{
    confirmOperation_ = MCQuitOperation;
    MCConfirm::confirm(this, this, "确定退出？");
}
