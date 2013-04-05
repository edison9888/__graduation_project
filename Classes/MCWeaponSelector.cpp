//
//  MCWeaponSelector.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-3.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCWeaponSelector.h"
#include "MCEquipmentManager.h"

bool
MCWeaponSelector::init()
{
    CCSize winSize;
    
    if (CCLayerColor::initWithColor(ccc4(32, 32, 32, 196))) {
        winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCMenu *menu = CCMenu::create();
        addChild(menu);
        
        MCEquipmentManager *equipmentManager = MCEquipmentManager::sharedEquipmentManager();
        CCArray *weapons = equipmentManager->getWeapons();
        CCObject *obj;
        MCEquipmentItem *equipmentItem;
        CCLabelTTF *buttonCellLabel;
        CCMenuItemLabel *buttonCell;
        CCARRAY_FOREACH(weapons, obj) {
            equipmentItem = dynamic_cast<MCEquipmentItem *>(obj);
            buttonCellLabel = CCLabelTTF::create(equipmentItem->getName()->getCString(), "Marker Felt", 21);
            buttonCell = CCMenuItemLabel::create(buttonCellLabel,
                                                 this,
                                                 menu_selector(MCWeaponSelector::weaponDidFinishSelecting));
            buttonCell->setUserObject(equipmentItem);
            menu->addChild(buttonCell);
        }
        
        menu->alignItemsVertically();
        menu->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        menu->setOpacity(0);
        weaponsMenu_ = menu;
        
        menu = CCMenu::create();
        addChild(menu);
        
        buttonCellLabel = CCLabelTTF::create("取消", "Marker Felt", 24);
        buttonCell = CCMenuItemLabel::create(buttonCellLabel,
                                             this,
                                             menu_selector(MCWeaponSelector::cancelButtonDidClick));
        menu->addChild(buttonCell);
        menu->setOpacity(0);
        menu->setPosition(ccp(winSize.width - buttonCell->getContentSize().width * 2,
                              buttonCell->getContentSize().height * 2));
        cancel_ = menu;
        
        setVisible(false);
        
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, menu->getTouchPriority(), true);
        
        return true;
    }
    
    return false;
}

void
MCWeaponSelector::open()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    setAnchorPoint(ccp(0, 0));
    setPosition(ccp(winSize.width * 2, winSize.height));
    setVisible(true);
    runAction(CCSequence::create(CCMoveTo::create(0.2, ccp(0, 0)),
                                 CCCallFuncN::create(this,
                                                     callfuncN_selector(MCWeaponSelector::showSelector)),
                                 NULL));
}

void
MCWeaponSelector::close()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    runAction(CCSequence::create(CCCallFuncN::create(this,
                                                     callfuncN_selector(MCWeaponSelector::hideSelector)),
                                 CCDelayTime::create(0.3),
                                 CCMoveTo::create(0.2, ccp(0, winSize.height)),
                                 CCCallFunc::create(this, callfunc_selector(MCWeaponSelector::destroy)),
                                 NULL));
}

bool
MCWeaponSelector::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    /* 截取触摸事件 */
    return true;
}

MCWeaponSelector *
MCWeaponSelector::weaponSelector(CCNode *aParent, MCWeaponSelectorDelegate *aDelegate)
{
    MCWeaponSelector *weaponSelector = MCWeaponSelector::create();
    
    weaponSelector->setDelegate(aDelegate);
    aParent->addChild(weaponSelector);
    weaponSelector->open();
    
    return weaponSelector;
}

void
MCWeaponSelector::showSelector(CCNode* pSender)
{
    weaponsMenu_->runAction(CCFadeIn::create(0.2));
    cancel_->runAction(CCFadeIn::create(0.2));
}

void
MCWeaponSelector::hideSelector(CCNode* pSender)
{
    weaponsMenu_->runAction(CCFadeOut::create(0.2));
    cancel_->runAction(CCFadeOut::create(0.2));
}

void
MCWeaponSelector::destroy()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    removeFromParentAndCleanup(true);
}

void
MCWeaponSelector::weaponDidFinishSelecting(CCObject *obj)
{
    if (delegate_) {
        MCEquipmentItem *weapon = dynamic_cast<MCEquipmentItem *>(dynamic_cast<CCMenuItemLabel *>(obj)->getUserObject());
        delegate_->weaponSelectorDidFinishSelecting(this, weapon);
        close();
    }
}

void
MCWeaponSelector::cancelButtonDidClick(CCObject *obj)
{
    if (delegate_) {
        delegate_->weaponSelectorDidCancelSelecting(this);
        close();
    }
}