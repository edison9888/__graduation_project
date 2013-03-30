//
//  MCBattleController.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCBattleController.h"

const char *kMCMultiSelectionFilepath = "UI/bc_multi_selection.png";

bool
MCBattleController::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        teamLayer_ = MCTeamLayer::create();
        addChild(teamLayer_);
        
        CCMenu *menu;
        CCMenuItemLabel *menuItem;
        CCLabelTTF *label;
        
        menu = CCMenu::create();
        label = CCLabelTTF::create("全选", "Marker Felt", 24);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCBattleController::didSelectAll));
        menu->addChild(menuItem);
        CCSize menuSize = menuItem->getContentSize();
        menu->setPosition(ccp(menuSize.width, menuSize.height));
        
        addChild(menu);
        selectAllMenu_ = menu;
        
        multiSelection_ = CCSprite::create(kMCMultiSelectionFilepath);
        multiSelection_->setPosition(ccp(winSize.width - 72, 72));
        addChild(multiSelection_);
        
        multiSelectionTouch_ = NULL;
        
        return true;
    }
    
    return false;
}

void
MCBattleController::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    
    MCRoleBaseInfo *roleBaseInfo = teamLayer_->roleBaseInfoForTouch(touch);
    if (roleBaseInfo) {
        if (! teamLayer_->isMultiSeletionMode()) {
            teamLayer_->unselectAll();
        }
        if (roleBaseInfo->isSelected()) {
            teamLayer_->unselectRole(roleBaseInfo->getRole());
            delegate_->controllerDidUnselectRole(delegate_, roleBaseInfo->getRole());
        } else {
            teamLayer_->selectRole(roleBaseInfo->getRole());
            delegate_->controllerDidSelectRole(delegate_, roleBaseInfo->getRole());
        }
    }
    /* 检测多选模式 */
    CCPoint origin = multiSelection_->getPosition();
    CCSize size = multiSelection_->getContentSize();
    CCRect r = CCRectMake(origin.x - size.width / 2, origin.y - size.height / 2, size.width, size.height);
    for (CCSetIterator iterator = pTouches->begin();
         iterator != pTouches->end();
         ++iterator) {
        CCTouch *touch = dynamic_cast<CCTouch *>(*iterator);
        if (r.containsPoint(touch->getLocation())) {
            teamLayer_->setMultiSeletionMode(true);
            multiSelection_->setScale(1.2f);
            multiSelectionTouch_ = touch;
        }

    }
    
    CCLayer::ccTouchesBegan(pTouches, pEvent);
}

void
MCBattleController::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    /* 检测多选模式 */
    if (multiSelectionTouch_) {
        CCPoint origin = multiSelection_->getPosition();
        CCSize size = multiSelection_->getContentSize();
        CCRect r = CCRectMake(origin.x - size.width / 2, origin.y - size.height / 2, size.width, size.height);
        if (! r.containsPoint(multiSelectionTouch_->getLocation())) {
            teamLayer_->setMultiSeletionMode(false);
            multiSelection_->setScale(1.0f);
            multiSelectionTouch_ = NULL;
        }
    }
    CCLayer::ccTouchesMoved(pTouches, pEvent);
}

void
MCBattleController::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    teamLayer_->setMultiSeletionMode(false);
    multiSelection_->setScale(1.f);
    multiSelectionTouch_ = NULL;
    CCLayer::ccTouchesEnded(pTouches, pEvent);
}

void
MCBattleController::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    teamLayer_->setMultiSeletionMode(false);
    multiSelection_->setScale(1.f);
    multiSelectionTouch_ = NULL;
    CCLayer::ccTouchesCancelled(pTouches, pEvent);
}

void
MCBattleController::didSelectAll(CCObject *aSender)
{
    CCMenuItemLabel *menuItem = (CCMenuItemLabel *)selectAllMenu_->getChildren()->objectAtIndex(0);
    CCLabelTTF *label = (CCLabelTTF *) menuItem->getLabel();
    CCArray *roles = teamLayer_->getSelectedRoles();
    CCObject *obj;
    if (roles->count() == teamLayer_->size()) {
        /* 已经选择全部了，则全部取消选择 */
        CCARRAY_FOREACH(roles, obj) {
            delegate_->controllerDidUnselectRole(delegate_, dynamic_cast<MCRole *>(obj));
        }
        teamLayer_->unselectAll();
        label->setString("全选");
        delegate_->controllerDidUnselectAll(delegate_, MCTeam::sharedTeam());
    } else {
        CCARRAY_FOREACH(roles, obj) {
            delegate_->controllerDidSelectRole(delegate_, dynamic_cast<MCRole *>(obj));
        }
        teamLayer_->selectAll();
        label->setString("取消");
        delegate_->controllerDidSelectAll(delegate_, MCTeam::sharedTeam());
    }
}
