//
//  MCMainMenuScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCMainMenuScene.h"

#include "MCTestControllerScene.h"

bool
MCMainMenuLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(24, 24, 24, 255)))
    {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCMenuItemLabel *item;
        CCMenu *menu;
        CCLabelTTF *label;
        
        /* Play Game */
        menu = CCMenu::create();
        label = CCLabelTTF::create("PLAY GAME", "Marker Felt", 64);
        label->setColor(ccc3(64, 128, 216));
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenuLayer::playGame_clicked));
        menu->addChild(item);
        
        label = CCLabelTTF::create("multiplayer", "Marker Felt", 24);
        label->setColor(ccc3(64, 128, 216));
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenuLayer::multiplayer_clicked));
        menu->addChild(item);
        menu->alignItemsVerticallyWithPadding(12);
        
        addChild(menu);
        
        /* About */
        menu = CCMenu::create();
        label = CCLabelTTF::create("About", "Marker Felt", 18);
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenuLayer::about_clicked));
        menu->addChild(item);
        addChild(menu);
        menu->setPosition(ccp(item->getContentSize().width,
                              item->getContentSize().height));
        
        /* Quit */
        menu = CCMenu::create();
        label = CCLabelTTF::create("Quit", "Marker Felt", 18);
        label->setColor(ccc3(240, 13, 13));
        item = CCMenuItemLabel::create(label,
                                       this,
                                       menu_selector(MCMainMenuLayer::quit_clicked));
        menu->addChild(item);
        addChild(menu);
        menu->setPosition(ccp(winSize.width - item->getContentSize().width,
                              item->getContentSize().height));
                
        return true;
    }
    
    return false;
}

void
MCMainMenuLayer::playGame_clicked(CCObject* sender)    /* 单人游戏 */
{
    CCDirector::sharedDirector()->replaceScene(MCTestControllerLayer::scene());
}

void
MCMainMenuLayer::multiplayer_clicked(CCObject* sender) /* 多人对战 */
{
    
}

void
MCMainMenuLayer::about_clicked(CCObject* sender)       /* 关于 */
{
    
}

void
MCMainMenuLayer::quit_clicked(CCObject* sender)        /* 退出 */
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    exit(0);
#endif
}
