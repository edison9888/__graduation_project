//
//  MCBattleControllerLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCBattleControllerLayer.h"
#include "MCBattleController.h"
#include "MCScene.h"

MCBattleControllerLayer::MCBattleControllerLayer()
{
    
}

MCBattleControllerLayer::~MCBattleControllerLayer()
{
    
}

bool
MCBattleControllerLayer::init()
{
    if (MCJoypadControllerLayer::init()) {
        controller_ = MCBattleController::create();
        addChild(controller_);
        
        CCMenuItem *menuItem = dynamic_cast<CCMenuItem *>(actionButton_->getChildren()->objectAtIndex(0));
        menuItem->setTarget(this,
                            menu_selector(MCBattleControllerLayer::activate));
        
        return true;
    }
    
    return false;
}

MCBattleControllerDelegate *
MCBattleControllerLayer::getDelegate()
{
    return controller_->getDelegate();
}

void
MCBattleControllerLayer::setDelegate(MCBattleControllerDelegate* aDelegate)
{
    if (aDelegate) {
        controller_->setVisible(true);
        controller_->setTouchEnabled(true);
    } else {
        controller_->setVisible(false);
        controller_->setTouchEnabled(false);
    }
    aDelegate->controllerDidAttach(controller_);
    controller_->setDelegate(aDelegate);
}

void
MCBattleControllerLayer::onEnter()
{
    MCJoypadControllerLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void
MCBattleControllerLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    MCJoypadControllerLayer::onExit();
}

bool
MCBattleControllerLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    /* 在已经选中了人物的情况下需要检查是否命中选中敌人 */
#if MC_SELECT_ALL_SUPPORT == 1
    if (controller_->getSelectedRoles()->count() > 0) {
#else
    if (controller_->getSelectedRole() != NULL) {
#endif
        MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
        CCArray *roles = sceneContext->getObjects();
        CCObject *obj;
        MCRole *role;
        MCRoleEntity *entity;
        CCPoint touchedLocation = pTouch->getLocation();
        
        CCARRAY_FOREACH(roles, obj) {
            role = dynamic_cast<MCRole *>(obj);
            entity = role->getEntity();
            CCPoint local = entity->convertToNodeSpace(touchedLocation);
            CCSize s = entity->getContentSize();
            CCRect r = CCRectMake(0, 0, s.width, s.height);
            if (r.containsPoint(local)) {
                /* 干之！ */
                controller_->selectTarget(role);
                /* 拦截 */
                return true;
            }
        }
    }
    
    return false;
}

bool
MCBattleControllerLayer::isEnabled()
{
    return controller_->isTouchEnabled();
}

void
MCBattleControllerLayer::setEnabled(bool var)
{
    MCJoypadControllerLayer::setEnabled(var);
    controller_->setVisible(var);
    controller_->setTouchEnabled(var);
}

bool
MCBattleControllerLayer::isJoypadEnable()
{
    return joypad_->isTouchEnabled();
}

void
MCBattleControllerLayer::setJoypadEnable(bool var)
{
    joypad_->setVisible(var);
    joypad_->setTouchEnabled(var);
    controller_->setJoypadEnable(var);
}

/**
 * 在这里是切换为Joypad控制的按钮 
 * 仅能在选中一人的情况下使用摇杆
 */
void
MCBattleControllerLayer::activate(CCObject *aSender)
{
#if MC_SELECT_ALL_SUPPORT == 1
    CCArray *selectedRoles = controller_->getSelectedRoles();
    MCRole *selectedRole;
    if (selectedRoles->count() != 1) {
        return;
    }
#else
    MCRole *selectedRole = controller_->getSelectedRole();
    if (! selectedRole) {
        return;
    }
#endif
    if (joypad_->isVisible()) {
        setJoypadEnable(false);
        controller_->setVisible(true);
        controller_->setTouchEnabled(true);
    } else {
        setJoypadEnable(true);
        controller_->setVisible(false);
        controller_->setTouchEnabled(false);
        /* 聚焦人物 */
#if MC_SELECT_ALL_SUPPORT == 1
        selectedRole = dynamic_cast<MCRole *>(selectedRoles->objectAtIndex(0));
#endif
        selectedRole->getEntity()->stopWalking();
        controller_->getDelegate()->controllerDidFocus(controller_, selectedRole);
    }
}
