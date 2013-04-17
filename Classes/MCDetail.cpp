//
//  MCDetail.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-29.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCDetail.h"
#include "MCStateLayer.h"
#include "MCPropsLayer.h"
#include "MCTaskLayer.h"
#include "MCEquipmentLayer.h"
#include "MCGameState.h"

#include <cocos-ext.h>
USING_NS_CC_EXT;

enum __detail_scene_Tags {
    kMCTagState,
    kMCTagProps,
    kMCTagEquipment,
    kMCTagTask,
    kMCTagSkills,
    kMCTagMercenary,
    kMCTagQuit,
};

const char *kMCDetailDidHideNotification = "kMCDetailDidHideNotification";
static const float kMCActionDuration = 0.1f;

static const char *kMCBackgroundFilepath = "bg.png";

class __MCViewSelectorLayer : public CCLayer {
public:
    bool initWithTarget(CCObject *target);
    
    inline CCMenuItem *itemWithTag(int aTag) {
        return (CCMenuItem *) viewsMenu_->getChildByTag(aTag);
    }
   
    CC_SYNTHESIZE_READONLY(CCMenuItem *, defaultMenuItem_, DefaultMenuItem);
    
private:
    CCMenu *viewsMenu_;
};

bool
__MCViewSelectorLayer::initWithTarget(CCObject *target)
{
    if (CCLayer::init()) {
        CCMenuItem *menuItem;
        CCMenu *menu;
        float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
        
        CCSize winSize = CCDirectorGetWindowsSize();
        CCMenuItemImage *backwardMenuItem = CCMenuItemImage::create("back.png", "back_pressed.png");
        backwardMenuItem->setTarget(target, menu_selector(MCDetail::backward));
        menu = CCMenu::create();
        addChild(menu);
        menu->addChild(backwardMenuItem);
        menu->setPosition(ccp(backwardMenuItem->getContentSize().width / 2 - 16 / contentScaleFactor,
                              winSize.height - backwardMenuItem->getContentSize().height / 2 + 16 / contentScaleFactor));
        
        menu = CCMenu::create();
        addChild(menu);
        
        menuItem = CCMenuItemImage::create("user_info.png", "user_info_selected.png");
        menu->addChild(menuItem);
        defaultMenuItem_ = menuItem;
        menuItem->setTag(kMCTagState);
        menuItem->setTarget(target, menu_selector(MCDetail::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("props.png", "props_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(kMCTagProps);
        menuItem->setTarget(target, menu_selector(MCDetail::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("equipment.png", "equipment_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(kMCTagEquipment);
        menuItem->setTarget(target, menu_selector(MCDetail::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("tasks.png", "tasks_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(kMCTagTask);
        menuItem->setTarget(target, menu_selector(MCDetail::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("skills.png", "skills_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(kMCTagSkills);
        menuItem->setTarget(target, menu_selector(MCDetail::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("mercenary.png", "mercenary_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(kMCTagMercenary);
        menuItem->setTarget(target, menu_selector(MCDetail::menuItem_clicked));
        
        menuItem = CCMenuItemImage::create("quit.png", "quit_selected.png");
        menu->addChild(menuItem);
        menuItem->setTag(kMCTagQuit);
        menuItem->setTarget(target, menu_selector(MCDetail::menuItem_clicked));
        
        menu->alignItemsVerticallyWithPadding(0);
        menu->setPosition(ccp(menuItem->getContentSize().width / 2, menu->getPosition().y));
        
        viewsMenu_ = menu;
        
        return true;
    }
    
    return false;
}

bool
MCDetail::init()
{
    if (CCLayer::init()) {
        CCSprite *bg = CCSprite::create(kMCBackgroundFilepath);
        addChild(bg);
        bg->setAnchorPoint(CCPointZero);
        bg->setPosition(CCPointZero);
        
        __MCViewSelectorLayer *viewSelector = new __MCViewSelectorLayer;
        viewSelector->initWithTarget(this);
        viewSelector->autorelease();
        lastSelectedMenuItem_ = NULL;
        addChild(viewSelector);
        viewSelector_ = viewSelector;
        
        stateLayer_ = MCStateLayer::create();
        lastShownLayer_ = stateLayer_;
        addChild(stateLayer_);
        
        propsLayer_ = MCPropsLayer::create();
        addChild(propsLayer_);
        
        taskLayer_ = MCTaskLayer::create();
        addChild(taskLayer_);
        
        equipmentLayer_ = MCEquipmentLayer::create();
        addChild(equipmentLayer_);
        
        return true;
    }
    
    return false;
}

void
MCDetail::initPosition()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    setAnchorPoint(ccp(0, 0));
    setPosition(ccp(0, winSize.height));
}

void
MCDetail::show()
{
    menuItem_clicked(((__MCViewSelectorLayer *) viewSelector_)->getDefaultMenuItem());
    runAction(CCMoveTo::create(kMCActionDuration, CCPointZero));
}

void
MCDetail::hide()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    runAction(CCMoveTo::create(kMCActionDuration, ccp(0, winSize.height)));
}

void
MCDetail::showState() /* 显示状态选项卡 */
{
    stateLayer_->show();
    lastShownLayer_ = stateLayer_;
}

void
MCDetail::showProps() /* 显示道具选项卡 */
{
    propsLayer_->clickFirstItem();
    propsLayer_->show();
    lastShownLayer_ = propsLayer_;
}

void
MCDetail::showTask() /* 显示任务选项卡 */
{
    taskLayer_->show();
    lastShownLayer_ = taskLayer_;
}

void
MCDetail::showEquipment() /* 显示装备选项卡 */
{
    equipmentLayer_->show();
    lastShownLayer_ = equipmentLayer_;
}

void
MCDetail::showSkills() /* 显示技能选项卡 */
{
}

void
MCDetail::showMercenary() /* 显示佣兵选项卡 */
{
}

void
MCDetail::showQuitWindow() /* 显示退出窗口 */
{
    MCConfirm::confirm(this, this, "确定退出？");
}

void
MCDetail::confirmDidClickYesButton(MCConfirm *aConfirm)
{
    MCGameState::sharedGameState()->save();
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    exit(0);
#endif
}

void
MCDetail::backward(CCObject* aSender)
{
    hide();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCDetailDidHideNotification);
}

void
MCDetail::menuItem_clicked(CCObject* aSender)
{
    CCMenuItemImage *menuItem = (CCMenuItemImage *)aSender;
    int tag = menuItem->getTag();
    
    if (tag != kMCTagQuit) {
        menuItem->selected(); /* 点击过后menu那边就把menuitem设置为unselected */
        if (menuItem == lastSelectedMenuItem_) {
            return;
        }
        if (lastSelectedMenuItem_ != NULL) {
            lastSelectedMenuItem_->unselected();
        }
        lastSelectedMenuItem_ = menuItem;
        
        if (lastShownLayer_ != NULL) {
            lastShownLayer_->hide();
        }
    }
    
    switch (tag) {
        case kMCTagState:
            showState();
            break;
        case kMCTagProps:
            showProps();
            break;
        case kMCTagEquipment:
            showEquipment();
            break;
        case kMCTagTask:
            showTask();
            break;
        case kMCTagSkills:
            showSkills();
            break;
        case kMCTagMercenary:
            showMercenary();
            break;
        case kMCTagQuit:
            showQuitWindow();
            break;
    }
}
