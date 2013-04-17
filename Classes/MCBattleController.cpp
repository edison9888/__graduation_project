//
//  MCBattleController.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCTask.h"
#include "MCTaskContext.h"
#include "MCTaskManager.h"
#include "MCBattleController.h"

const char *kMCMultiSelectionFilepath = "UI/bc_multi_selection.png";

static bool
MCActionBarItemIsPotion(MCActionBarItem *anActionBarItem)
{
    MCTask *task = MCTaskManager::sharedTaskManager()->getCurrentTask();
    CCAssert(task != NULL, "代码逻辑错误");
    MCTaskContext *taskContext = task->getTaskContext();
    MCBackpackItem *backpackItem = anActionBarItem->getBackpackItem();
    
    return backpackItem == taskContext->getHealthPotion()
            || backpackItem == taskContext->getPhysicalPotion();
}

bool
MCBattleController::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        teamLayer_ = MCTeamLayer::create();
        addChild(teamLayer_);
        
        CCSizeLog(teamLayer_->getFrameSize());
        
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
        
        lastTouchedTime_.tv_sec = 0;
        lastTouchedTime_.tv_usec = 0;
        multiSelectionTouch_ = NULL;
        selectedItem_ = NULL;
        
        return true;
    }
    
    return false;
}

CCArray *
MCBattleController::getSelectedRoles()
{
    return teamLayer_->getSelectedRoles();
}

void
MCBattleController::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    
    MCRoleBaseInfo *info = teamLayer_->roleBaseInfoForTouch(touch);
    if (info) {
        info->setTouched(true);
    }
    
    /* item操作 */
    selectedItem_ = actionBar_->itemForTouch(touch);
    if (selectedItem_ && selectedItem_->getBackpackItem()->count > 0) {
        selectedItem_->touchedPoint = touch->getLocation();
        selectedItem_->setOpacity(kMCDraggingActionBarItemOpacity);
    }
    
    /* tag: #multi #selection */
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
            delegate_->controllerDidEnterMultiSelectionMode(this);
        }

    }
    
    CCLayer::ccTouchesBegan(pTouches, pEvent);
}

void
MCBattleController::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    /* tags: #multi #selection #enter */
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
    
    if (selectedItem_) {
        for (CCSetIterator iterator = pTouches->begin();
             iterator != pTouches->end();
             ++iterator) {
            CCTouch *touch = dynamic_cast<CCTouch *>(*iterator);
            if (selectedItem_->touchedPoint.equals(touch->getPreviousLocation())) {
                CCPoint offset = ccpSub(touch->getLocation(), touch->getPreviousLocation());
                selectedItem_->setPosition(ccpAdd(selectedItem_->getPosition(), offset));
                selectedItem_->touchedPoint = touch->getLocation();
                break;
            }
        }
        teamLayer_->acceptActionBarItem(selectedItem_);
    } else if (teamLayer_->getSelectedRoles()->count() == 0) { /* 拖动地图模式 */
        CCTouch *touch = (CCTouch *) pTouches->anyObject();
        CCPoint offset = ccpSub(touch->getLocation(), touch->getPreviousLocation());
        delegate_->controllerDidDragMap(this, offset);
    }
    
    CCLayer::ccTouchesMoved(pTouches, pEvent);
}

void
MCBattleController::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    bool findPath = true;
    
    struct cc_timeval touchedTime;
    CCTime::gettimeofdayCocos2d(&touchedTime, NULL);
    
    /* 队伍选择 */
    /* tags: #select,#team */
    /* 镜头将移到选中对象身上，若有多个选中对象，则移到队伍第一个角色(在选中的人中序号第一)身上 */
    MCRoleBaseInfo *roleBaseInfo = teamLayer_->roleBaseInfoForTouch(touch);
    if (roleBaseInfo && roleBaseInfo->getTouched()) {
        bool isSelected = roleBaseInfo->isSelected();
        MCRole *role = roleBaseInfo->getRole();
        double elapsed = 0;
        
        findPath = false;
        if (! teamLayer_->isMultiSeletionMode()) {
            teamLayer_->unselectAll();
        }
        if (lastTouchedTime_.tv_sec != 0) {
            elapsed = CCTime::timersubCocos2d(&lastTouchedTime_, &touchedTime);
        }
        
        /* 两次点击小于0.5秒则为聚焦人物，并且选中之 */
        if (elapsed < 500) {
            lastTouchedTime_.tv_sec = 0;
            teamLayer_->selectRole(role);
            delegate_->controllerDidSelectRole(this, role);
            delegate_->controllerDidFocus(this, role);
        } else if (isSelected) {
            teamLayer_->unselectRole(role);
            delegate_->controllerDidUnselectRole(this, role);
        } else {
            teamLayer_->selectRole(role);
            delegate_->controllerDidSelectRole(this, role);
        }
        if (teamLayer_->getSelectedRoles()->count() == teamLayer_->size()) { /* 全部选中了 */
            CCMenuItemLabel *menuItem = (CCMenuItemLabel *)selectAllMenu_->getChildren()->objectAtIndex(0);
            CCLabelTTF *label = (CCLabelTTF *) menuItem->getLabel();
            label->setString("取消");
        }
        roleBaseInfo->setTouched(false);
    }
    lastTouchedTime_ = touchedTime;
    
    /* 行动 */
    /* 行走 */
    CCArray *selectedRoles = teamLayer_->getSelectedRoles();
    CCObject *obj;
    if (findPath && selectedRoles->count() > 0) {
        CCARRAY_FOREACH(selectedRoles, obj) {
            MCRole *role = dynamic_cast<MCRole *>(obj);
            role->getEntity()->findPath(touch->getLocation());
        }
    }
    
    /* tags: #item,#use */
    for (CCSetIterator iterator = pTouches->begin();
         iterator != pTouches->end();
         ++iterator) {
        CCTouch *touch = dynamic_cast<CCTouch *>(*iterator);
        MCRoleBaseInfo *selectedRoleBaseInfo;
        if (selectedItem_ != NULL
            && selectedItem_ == actionBar_->itemForTouch(touch)) {
            if (selectedItem_->getItemPosition().equals(selectedItem_->getPosition())) {
                /* 直接点击使用道具 */
                if (MCActionBarItemIsPotion(selectedItem_)) { /* 是药品才能使用 */
                    teamLayer_->selectedRolesUseActionBarItem(selectedItem_);
                    selectedItem_->updateCount();
                }
            } else if ((selectedRoleBaseInfo = teamLayer_->collidesWithActionBarItem(selectedItem_))) {
                /* 拖动使用道具 */
                selectedRoleBaseInfo->setOpacity(255);
                selectedRoleBaseInfo->useActionBarItem(selectedItem_);
                selectedItem_->updateCount();
            }
        }
    }
    if (selectedItem_) {
        selectedItem_->resetPosition();
        if (selectedItem_->getBackpackItem()->count > 0) {
            selectedItem_->setOpacity(kMCNormalActionBarItemOpacity);
        } else {
            selectedItem_->setOpacity(kMCInvalidActionBarItemOpacity);
        }
        selectedItem_ = NULL;
    }
    
    /* todo: 木有测试 */
    /* tags: #multi #selection #exit */
    /* 退出多选模式检测 */
    CCPoint origin = multiSelection_->getPosition();
    CCSize size = multiSelection_->getContentSize();
    CCRect r = CCRectMake(origin.x - size.width / 2, origin.y - size.height / 2, size.width, size.height);
    for (CCSetIterator iterator = pTouches->begin();
         iterator != pTouches->end();
         ++iterator) {
        CCTouch *touch = dynamic_cast<CCTouch *>(*iterator);
        if (r.containsPoint(touch->getLocation())) {
            teamLayer_->setMultiSeletionMode(false);
            multiSelection_->setScale(1.f);
            multiSelectionTouch_ = NULL;
            delegate_->controllerDidExitMultiSelectionMode(this);
        }
        
    }
    
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
        /* tags: #select,#all */
        /* 全选的话，镜头将移到主角身上 */
        /* 已经选择全部了，则全部取消选择 */
        CCARRAY_FOREACH(roles, obj) {
            delegate_->controllerDidUnselectRole(this, dynamic_cast<MCRole *>(obj));
        }
        teamLayer_->unselectAll();
        label->setString("全选");
        delegate_->controllerDidUnselectAll(this, MCTeam::sharedTeam());
    } else {
        CCARRAY_FOREACH(roles, obj) {
            delegate_->controllerDidSelectRole(this, dynamic_cast<MCRole *>(obj));
        }
        teamLayer_->selectAll();
        label->setString("取消");
        delegate_->controllerDidSelectAll(this, MCTeam::sharedTeam());
    }
}
