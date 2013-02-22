//
//  MCDetailScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-29.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCDetailScene.h"

enum __detail_scene__Tags {
    Tag_State,
    Tag_Props,
    Tag_Equipment,
    Tag_Tasks,
    Tag_Skills,
    Tag_Mercenary,
    Tag_Quit,
};

class __MCItemSelectorLayer : public CCLayer {
public:
    bool initWithTarget(CCObject *target);
   
    CC_SYNTHESIZE_READONLY(CCMenuItem *, defaultMenuItem_, DefaultMenuItem);
};

bool
__MCItemSelectorLayer::initWithTarget(CCObject *target)
{
    if (CCLayer::init()) {
        CCMenuItem *menuItem;
        CCMenu *menu;
        
        CCSize winSize = CCDirectorGetWindowsSize();
        CCMenuItemImage *backwardMenuItem = CCMenuItemImage::create("back.png", "back_pressed.png");
        backwardMenuItem->setTarget(target, menu_selector(MCDetailScene::backward));
        menu = CCMenu::create();
        addChild(menu);
        menu->addChild(backwardMenuItem);
        menu->setPosition(ccp(backwardMenuItem->getContentSize().width / 2 - 16,
                              winSize.height - backwardMenuItem->getContentSize().height / 2 + 16));
        
        menu = CCMenu::create();
        addChild(menu);
        
        menuItem = CCMenuItemImage::create("user_info.png", "user_info_selected.png");
        menu->addChild(menuItem);
        defaultMenuItem_ = menuItem;
        menuItem->setTag(Tag_State);
        menuItem->setTarget(target, menu_selector(MCDetailScene::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("props.png", "props_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(Tag_Props);
        menuItem->setTarget(target, menu_selector(MCDetailScene::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("equipment.png", "equipment_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(Tag_Equipment);
        menuItem->setTarget(target, menu_selector(MCDetailScene::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("tasks.png", "tasks_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(Tag_Tasks);
        menuItem->setTarget(target, menu_selector(MCDetailScene::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("skills.png", "skills_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(Tag_Skills);
        menuItem->setTarget(target, menu_selector(MCDetailScene::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("mercenary.png", "mercenary_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(Tag_Mercenary);
        menuItem->setTarget(target, menu_selector(MCDetailScene::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("quit.png", "quit_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(Tag_Quit);
        menuItem->setTarget(target, menu_selector(MCDetailScene::menuItem_clicked));
        
        menu->alignItemsVerticallyWithPadding(0);
        menu->setPosition(ccp(menuItem->getContentSize().width / 2, menu->getPosition().y));
        
        return true;
    }
    
    return false;
}

bool
MCDetailScene::init()
{
    if (CCScene::init()) {
        CCSprite *bg = CCSprite::create("bg.jpg");
        addChild(bg);
        bg->setAnchorPoint(CCPointZero);
        bg->setPosition(CCPointZero);
        
        __MCItemSelectorLayer *menuLayer = new __MCItemSelectorLayer;
        menuLayer->initWithTarget(this);
        menuLayer->autorelease();
        lastSelectedMenuItem_ = menuLayer->getDefaultMenuItem();
        addChild(menuLayer);
        
        stateLayer_ = MCStateLayer::create();
        lastShownLayer_ = stateLayer_;
        addChild(stateLayer_);
        
        return true;
    }
    
    return false;
}
    
void
MCDetailScene::onEnter()
{
    CCScene::onEnter();
    lastSelectedMenuItem_->selected();
    lastShownLayer_->show();
}

void
MCDetailScene::onExit()
{
    CCScene::onExit();
}

void
MCDetailScene::showState() /* 显示状态选项卡 */
{
    stateLayer_->show();
    lastShownLayer_ = stateLayer_;
}

void
MCDetailScene::showProps() /* 显示道具选项卡 */
{
    
}

void
MCDetailScene::showTasks() /* 显示任务选项卡 */
{
    
}

void
MCDetailScene::showEquipment() /* 显示装备选项卡 */
{
    
}

void
MCDetailScene::showSkills() /* 显示技能选项卡 */
{
    
}

void
MCDetailScene::showMercenary() /* 显示佣兵选项卡 */
{
    
}

void
MCDetailScene::showQuitWindow() /* 显示退出窗口 */
{
    exit(0);
}

void
MCDetailScene::backward(CCObject* aSender)
{
    CCDirector::sharedDirector()->popScene();
}

void
MCDetailScene::menuItem_clicked(CCObject* aSender)
{
    CCMenuItemImage *menuItem = (CCMenuItemImage *)aSender;
    
    if (menuItem == lastSelectedMenuItem_) {
        menuItem->selected();
        return;
    }
    if (lastSelectedMenuItem_ != NULL) {
        lastSelectedMenuItem_->unselected();
    }
    menuItem->selected();
    lastSelectedMenuItem_ = menuItem;
    
    if (lastShownLayer_ != NULL) {
        lastShownLayer_->hide();
    }
    switch (menuItem->getTag()) {
        case Tag_State:
            showState();
            break;
        case Tag_Props:
            showProps();
            break;
        case Tag_Equipment:
            showEquipment();
            break;
        case Tag_Tasks:
            showTasks();
            break;
        case Tag_Skills:
            showSkills();
            break;
        case Tag_Mercenary:
            showMercenary();
            break;
        case Tag_Quit:
            showQuitWindow();
            break;
    }
}
