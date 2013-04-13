//
//  MCTradingPropsUI.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCTradingPropsUI.h"
#include "MCBackpack.h"
#include "MCScene.h"
#include "MCToast.h"
#include "MCRangeInput.h"

#include <cocos-ext.h>
USING_NS_CC_EXT;

#define kMCValueOffsetX 13

const char *kMCTradingPropsUIDidHideNotification = "kMCTradingPropsUIDidHideNotification";

const float kMCSellPercentage = 0.3;

static const float kMCActionDuration = 0.1f;

static const char *kMCBackgroundFilepath = "bg.png";

static const ccColor3B kMCUnselectedColor = ccc3(255, 255, 255);
static const ccColor3B kMCSelectedColor = ccc3(240, 196, 64);

bool
MCTradingPropsUI::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCMenu *menu;
        CCMenuItem *menuItem;
        CCLabelTTF *label;
        float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
        float fontSize = 21 / contentScaleFactor;
        float valueFontSize = 24 / contentScaleFactor;
        float contentHeight = winSize.height - 90;
        float contentHeightWithoutBottom = contentHeight * 2 / 3;
        float offsetX = 24 / contentScaleFactor;
        float offsetY = contentHeightWithoutBottom / 4 + 4;
        float itemHeight = 0;
        MCBackpack *backpack = MCBackpack::sharedBackpack();
        MCBackpackItem *backpackItem;
        CCPoint menuItemAnchorPoint = ccp(0, 0.5);
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        
        CCSprite *bg = CCSprite::create(kMCBackgroundFilepath);
        addChild(bg);
        bg->setAnchorPoint(CCPointZero);
        bg->setPosition(CCPointZero);
        
        CCMenuItemImage *backwardMenuItem = CCMenuItemImage::create("back.png", "back_pressed.png");
        backwardMenuItem->setTarget(this, menu_selector(MCTradingPropsUI::backward));
        menu = CCMenu::create();
        addChild(menu);
        menu->addChild(backwardMenuItem);
        menu->setPosition(ccp(backwardMenuItem->getContentSize().width / 2 - 16 / contentScaleFactor,
                              winSize.height - backwardMenuItem->getContentSize().height / 2 + 16 / contentScaleFactor));
        
        
        CCMenu *items = CCMenu::create();
        addChild(items);
        
        /**
         * item的userdata设置为对应的MCBackpackItem
         * item的userobject设置为对应的MCBackpackItem的图标的SpriteFrame
         */
        /* health potion */
        backpackItem = backpack->getHealthPotion();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCHealthPotionIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* physical potion */
        backpackItem = backpack->getPhysicalPotion();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCPhysicalPotionIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* fireball trap wide */
        backpackItem = backpack->getFireballTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFireballWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* fireball trap damage */
        backpackItem = backpack->getFireballTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFireballDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* curse trap wide */
        backpackItem = backpack->getCurseTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCCurseWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* curse trap damage */
        backpackItem = backpack->getCurseTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCCurseDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* paralysis trap wide */
        backpackItem = backpack->getParalysisTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCParalysisWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* paralysis trap damage */
        backpackItem = backpack->getParalysisTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCParalysisDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* fog trap wide */
        backpackItem = backpack->getFogTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFogWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* fog trap damage */
        backpackItem = backpack->getFogTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFogDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* flash trap wide */
        backpackItem = backpack->getFlashTrapWide();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFlashWideIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        /* flash trap damage */
        backpackItem = backpack->getFlashTrapDamage();
        label = CCLabelTTF::create(backpackItem->item->getName()->getCString(), "", fontSize);
        label->setColor(kMCUnselectedColor);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::item_clicked));
        menuItem->setUserData(backpackItem);
        menuItem->setUserObject(cache->spriteFrameByName(kMCFlashDamageIcon));
        itemHeight += label->getContentSize().height;
        menuItem->setAnchorPoint(menuItemAnchorPoint);
        items->addChild(menuItem);
        
        items->setAnchorPoint(ccp(0, 1));
        items->setPosition(ccp(offsetX,  winSize.height - offsetY - itemHeight / 2));
        items->alignItemsVertically();
        items_ = items;
        
        CCPoint detailOrigin = ccp(winSize.width - offsetY,
                                   winSize.height - offsetY);
        CCSize detailSize = CCSizeMake(winSize.width / 2,
                                       itemHeight);
        
        /* 物品信息 */
        detailOrigin = ccp(detailOrigin.x - detailSize.width + 24 / contentScaleFactor,
                           detailOrigin.y - 24 / contentScaleFactor);
        float offsetYInc = 0;
        float valuePositionX;
        info_ = CCLayer::create();
        addChild(info_);
        
        /* icon */
        icon_ = CCSprite::create(); /* 64x64 */
        info_->addChild(icon_);
        icon_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        icon_->setPosition(ccp(detailOrigin.x, detailOrigin.y - offsetYInc));
        offsetYInc += 72 / contentScaleFactor; /* 图标的高加上偏移值8 */
        
        /* name */
        label = CCLabelTTF::create("物品名称", "", fontSize);
        info_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(detailOrigin.x, detailOrigin.y - offsetYInc));
        offsetYInc += label->getContentSize().height;
        valuePositionX = detailOrigin.x + label->getContentSize().width * 8 / 3;
        
        name_ = CCLabelTTF::create("物品名称", "", valueFontSize);
        info_->addChild(name_);
        name_->setAnchorPoint(ccp(0.5, 1));
        name_->setPosition(ccp(valuePositionX,
                               label->getPositionY() - (label->getContentSize().height - name_->getContentSize().height) / 2));
        
        /* count */
        label = CCLabelTTF::create("持有数量", "", fontSize);
        info_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(detailOrigin.x, detailOrigin.y - offsetYInc));
        offsetYInc += label->getContentSize().height;
        valuePositionX = detailOrigin.x + label->getContentSize().width * 8 / 3;
        
        count_ = CCLabelTTF::create("0", "", valueFontSize);
        info_->addChild(count_);
        count_->setAnchorPoint(ccp(0.5, 1));
        count_->setPosition(ccp(valuePositionX,
                                label->getPositionY() - (label->getContentSize().height - count_->getContentSize().height) / 2));
        
        /* count */
        label = CCLabelTTF::create("物品单价", "", fontSize);
        info_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(detailOrigin.x, detailOrigin.y - offsetYInc));
        offsetYInc += label->getContentSize().height;
        
        price_ = CCLabelTTF::create("0", "", valueFontSize);
        info_->addChild(price_);
        price_->setAnchorPoint(ccp(0.5, 1));
        price_->setPosition(ccp(valuePositionX,
                                label->getPositionY() - (label->getContentSize().height - price_->getContentSize().height) / 2));
        
        /* description */
        label = CCLabelTTF::create("物品描述", "", fontSize);
        info_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(detailOrigin.x, detailOrigin.y - offsetYInc));
        offsetYInc += label->getContentSize().height;
        
        description_ = CCLabelTTF::create("物品描述", "", valueFontSize);
        info_->addChild(description_);
        description_->setDimensions(CCSizeMake(detailSize.width - label->getContentSize().width - 64 / contentScaleFactor,
                                               0));
        description_->setHorizontalAlignment(kCCTextAlignmentLeft);
        description_->setAnchorPoint(ccp(0.5, 1));
        description_->setPosition(ccp(valuePositionX,
                                      label->getPositionY() - (label->getContentSize().height - description_->getContentSize().height) / 2));
        
        menu = CCMenu::create();
        info_->addChild(menu);
        label = CCLabelTTF::create("购买", "", fontSize);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::buy_click));
        menu->addChild(menuItem);
        menu->setPosition(ccp(detailOrigin.x - menuItem->getContentSize().width,
                              winSize.height / 4));
        
        menu = CCMenu::create();
        info_->addChild(menu);
        label = CCLabelTTF::create("卖出", "", fontSize);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::sell_click));
        menu->addChild(menuItem);
        menu->setPosition(ccp(detailOrigin.x + menuItem->getContentSize().width,
                              winSize.height / 4));
        /* money */
        label = CCLabelTTF::create("马克", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(1, 0)); /* 右下角 */
        label->setPosition(ccp(winSize.width - 48 / contentScaleFactor, 48 / contentScaleFactor));
        
        money_ = CCLabelTTF::create(CCString::createWithFormat("%d", backpack->getMoney())->getCString(),
                                    "Marker Felt",
                                    valueFontSize);
        addChild(money_);
        money_->setAnchorPoint(ccp(1, 0)); /* 右下角 */
        money_->setPosition(ccp(label->getPosition().x - label->getContentSize().width - kMCValueOffsetX,
                                48 / contentScaleFactor));
        
        /* line */
        CCSize separatorSize = CCSizeMake(winSize.width - offsetX - 45 / contentScaleFactor, 4);
        CCScale9Sprite *line = CCScale9Sprite::create("UI/separator.png");
        addChild(line);
        line->setContentSize(separatorSize);
        line->setAnchorPoint(ccp(0, 0)); /* 左下角 */
        line->setPosition(ccp(offsetX, 45 / contentScaleFactor));
        
        info_->setVisible(false);
        lastClickedItem_ = NULL;
        tradingOperation_ = MCUnknownOperation;
        
        return true;
    }
    
    return false;
}

void
MCTradingPropsUI::initPosition()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    setAnchorPoint(CCPointZero);
    setPosition(ccp(0, winSize.height));
}

void
MCTradingPropsUI::attach(MCScene *aScene)
{
    aScene->addChild(this);
    aScene->pauseScene();
    initPosition();
    
    runAction(CCMoveTo::create(kMCActionDuration, CCPointZero));
}

void
MCTradingPropsUI::detach()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    runAction(CCSequence::createWithTwoActions(CCMoveTo::create(kMCActionDuration, ccp(0, winSize.height)),
                                               CCCallFunc::create(this, callfunc_selector(MCTradingPropsUI::destroy))));
}

void
MCTradingPropsUI::backward(CCObject *aSender) /* 关闭.... */
{
    detach();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCTradingPropsUIDidHideNotification);
}

void
MCTradingPropsUI::buy_click(CCObject *aSender)
{
    CCMenuItem *menuItem = dynamic_cast<CCMenuItemLabel *>(lastClickedItem_);
    MCBackpackItem *backpackItem = (MCBackpackItem *) menuItem->getUserData();
    mc_size_t now = backpackItem->count;
    mc_size_t min = now < kMCItemMax ? 1 : 0;
    mc_size_t max = kMCItemMax - now;
    MCRangeInput *rangeInpout = MCRangeInput::defaultRangeInput(min, max, min);
    
    tradingOperation_ = MCBuyOperation;
    rangeInpout->setDelegate(this);
    rangeInpout->attach(this);
}

void
MCTradingPropsUI::sell_click(CCObject *aSender)
{
    CCMenuItem *menuItem = dynamic_cast<CCMenuItemLabel *>(lastClickedItem_);
    MCBackpackItem *backpackItem = (MCBackpackItem *) menuItem->getUserData();
    mc_size_t min = 0;
    mc_size_t max = backpackItem->count;
    MCRangeInput *rangeInpout = MCRangeInput::defaultRangeInput(min, max, min);
    
    tradingOperation_ = MCSellOperation;
    rangeInpout->setDelegate(this);
    rangeInpout->attach(this);
}

void
MCTradingPropsUI::rangeInputDidSetValue(MCRangeInput *aRangeInput)
{
    CCMenuItemLabel *menuItem = dynamic_cast<CCMenuItemLabel *>(lastClickedItem_);
    MCBackpackItem *backpackItem = (MCBackpackItem *) menuItem->getUserData();
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    mc_size_t count = aRangeInput->getValue();
    mc_price_t money = backpack->getMoney();
    mc_price_t price = backpackItem->item->getPrice() * count;
    
    if (tradingOperation_ == MCBuyOperation) {
        if (backpack->spend(price)) {
            backpackItem->count += count;
        } else {
            MCToast::make(this, "兄台，你不够钱~")->show();
        }
    } else if (tradingOperation_ == MCSellOperation) {
        backpack->setMoney(money + (mc_price_t) (price * kMCSellPercentage));
    }
    loadItem(menuItem);
    money_->setString(CCString::createWithFormat("%d", backpack->getMoney())->getCString());
}

void
MCTradingPropsUI::item_clicked(CCObject *obj)
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
    info_->setVisible(true);
    loadItem(menuItem);
}

void
MCTradingPropsUI::destroy()
{
    MCScene *scene = dynamic_cast<MCScene *>(m_pParent);
    
    scene->resumeScene();
    removeFromParentAndCleanup(true);
}

void
MCTradingPropsUI::loadItem(CCMenuItemLabel *aMenuItem)
{
    MCBackpackItem *backpackItem;
    MCEffectiveItem *item;
    CCSpriteFrame *frame;
    if (aMenuItem) {
        backpackItem = (MCBackpackItem *) aMenuItem->getUserData();
        frame = dynamic_cast<CCSpriteFrame *>(aMenuItem->getUserObject());
        item = dynamic_cast<MCEffectiveItem *>(backpackItem->item);
        /* icon */
        icon_->setDisplayFrame(frame);
        
        /* name */
        name_->setString(item->getName()->getCString());
        
        /* count */
        count_->setString(CCString::createWithFormat("%hi", backpackItem->count)->getCString());
        
        /* price */
        price_->setString(CCString::createWithFormat("%d", item->getPrice())->getCString());
        
        /* description */
        description_->setString(item->getDescription()->getCString());
    }
}
