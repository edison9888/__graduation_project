//
//  MCActionMenu.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-13.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCActionMenu.h"
#include "MCGameScene.h"

const int kMCTagMagicNumber = 0x1234;

MCActionMenu::MCActionMenu()
{
    actionGroups_ = CCDictionary::create();
    actionGroups_->retain();
    actionItemHolder_ = CCDictionary::create();
    actionItemHolder_->retain();
    activeActionGroupRadius_ = 0;
    
    gameScene_ = NULL;
}

MCActionMenu::~MCActionMenu()
{
    CC_SAFE_RELEASE(actionItem_);
    CC_SAFE_RELEASE(actionItemHolder_);
    CC_SAFE_RELEASE(actionGroups_);
}

bool
MCActionMenu::init(MCActionMenuType anActionMenuType)
{
    if (CCMenu::init()) {
        actionItem_ = new CCMenuItemImage;
        actionItem_->initWithNormalImage("action-menu.png",
                                         "action-menu-pressed.png",
                                         NULL,
                                         this,
                                         menu_selector(MCActionMenu::openMenu));
        addChild(actionItem_);
        actionMenuType_ = anActionMenuType;
        
        closeItem_ = new CCMenuItemLabel;
        CCLabelTTF *label = CCLabelTTF::create("关闭", "Marker Felt", 24);
        label->setColor(ccc3(64, 128, 240));
        closeItem_->initWithLabel(label, this, menu_selector(MCActionMenu::closeMenu));
        
        return true;
    }
    
    return false;
}

MCActionMenu *
MCActionMenu::create(MCActionMenuType anActionMenuType)
{
    MCActionMenu *actionMenu = new MCActionMenu;
    
    if (actionMenu && actionMenu->init(anActionMenuType)) {
        actionMenu->autorelease();
    } else {
        CC_SAFE_DELETE(actionMenu);
        actionMenu = NULL;
    }
    
    return actionMenu;
}

/**
 * return y
 */
inline static float
circleFormula(float x, float radius)
{
//    CCLog("x: %f, r: %f", x, radius);
//    CCLog("x^2: %f, r^2: %f", x*x, radius*radius);
//    CCLog("y: %f, y^2: %f", sqrtf(radius * radius - x * x), radius * radius - x * x);
    float xMaP = powf(x, 2.f);
    CCLog("xmap: %f, rp: %f", xMaP, radius * radius);
    float yMbP = sqrtf(radius * radius - xMaP * xMaP);
    CCLog("ymbp: %f", yMbP);
    return yMbP;
}

inline static float
xAtCircleForRadius(float centerX, float radius, float radian)
{
    return centerX + cosf(radian) * radius;
}

inline static float
yAtCircleForRadius(float centerY, float radius, float radian)
{
    return centerY + sinf(radian) * radius;
}

void
MCActionMenu::showActions()
{
    CCObject *obj;
    MCMenuItemAction *menuItemAction;
    CCMenuItemLabel *menuItem;
    CCPoint targetPosition;
    CCPoint centerPoint = this->getPosition();
    CCSize activeMenuItemSize = actionItem_->getContentSize();
    CCSize winSize = CCDirectorGetWindowsSize();
    float minGrowthRotationValue;
    float growthRotationValue; /* 以0点为起始点，默认逆时针弹出 */
    float rotationValue;
    float activeMenuItemRadius = sqrtf(powf(activeMenuItemSize.width, 2) + powf(activeMenuItemSize.height, 2)) / 2;
    float circleRadius = activeMenuItemRadius + activeActionGroupRadius_; /* 环形菜单半径 */
    
    /**
     * cos(C) = (a^2+b^2-c^2) / (2ab)
     * C = acos((a^2+b^2-c^2) / (2ab))
     */
    minGrowthRotationValue = acosf((powf(circleRadius, 2) * 2
                                        - powf((activeActionGroupRadius_ * 2), 2))
                                    / (2 * circleRadius * circleRadius));
    /* 半圆不够空间容纳 */
    if (minGrowthRotationValue * (activeActionGroup_->count() + 1) > M_PI) {
        minGrowthRotationValue = M_PI / (activeActionGroup_->count() + 1);
    }
    growthRotationValue = minGrowthRotationValue;
    rotationValue = CC_DEGREES_TO_RADIANS(90);
    targetPosition.x = cosf(rotationValue) * circleRadius;
    targetPosition.y = sinf(rotationValue) * circleRadius;
    CCARRAY_FOREACH(activeActionGroup_, obj) {
        menuItemAction = (MCMenuItemAction *) obj;
        menuItem = (CCMenuItemLabel *) getChildByTag(menuItemAction->id_ + kMCTagMagicNumber);
        menuItem->runAction(CCSequence::create(CCShow::create(),
                                               CCMoveTo::create(0.1, targetPosition),
                                               NULL));
        rotationValue += growthRotationValue;
        targetPosition.x = cosf(rotationValue) * circleRadius;
        targetPosition.y = sinf(rotationValue) * circleRadius;
    }
    closeItem_->runAction(CCSequence::create(CCShow::create(),
                                             CCMoveTo::create(0.1, targetPosition),
                                             NULL));
}

void
MCActionMenu::hideActions()
{
    CCObject *obj;
    MCMenuItemAction *menuItemAction;
    CCMenuItemLabel *menuItem;
    CCPoint centerPoint = actionItem_->getPosition();
    CCARRAY_FOREACH(activeActionGroup_, obj) {
        menuItemAction = (MCMenuItemAction *) obj;
        menuItem = (CCMenuItemLabel *) getChildByTag(menuItemAction->id_ + kMCTagMagicNumber);
        menuItem->runAction(CCSequence::create(CCMoveTo::create(0.1, centerPoint),
                                               CCHide::create(),
                                               NULL));
    }
    closeItem_->runAction(CCSequence::create(CCMoveTo::create(0.1, centerPoint),
                                             CCHide::create(),
                                             NULL));
    if (actionMenuType_ == MCFloatMenu) {
        actionItem_->setVisible(false);
    }
}

void
MCActionMenu::onEnter()
{
    CCMenu::onEnter();
}

void
MCActionMenu::onExit()
{
    CCMenu::onExit();
}

/**
 * 依附在场景中
 */
void
MCActionMenu::attach(MCGameScene *aGameScene)
{
    CCAssert(aGameScene != NULL, "aGameScene cannot be NULL!");
    if (gameScene_ == NULL) {
        aGameScene->addChild(this);
        if (actionMenuType_ == MCFloatMenu) {
            actionItem_->setVisible(false);
        }
        CCSize winSize = CCDirectorGetWindowsSize();
        CCSize menuItemSize = actionItem_->getContentSize();
        setPosition(ccp(winSize.width - menuItemSize.width * 3 / 4,
                        winSize.height / 2));
        gameScene_ = aGameScene;
    }
}

/**
 * 从被依附的场景中脱离
 */
void
MCActionMenu::detach()
{
    if (gameScene_) {
        gameScene_->removeChild(this);
    }
}


void
MCActionMenu::addActionGroup(mc_dict_key_t aGroupId, CCArray *actions)
{
    actionGroups_->setObject(actions, aGroupId);
}

void
MCActionMenu::removeActionGroup(mc_dict_key_t aGroupId)
{
    actionGroups_->removeObjectForKey(aGroupId);
}

void
MCActionMenu::setActiveActionGroup(mc_dict_key_t aGroupId)
{
    CCObject *obj;
    MCMenuItemAction *menuItemAction;
    CCLabelTTF *label;
    CCSize labelSize;
    float labelRadius;
    CCMenuItemLabel *actionItem;
    
    activeActionGroup_ = (CCArray *) actionGroups_->objectForKey(aGroupId);
    /* reload action items */
    removeAllChildren();
    addChild(actionItem_);
    actionItem_->release();
    /* load new action items */
    CCARRAY_FOREACH(activeActionGroup_, obj) {
        menuItemAction = (MCMenuItemAction *) obj;
        actionItem = (CCMenuItemLabel *) actionItemHolder_->objectForKey(menuItemAction->id_);
        if (actionItem == NULL) {
            label = CCLabelTTF::create(menuItemAction->label_->getCString(), "Marker Felt", 24);
            label->setColor(ccc3(64, 128, 240));
            labelSize = label->getContentSize();
            labelRadius = sqrtf(powf(labelSize.width, 2) + powf(labelSize.height, 2)) / 2;
            if (labelRadius > activeActionGroupRadius_) {
                activeActionGroupRadius_ = labelRadius;
            }
            actionItem = CCMenuItemLabel::create(label,
                                                 menuItemAction->target_,
                                                 menuItemAction->menuHandler_);
            actionItem->setTag(menuItemAction->id_ + kMCTagMagicNumber);
            actionItem->setVisible(false);
        }
        addChild(actionItem);
    }
    closeItem_->setVisible(false);
    addChild(closeItem_);
    closeItem_->release();
}

void
MCActionMenu::openMenu(CCObject *aMenuItem)
{
    if (!closeItem_->isVisible()) {
         showActions();
    }
}

void
MCActionMenu::closeMenu(CCObject *aMenuItem)
{
    if (closeItem_->isVisible()) {
        hideActions();
    }
}
