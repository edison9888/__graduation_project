//
//  MCMainMenuScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include <cocos-ext.h>

#include "AppMacros.h"
#include "MCMainMenuScene.h"
#include "MCGameState.h"
#include "MCSceneController.h"
#include "MCDungeonMaster.h"
#include "MCDialog.h"

USING_NS_CC_EXT;

static const float kMCMajorFontSize = 64.0f;
static const float kMCNormalFontSize = 36.0f;
static const CCSize kMCBackgroundSize = CCSizeMake(960.0f, 640.0f);
static const ccColor3B kMCMenuTextColor = ccc3(24, 24, 240);
static const ccColor3B kMCAboutTextColor = ccc3(240, 240, 240);
static const ccColor3B kMCQuitTextColor = ccc3(240, 13, 13);

bool
MCMainMenu::init()
{
    if (CCLayerColor::initWithColor(ccc4(24, 24, 24, 255))) {
        CCSize winSize = CCDirectorGetWindowsSize();
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        CCMenuItemLabel *item;
        CCMenu *menu;
        CCLabelTTF *label;
        MCGameState *gameState = MCGameState::sharedGameState();
        bool isSaveFileExists = gameState->isSaveFileExists();
        
        /* bg */
        CCSprite *bg = CCSprite::create("menu.jpg");
        addChild(bg);
        bg->setScaleX(winSize.width / kMCBackgroundSize.width * contentScaleFactor);
        bg->setScaleY(winSize.height / kMCBackgroundSize.height * contentScaleFactor);
        bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        
        menu = CCMenu::create();
        
        /* continue */
        if (isSaveFileExists) {
            label = CCLabelTTF::create("继续游戏", "Marker Felt", kMCMajorFontSize);
            label->setColor(kMCMenuTextColor);
            item = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCMainMenu::continue_clicked));
            menu->addChild(item);
        }
        
        /* Play Game */
        label = CCLabelTTF::create("开始游戏",
                                   "Marker Felt",
                                   isSaveFileExists ? kMCNormalFontSize : kMCMajorFontSize);
        label->setColor(kMCMenuTextColor);
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenu::playGame_clicked));
        menu->addChild(item);
#if MC_MULTIPLAYER_SUPPORT == 1
        /* multiplayer */
        label = CCLabelTTF::create("多人游戏", "Marker Felt", kMCNormalFontSize);
        label->setColor(kMCMenuTextColor);
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenu::multiplayer_clicked));
        menu->addChild(item);
#endif
        menu->alignItemsVerticallyWithPadding(12);
        addChild(menu);
        
        /* About */
        menu = CCMenu::create();
        label = CCLabelTTF::create("关于", "Marker Felt", kMCNormalFontSize);
        label->setColor(kMCAboutTextColor);
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenu::about_clicked));
        menu->addChild(item);
        addChild(menu);
        menu->setPosition(ccp(item->getContentSize().width,
                              item->getContentSize().height));
        
        /* Quit */
        menu = CCMenu::create();
        label = CCLabelTTF::create("退出", "Marker Felt", kMCNormalFontSize);
        label->setColor(kMCQuitTextColor);
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
