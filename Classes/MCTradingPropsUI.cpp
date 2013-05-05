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
#include "MCSlider.h"
#include "MCTableViewTextFieldCell.h"

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
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        float fontSize = 21 / contentScaleFactor;
        float valueFontSize = 24 / contentScaleFactor;
        float contentHeight = winSize.height - 90;
        float contentHeightWithoutBottom = contentHeight * 2 / 3;
        float offsetX = 24 / contentScaleFactor;
        float offsetY = contentHeightWithoutBottom / 4 + 4;
        float itemHeight = 0;
        CCLabelTTF *label;
        MCBackpack *backpack = MCBackpack::sharedBackpack();
        MCBackpackItem *backpackItem;
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        CCNode *backpackItemObject;
        
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
        
        
        CCArray *items = CCArray::create();
        items->retain();
        
        /**
         * item的userdata设置为对应的MCBackpackItem
         * item的userobject设置为对应的MCBackpackItem的图标的SpriteFrame
         */
        /* health potion */
        backpackItem = backpack->getHealthPotion();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCHealthPotionIcon));
        items->addObject(backpackItemObject);
        
        /* physical potion */
        backpackItem = backpack->getPhysicalPotion();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCPhysicalPotionIcon));
        items->addObject(backpackItemObject);
        
        /* fireball trap wide */
        backpackItem = backpack->getFireballTrapWide();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCFireballWideIcon));
        items->addObject(backpackItemObject);
        
        /* fireball trap damage */
        backpackItem = backpack->getFireballTrapDamage();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCFireballDamageIcon));
        items->addObject(backpackItemObject);
        
        /* curse trap wide */
        backpackItem = backpack->getCurseTrapWide();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCCurseWideIcon));
        items->addObject(backpackItemObject);
        
        /* curse trap damage */
        backpackItem = backpack->getCurseTrapDamage();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCCurseDamageIcon));
        items->addObject(backpackItemObject);
        
        /* paralysis trap wide */
        backpackItem = backpack->getParalysisTrapWide();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCParalysisWideIcon));
        items->addObject(backpackItemObject);
        
        /* paralysis trap damage */
        backpackItem = backpack->getParalysisTrapDamage();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCParalysisDamageIcon));
        items->addObject(backpackItemObject);
        
        /* fog trap wide */
        backpackItem = backpack->getFogTrapWide();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCFogWideIcon));
        items->addObject(backpackItemObject);
        
        /* fog trap damage */
        backpackItem = backpack->getFogTrapDamage();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCFogDamageIcon));
        items->addObject(backpackItemObject);
        
        /* flash trap wide */
        backpackItem = backpack->getFlashTrapWide();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCFlashWideIcon));
        items->addObject(backpackItemObject);
        
        /* flash trap damage */
        backpackItem = backpack->getFlashTrapDamage();
        backpackItemObject = CCNode::create();
        backpackItemObject->setUserData(backpackItem);
        backpackItemObject->setUserObject(cache->spriteFrameByName(kMCFlashDamageIcon));
        items->addObject(backpackItemObject);
        
        effectiveItems_ = items;
        
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
        
        menu = CCMenu::create();
        info_->addChild(menu);
        
        label = CCLabelTTF::create("购买", "", fontSize);
        backpackItemObject = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::buy_click));
        menu->addChild(backpackItemObject);
        
        label = CCLabelTTF::create("卖出", "", fontSize);
        backpackItemObject = CCMenuItemLabel::create(label, this, menu_selector(MCTradingPropsUI::sell_click));
        menu->addChild(backpackItemObject);
        menu->alignItemsHorizontallyWithPadding(13.0f / contentScaleFactor);
        menu->setPosition(ccp(detailOrigin.x + detailSize.width - backpackItemObject->getContentSize().width,
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
        
        tableViewSize_ = CCSizeMake(180 / contentScaleFactor,
                                    (400 - line->getPositionY()) / contentScaleFactor);
        tableView_ = CCTableView::create(this, tableViewSize_);
        addChild(tableView_);
        tableView_->setDirection(kCCScrollViewDirectionVertical);
        tableView_->setDelegate(this);
        tableView_->setVerticalFillOrder(kCCTableViewFillTopDown);
        tableView_->reloadData();
        tableView_->setPosition(ccp(offsetX,
                                    (winSize.height - tableViewSize_.height) / 2));
        selectedCell_ = NULL;
        
        info_->setVisible(false);
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
    if (!selectedCell_) {
        return;
    }
    CCNode *node = dynamic_cast<CCNode *>(effectiveItems_->objectAtIndex(selectedCell_->getIdx()));
    MCBackpackItem *backpackItem = (MCBackpackItem *) node->getUserData();
    float now = (float) backpackItem->count;
    float min = now < kMCItemMax ? 1.0f : 0.0f;
    float max = (float) kMCItemMax - now;
    MCSlider *slider = MCSlider::defaultSlider(0.0f, 99.0f, 0.0f);
    
    slider->setMinimumAllowedValue(min);
    slider->setValue(now);
    slider->setMaximumAllowedValue(max);
    
    tradingOperation_ = MCBuyOperation;
    slider->setDelegate(this);
    slider->attach(this);
}

void
MCTradingPropsUI::sell_click(CCObject *aSender)
{
    if (!selectedCell_) {
        return;
    }
    CCNode *node = dynamic_cast<CCNode *>(effectiveItems_->objectAtIndex(selectedCell_->getIdx()));
    MCBackpackItem *backpackItem = (MCBackpackItem *) node->getUserData();
    float min = 0.0f;
    float max = (float) backpackItem->count;
    MCSlider *slider = MCSlider::defaultSlider(0.0f, 99.0f, 0.0f);
    
    slider->setMinimumAllowedValue(min);
    slider->setValue(min);
    slider->setMaximumAllowedValue(max);
    
    tradingOperation_ = MCSellOperation;
    slider->setDelegate(this);
    slider->attach(this);
}

void
MCTradingPropsUI::sliderDidOk(MCSlider *aSlider)
{
    if (!selectedCell_) {
        return;
    }
    CCNode *node = dynamic_cast<CCNode *>(effectiveItems_->objectAtIndex(selectedCell_->getIdx()));
    MCBackpackItem *backpackItem = (MCBackpackItem *) node->getUserData();
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    mc_size_t count = aSlider->getValue();
    mc_price_t money = backpack->getMoney();
    mc_price_t price = backpackItem->item->getPrice() * count;
    
    if (tradingOperation_ == MCBuyOperation) {
        if (backpack->spend(price)) {
            backpackItem->count += count;
        } else {
            MCToast::make(this, "兄台，你不够钱~")->show();
        }
    } else if (tradingOperation_ == MCSellOperation) {
        backpackItem->count -= count;
        backpack->setMoney(money + (mc_price_t) (price * kMCSellPercentage));
    }
    loadItem(node);
    money_->setString(CCString::createWithFormat("%d", backpack->getMoney())->getCString());
}

/* CCTableViewDataSource */
CCSize
MCTradingPropsUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(96, 32);
}

CCTableViewCell *
MCTradingPropsUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCNode *node = dynamic_cast<CCNode *>(effectiveItems_->objectAtIndex(idx));
    MCBackpackItem *backpackItem = (MCBackpackItem *) node->getUserData();
    MCEffectiveItem *effectiveItem = dynamic_cast<MCEffectiveItem *>(backpackItem->item);
    
    CCTableViewCell *cell = table->dequeueCell();
    if (! cell) {
        cell = MCTableViewTextFieldCell::create(effectiveItem->getName()->getCString(),
                                                "Helvetica",
                                                18.0f / CC_CONTENT_SCALE_FACTOR());
    } else {
        dynamic_cast<MCTableViewTextFieldCell *>(cell)->setString(effectiveItem->getName()->getCString());
    }
        
    return cell;
}

unsigned int
MCTradingPropsUI::numberOfCellsInTableView(CCTableView *table)
{
    return effectiveItems_ ? effectiveItems_->count() : 0;
}

/* CCTableViewDelegate */
void
MCTradingPropsUI::tableCellTouched(CCTableView *table, CCTableViewCell *cell)
{
    if (selectedCell_) {
        selectedCell_->unselected();
    }
    selectedCell_ = dynamic_cast<MCTableViewTextFieldCell *>(cell);
    selectedCell_->selected();

    loadItem(dynamic_cast<CCNode *>(effectiveItems_->objectAtIndex(cell->getIdx())));
}

void
MCTradingPropsUI::destroy()
{
    MCScene *scene = dynamic_cast<MCScene *>(m_pParent);
    
    scene->resumeScene();
    removeFromParentAndCleanup(true);
}

void
MCTradingPropsUI::loadItem(CCNode *aBackpackItemObject)
{
    MCBackpackItem *backpackItem;
    MCEffectiveItem *item;
    CCSpriteFrame *frame;
    if (aBackpackItemObject) {
        backpackItem = (MCBackpackItem *) aBackpackItemObject->getUserData();
        frame = dynamic_cast<CCSpriteFrame *>(aBackpackItemObject->getUserObject());
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
        info_->setVisible(true);
    } else {
        info_->setVisible(false);
    }
}
