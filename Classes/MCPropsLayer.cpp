//
//  MCPropsLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCPropsLayer.h"
#include "MCHero.h"
#include "MCBackpack.h"
#include "MCEquipmentManager.h"
#include "MCItemManager.h"

#include <cocos-ext.h>
USING_NS_CC_EXT;

#define MCValueOffsetX 13

static const ccColor3B kMCSelectedColor = ccc3(240, 240, 64);
static const ccColor3B kMCUnselectedColor = ccc3(224, 224, 224);
static const CCRect kMCDetailBackgroundSourceRect = CCRectMake(0, 0, 400, 400);
static const CCRect kMCDetailBackgroundInsetRect = CCRectMake(13, 13, 387, 387);

bool
MCPropsLayer::init()
{
    if (MCBaseInfoLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCLabelTTF *label;
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        float fontSize = 21 / contentScaleFactor;
        float valueFontSize = 24 / contentScaleFactor;
        float contentHeight = winSize.height - 90;
        float contentHeightWithoutBottom = contentHeight * 2 / 3;
        float offsetX = 180 / contentScaleFactor;
        float offsetY = contentHeightWithoutBottom / 4 + 4;
        float itemHeight = 0;
        MCBackpack *backpack = MCBackpack::sharedBackpack();
        MCBackpackItem *backpackItem;
        CCMenuItemLabel *menuItem;
        CCPoint menuItemAnchorPoint = ccp(0, 0.5);
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        
        CCMenu *items = CCMenu::create();
        addChild(items);
        
        /**
         * item的userdata设置为对应的MCBackpackItem
         * item的userobject设置为对应的MCBackpackItem的图标的SpriteFrame
         */
        /* health potion */
        backpackItem = backpack->getHealthPotion();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCHealthPotionIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        firstItem_ = menuItem;
        
        /* physical potion */
        backpackItem = backpack->getPhysicalPotion();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCPhysicalPotionIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* fireball trap wide */
        backpackItem = backpack->getFireballTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFireballWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* fireball trap damage */
        backpackItem = backpack->getFireballTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFireballDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* curse trap wide */
        backpackItem = backpack->getCurseTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCCurseWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* curse trap damage */
        backpackItem = backpack->getCurseTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCCurseDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* paralysis trap wide */
        backpackItem = backpack->getParalysisTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCParalysisWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* paralysis trap damage */
        backpackItem = backpack->getParalysisTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCParalysisDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* fog trap wide */
        backpackItem = backpack->getFogTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFogWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* fog trap damage */
        backpackItem = backpack->getFogTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFogDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* flash trap wide */
        backpackItem = backpack->getFlashTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFlashWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* flash trap damage */
        backpackItem = backpack->getFlashTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "Marker Felt", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCPropsLayer::itemDidClick));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFlashDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        items->setAnchorPoint(ccp(0, 1));
        items->setPosition(ccp(offsetX,  winSize.height - offsetY - itemHeight / 2));
        items->alignItemsVertically();
        items_ = items;
        
        CCPoint detailOrigin = ccp(winSize.width - offsetY / 2,
                                   winSize.height - offsetY);
        CCSize detailSize = CCSizeMake(winSize.width / 2,
                                       itemHeight);
        
        /* 物品信息 */
        detailOrigin = ccp(detailOrigin.x - detailSize.width + 24 / contentScaleFactor,
                           detailOrigin.y - 24 / contentScaleFactor);
        float offsetYInc = 0;
        float valuePositionX;
        
        /* icon */
        icon_ = CCSprite::create(); /* 64x64 */
        addChild(icon_);
        icon_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        icon_->setPosition(ccp(detailOrigin.x, detailOrigin.y - offsetYInc));
        offsetYInc += 72 / contentScaleFactor; /* 图标的高加上偏移值8 */
        
        /* name */
        label = CCLabelTTF::create("物品名称", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(detailOrigin.x, detailOrigin.y - offsetYInc));
        offsetYInc += label->getContentSize().height;
        valuePositionX = detailOrigin.x + label->getContentSize().width * 8 / 3;
        
        name_ = CCLabelTTF::create("物品名称", "Marker Felt", valueFontSize);
        addChild(name_);
        name_->setAnchorPoint(ccp(0.5, 1));
        name_->setPosition(ccp(valuePositionX,
                               label->getPositionY() - (label->getContentSize().height - name_->getContentSize().height) / 2));
        
        /* count */
        label = CCLabelTTF::create("物品数量", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(detailOrigin.x, detailOrigin.y - offsetYInc));
        offsetYInc += label->getContentSize().height;
        valuePositionX = detailOrigin.x + label->getContentSize().width * 8 / 3;
        
        count_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        addChild(count_);
        count_->setAnchorPoint(ccp(0.5, 1));
        count_->setPosition(ccp(valuePositionX,
                               label->getPositionY() - (label->getContentSize().height - count_->getContentSize().height) / 2));
        
        /* description */
        label = CCLabelTTF::create("物品描述", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(detailOrigin.x, detailOrigin.y - offsetYInc));
        offsetYInc += label->getContentSize().height;
        
        description_ = CCLabelTTF::create("物品描述", "Marker Felt", valueFontSize);
        addChild(description_);
        description_->setDimensions(CCSizeMake(detailSize.width - label->getContentSize().width - 64 / contentScaleFactor,
                                               0));
        description_->setHorizontalAlignment(kCCTextAlignmentLeft);
        description_->setAnchorPoint(ccp(0.5, 1));
        description_->setPosition(ccp(valuePositionX,
                               label->getPositionY() - (label->getContentSize().height - description_->getContentSize().height) / 2));
        
        lastClickedItem_ = NULL;
        
        return true;
    }
    
    return false;
}

void
MCPropsLayer::clickFirstItem()
{
    itemDidClick(firstItem_);
}

void
MCPropsLayer::loadData()
{
    MCBackpackItem *backpackItem;
    MCEffectiveItem *item;
    CCSpriteFrame *frame;
    if (lastClickedItem_) {
        backpackItem = (MCBackpackItem *) lastClickedItem_->getUserData();
        frame = dynamic_cast<CCSpriteFrame *>(lastClickedItem_->getUserObject());
        item = dynamic_cast<MCEffectiveItem *>(backpackItem->item);
        /* icon */
        icon_->setDisplayFrame(frame);
        
        /* name */
        name_->setString(item->getName()->getCString());
        
        /* count */
        count_->setString(CCString::createWithFormat("%hi", backpackItem->count)->getCString());
        
        /* description */
        description_->setString(item->getDescription()->getCString());
    }
}

void
MCPropsLayer::itemDidClick(CCObject *obj)
{
    CCMenuItemLabel *menuItem = dynamic_cast<CCMenuItemLabel *>(obj);
    CCLabelTTF *label;
    
    /* 更改选中颜色 */
    if (lastClickedItem_) {
        label = dynamic_cast<CCLabelTTF *>(lastClickedItem_->getLabel());
        label->setColor(kMCUnselectedColor);
    }
    label = dynamic_cast<CCLabelTTF *>(menuItem->getLabel());
    label->setColor(kMCSelectedColor);
    lastClickedItem_ = menuItem;
    
    /* 更改显示内容 */
    loadData();
}
