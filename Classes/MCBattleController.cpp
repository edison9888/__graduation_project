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
        
        actionBar_ = MCActionBar::create();
        addChild(actionBar_);
        
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
        selectedItem_ = NULL;
        
        return true;
    }
    
    return false;
}

void
MCBattleController::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    
    /* 队伍选择 */
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
        if (teamLayer_->getSelectedRoles()->count() == teamLayer_->size()) { /* 全部选中了 */
            CCMenuItemLabel *menuItem = (CCMenuItemLabel *)selectAllMenu_->getChildren()->objectAtIndex(0);
            CCLabelTTF *label = (CCLabelTTF *) menuItem->getLabel();
            label->setString("取消");
        }
    }
    
    /* item操作 */
    selectedItem_ = actionBar_->itemForTouch(touch);
    if (selectedItem_) {
        selectedItem_->touchedPoint = touch->getLocation();
        selectedItem_->setOpacity(160);
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
    
#warning 太快移动的话，会失控，用队列记录位置？
    if (selectedItem_) {
        for (CCSetIterator iterator = pTouches->begin();
             iterator != pTouches->end();
             ++iterator) {
            CCTouch *touch = dynamic_cast<CCTouch *>(*iterator);
            if (selectedItem_ == actionBar_->itemForTouch(touch)) {
                CCPoint offset = ccpSub(touch->getLocation(), touch->getPreviousLocation());
                selectedItem_->setPosition(ccpAdd(selectedItem_->getPosition(), offset));
                selectedItem_->touchedPoint = touch->getLocation();
                break;
            }
        }
        teamLayer_->acceptActionBarItem(selectedItem_);
    }
    
    CCLayer::ccTouchesMoved(pTouches, pEvent);
}

void
MCBattleController::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator iterator = pTouches->begin();
         iterator != pTouches->end();
         ++iterator) {
        CCTouch *touch = dynamic_cast<CCTouch *>(*iterator);
        MCRoleBaseInfo *selectedRoleBaseInfo;
        if (selectedItem_ != NULL
            && selectedItem_ == actionBar_->itemForTouch(touch)) {
            if (selectedItem_->getItemPosition().equals(selectedItem_->getPosition())) {
                /* todo: 直接点击使用道具 */
                CCLog("直接点击使用道具");
            } else if ((selectedRoleBaseInfo = teamLayer_->collidesWithActionBarItem(selectedItem_))) {
                /* todo: 检测是否移动到图标上 */
                selectedRoleBaseInfo->setOpacity(255);
            } else {
                /* todo: 删除这段，木有使用成功输出 */
                CCLog("木有使用成功");
            }
        }
    }
    if (selectedItem_) {
        selectedItem_->resetPosition();
        selectedItem_->setOpacity(255);
        selectedItem_ = NULL;
    }
    
    teamLayer_->setMultiSeletionMode(false);
    multiSelection_->setScale(1.f);
    multiSelectionTouch_ = NULL;
    
    CCLayer::ccTouchesEnded(pTouches, pEvent);
}

void
MCBattleController::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    if (selectedItem_) {
        selectedItem_->resetPosition();
        selectedItem_->setOpacity(255);
        selectedItem_ = NULL;
    }
    
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
