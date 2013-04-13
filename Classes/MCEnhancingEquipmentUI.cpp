//
//  MCEnhancingEquipmentUI.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCEnhancingEquipmentUI.h"
#include "MCEquipmentItem.h"
#include "MCBackpack.h"
#include "MCScene.h"
#include "MCToast.h"
#include "MCEquipmentManager.h"
#include "MCTableViewTextFieldCell.h"

#include <cocos-ext.h>
USING_NS_CC_EXT;

#define kMCValueOffsetX 13

const char *kMCEnhancingEquipmentUIDidHideNotification = "kMCEnhancingEquipmentUIDidHideNotification";
static const float kMCActionDuration = 0.1f;

static const char *kMCBackgroundFilepath = "bg.png";

static const int kMCTagWeapon = 0x13;
static const ccColor3B kMCItemColor = ccc3(192, 192, 192);
static const char *kMCDetailBackground = "UI/pl_detail_background.png";
static const CCRect kMCDetailBackgroundSourceRect = CCRectMake(0, 0, 400, 400);
static const CCRect kMCDetailBackgroundInsetRect = CCRectMake(13, 13, 387, 387);

static const ccColor3B kMCUnselectedColor = ccc3(255, 255, 255);
static const ccColor3B kMCSelectedColor = ccc3(240, 196, 64);

bool
MCEnhancingEquipmentUI::init()
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
        float offsetXLeft = 180 / contentScaleFactor;
        float offsetYInc = offsetY / 2;
        float valuePositionXLeft;
        MCEquipmentManager *equipmentManager = MCEquipmentManager::sharedEquipmentManager();
        CCArray *weapons = equipmentManager->getWeapons();
        CCArray *armors = equipmentManager->getArmors();
        MCBackpack *backpack = MCBackpack::sharedBackpack();
        
        CCSprite *bg = CCSprite::create(kMCBackgroundFilepath);
        addChild(bg);
        bg->setAnchorPoint(CCPointZero);
        bg->setPosition(CCPointZero);
        
        CCMenuItemImage *backwardMenuItem = CCMenuItemImage::create("back.png", "back_pressed.png");
        backwardMenuItem->setTarget(this, menu_selector(MCEnhancingEquipmentUI::backward));
        menu = CCMenu::create();
        addChild(menu);
        menu->addChild(backwardMenuItem);
        menu->setPosition(ccp(backwardMenuItem->getContentSize().width / 2 - 16 / contentScaleFactor,
                              winSize.height - backwardMenuItem->getContentSize().height / 2 + 16 / contentScaleFactor));
        
        menu = CCMenu::create();
        addChild(menu);
        
        label = CCLabelTTF::create("武器", "", fontSize);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCEnhancingEquipmentUI::equipmentType_clicked));
        menu->addChild(menuItem);
        menuItem->setTag(kMCTagWeapon);
        menuItem->setUserObject(weapons);
        weapons->release();
        
        label = CCLabelTTF::create("防具", "", fontSize);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCEnhancingEquipmentUI::equipmentType_clicked));
        menu->addChild(menuItem);
        menuItem->setUserObject(armors);
        armors->release();
        
        menu->alignItemsVertically();
        menu->setAnchorPoint(ccp(0, 0.5));
        menu->setPosition(ccp(offsetX,
                              winSize.height / 2));
        
        /* info */
        CCScale9Sprite *detailBackground = CCScale9Sprite::create(kMCDetailBackground,
                                                                  kMCDetailBackgroundSourceRect,
                                                                  kMCDetailBackgroundInsetRect);
        detailBackground->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        detailBackground->setPosition(ccp(offsetXLeft,
                                          winSize.height - offsetYInc));
        detailBackground->setContentSize(CCSizeMake(winSize.width - offsetXLeft * 3 / 2,
                                                    winSize.height - offsetYInc - 96 / contentScaleFactor));
        detailBackground->setOpacity(196);
        addChild(detailBackground);
        offsetYInc += 26;
        
        /* 装备信息 */
        weaponLayer_ = CCLayer::create();
        addChild(weaponLayer_);
        weaponLayer_->setVisible(false);
        armorLayer_ = CCLayer::create();
        addChild(armorLayer_);
        armorLayer_->setVisible(false);
        
        float iconWidth = 64 / contentScaleFactor;
        float iconHeight = 64 / contentScaleFactor;
        /* 共有的 */
        /* icon */
        weaponIcon_ = CCSprite::create();
        weaponLayer_->addChild(weaponIcon_);
        weaponIcon_->setAnchorPoint(ccp(1, 1)); /* 右上角 */
        weaponIcon_->setPosition(ccp(detailBackground->getPositionX()
                                     + detailBackground->getContentSize().width
                                     - iconWidth / 2,
                                     detailBackground->getPositionY()
                                     - iconHeight / 2));
        armorIcon_ = CCSprite::create();
        armorLayer_->addChild(armorIcon_);
        armorIcon_->setAnchorPoint(weaponIcon_->getAnchorPoint());
        armorIcon_->setPosition(weaponIcon_->getPosition());
        
        /* name */
        weaponName_ = CCLabelTTF::create("武器", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponName_);
        weaponName_->setAnchorPoint(ccp(0.5, 1));
        weaponName_->setPosition(ccp(weaponIcon_->getPositionX()
                                     - iconWidth / 2,
                                     weaponIcon_->getPositionY()
                                     - iconHeight));
        armorName_ = CCLabelTTF::create("防具", "Marker Felt", valueFontSize);
        armorLayer_->addChild(armorName_);
        armorName_->setAnchorPoint(weaponName_->getAnchorPoint());
        armorName_->setPosition(weaponName_->getPosition());
        offsetXLeft = detailBackground->getPositionX() + 13;
        
        /* ore */
        label = CCLabelTTF::create("矿石", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        label = CCLabelTTF::create("矿石", "Marker Felt", valueFontSize);
        armorLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        
        valuePositionXLeft = label->getPositionX() + label->getContentSize().width * 3;
        
        weaponOre_ = CCLabelTTF::create("奥利哈刚", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponOre_);
        weaponOre_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponOre_->setPosition(ccp(valuePositionXLeft,
                                    winSize.height - offsetYInc));
        armorOre_ = CCLabelTTF::create("奥利哈刚", "Marker Felt", valueFontSize);
        armorLayer_->addChild(armorOre_);
        armorOre_->setAnchorPoint(weaponOre_->getAnchorPoint()); /* 左上角 */
        armorOre_->setPosition(weaponOre_->getPosition());
        offsetYInc += label->getContentSize().height;
        
        /* dexterity */
        label = CCLabelTTF::create("敏捷调整值", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        label = CCLabelTTF::create("敏捷调整值", "Marker Felt", valueFontSize);
        armorLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        
        weaponDexterity_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponDexterity_);
        weaponDexterity_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponDexterity_->setPosition(ccp(valuePositionXLeft,
                                          winSize.height - offsetYInc));
        armorDexterity_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        armorLayer_->addChild(armorDexterity_);
        armorDexterity_->setAnchorPoint(weaponDexterity_->getAnchorPoint());
        armorDexterity_->setPosition(weaponDexterity_->getPosition());
        offsetYInc += label->getContentSize().height;
        
        float checkpoint = offsetYInc; /* ~~~~ */
        /* 各自的 */
        /* 武器展示 */
        /* damage */
        label = CCLabelTTF::create("伤害值", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        weaponDamage_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponDamage_);
        weaponDamage_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponDamage_->setPosition(ccp(valuePositionXLeft,
                                       winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        
        /* distance */
        label = CCLabelTTF::create("攻击距离", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        weaponDistance_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponDistance_);
        weaponDistance_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponDistance_->setPosition(ccp(valuePositionXLeft,
                                         winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        
        /* critical hit */
        label = CCLabelTTF::create("重击倍数", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        weaponCriticalHit_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponCriticalHit_);
        weaponCriticalHit_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponCriticalHit_->setPosition(ccp(valuePositionXLeft,
                                            winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        
        /* critical hit visible */
        label = CCLabelTTF::create("重击范围", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        
        float temp = label->getContentSize().height;
        label = CCLabelTTF::create("可视区域内", "Marker Felt", valueFontSize - 3);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft + label->getContentSize().width,
                               winSize.height - offsetYInc
                               - (temp - label->getContentSize().height) / 2));
        weaponCriticalHitVisible_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize - 3);
        weaponLayer_->addChild(weaponCriticalHitVisible_);
        weaponCriticalHitVisible_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponCriticalHitVisible_->setPosition(ccp(label->getPositionX() + label->getContentSize().width + 26 / contentScaleFactor,
                                                   winSize.height - offsetYInc
                                                   - (temp - label->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        
        /* critical hit invisible */
        CCLabelTTF *tempLabel = label;
        label = CCLabelTTF::create("非可视区域内", "Marker Felt", valueFontSize - 3);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(tempLabel->getPositionX(),
                               winSize.height - offsetYInc
                               - (temp - label->getContentSize().height) / 2));
        weaponCriticalHitInvisible_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize - 3);
        weaponLayer_->addChild(weaponCriticalHitInvisible_);
        weaponCriticalHitInvisible_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponCriticalHitInvisible_->setPosition(ccp(weaponCriticalHitVisible_->getPositionX(),
                                                     winSize.height - offsetYInc
                                                     - (temp - label->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        
        /* effect */
        label = CCLabelTTF::create("附带效果", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        weaponEffect_ = CCLabelTTF::create("无", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponEffect_);
        weaponEffect_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponEffect_->setPosition(ccp(valuePositionXLeft,
                                       winSize.height - offsetYInc));
        
        /* effect check */
        label = CCLabelTTF::create("检测范围", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        weaponEffectCheckLabel_ = label;
        weaponEffectCheck_ = CCLabelTTF::create("无", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponEffectCheck_);
        weaponEffectCheck_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponEffectCheck_->setPosition(ccp(valuePositionXLeft,
                                            winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        /* 升级价格 */
        label = CCLabelTTF::create("升级价格", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        weaponPriceLabel_ = label;
        weaponPrice_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponPrice_);
        weaponPrice_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponPrice_->setPosition(ccp(valuePositionXLeft,
                                      winSize.height - offsetYInc));
        offsetYInc = checkpoint;
        
        /* 防具展示 */
        /* defense */
        label = CCLabelTTF::create("防御加值", "Marker Felt", valueFontSize);
        armorLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        armorDefense_ = CCLabelTTF::create("无", "Marker Felt", valueFontSize);
        armorLayer_->addChild(armorDefense_);
        armorDefense_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        armorDefense_->setPosition(ccp(valuePositionXLeft,
                                       winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        /* armor check penalty */
        label = CCLabelTTF::create("防具检定减值", "Marker Felt", valueFontSize);
        armorLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        armorArmorCheckPenalty_ = CCLabelTTF::create("无", "Marker Felt", valueFontSize);
        armorLayer_->addChild(armorArmorCheckPenalty_);
        armorArmorCheckPenalty_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        armorArmorCheckPenalty_->setPosition(ccp(valuePositionXLeft,
                                                 winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        /* 升级价格 */
        label = CCLabelTTF::create("升级价格", "Marker Felt", valueFontSize);
        armorLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft,
                               winSize.height - offsetYInc));
        armorPriceLabel_ = label;
        armorPrice_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        armorLayer_->addChild(armorPrice_);
        armorPrice_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        armorPrice_->setPosition(ccp(valuePositionXLeft,
                                     winSize.height - offsetYInc));
        
        /* 武器升级 */
        CCMenu *button = CCMenu::create();
        CCLabelTTF *buttonLabel = CCLabelTTF::create("升级", "Marker Felt", valueFontSize);
        addChild(button);
        CCMenuItemLabel *buttonCell = CCMenuItemLabel::create(buttonLabel,
                                                              this,
                                                              menu_selector(MCEnhancingEquipmentUI::levelUp_click));
        button->addChild(buttonCell);
        button->setPosition(ccp(detailBackground->getPositionX()
                                + detailBackground->getContentSize().width
                                - buttonCell->getContentSize().width,
                                detailBackground->getPositionY()
                                - detailBackground->getContentSize().height
                                + buttonCell->getContentSize().height));
        button->setVisible(false);
        levelUpButton_ = button;
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
                                    (400 - line->getPositionY() * 2) / contentScaleFactor);
        tableView_ = CCTableView::create(this, tableViewSize_);
        addChild(tableView_);
        tableView_->setDirection(kCCScrollViewDirectionVertical);
        tableView_->setDelegate(this);
        tableView_->setVerticalFillOrder(kCCTableViewFillTopDown);
        tableView_->reloadData();
        tableView_->setPosition(ccp(menu->getPositionX() + menuItem->getContentSize().width,
                                    (winSize.height - tableViewSize_.height) / 2));
        selectedCell_ = NULL;
        
        equipments_ = NULL;
        lastSelectedEquipmentTypeMenuItem_ = NULL;
        lastSelectedEquipmentItem_ = NULL;
        
        return true;
    }
    
    return false;
}

void
MCEnhancingEquipmentUI::initPosition()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    setAnchorPoint(CCPointZero);
    setPosition(ccp(0, winSize.height));
}

void
MCEnhancingEquipmentUI::attach(MCScene *aScene)
{
    aScene->addChild(this);
    aScene->pauseScene();
    initPosition();
    
    runAction(CCMoveTo::create(kMCActionDuration, CCPointZero));
}

void
MCEnhancingEquipmentUI::detach()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    runAction(CCSequence::createWithTwoActions(CCMoveTo::create(kMCActionDuration, ccp(0, winSize.height)),
                                               CCCallFunc::create(this, callfunc_selector(MCEnhancingEquipmentUI::destroy))));
}

void
MCEnhancingEquipmentUI::confirmDidClickYesButton(MCConfirm *aConfirm)
{
    MCEquipmentManager *equipmentManager = MCEquipmentManager::sharedEquipmentManager();
    MCEquipmentItem *equipmentItem = lastSelectedEquipmentItem_;
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    int result = equipmentManager->levelUp(equipmentItem);
    
    if (result == kMCHandleSucceed) {
        MCToast::make(this, "升级成功！")->show();
        if (lastSelectedEquipmentTypeMenuItem_->getTag() == kMCTagWeapon) {
            loadWeapon(equipmentItem);
        } else {
            loadArmor(equipmentItem);
        }
        money_->setString(CCString::createWithFormat("%d", backpack->getMoney())->getCString());
    } else if (result == kMCNotEnoughMoney) {
        MCToast::make(this, "兄台，你不够钱~")->show();
    } else if (result == kMCFullLevel) {
        MCToast::make(this, "等级已満~")->show();
    }
}

/* CCTableViewDataSource */
CCSize
MCEnhancingEquipmentUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(96, 32);
}

CCTableViewCell *
MCEnhancingEquipmentUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    MCEquipmentItem *equipmentItem = dynamic_cast<MCEquipmentItem *>(equipments_->objectAtIndex(idx));

    CCTableViewCell *cell = table->dequeueCell();
    if (! cell) {
        cell = MCTableViewTextFieldCell::create(equipmentItem->getName()->getCString(),
                                                "Helvetica",
                                                18.0f / CCDirectorGetContentScaleFactor());
    } else {
        dynamic_cast<MCTableViewTextFieldCell *>(cell)->setString(equipmentItem->getName()->getCString());
    }
    
    
    return cell;
}

unsigned int
MCEnhancingEquipmentUI::numberOfCellsInTableView(CCTableView *table)
{
    return equipments_ ? equipments_->count() : 0;
}

/* CCTableViewDelegate */
void
MCEnhancingEquipmentUI::tableCellTouched(CCTableView *table, CCTableViewCell *cell)
{
    if (selectedCell_) {
        selectedCell_->unselected();
    }
    selectedCell_ = dynamic_cast<MCTableViewTextFieldCell *>(cell);
    selectedCell_->selected();
    
    MCEquipmentItem *equipmentItem = dynamic_cast<MCEquipmentItem *>(equipments_->objectAtIndex(cell->getIdx()));
    if (lastSelectedEquipmentTypeMenuItem_->getTag() == kMCTagWeapon) {
        loadWeapon(equipmentItem);
        weaponLayer_->setVisible(true);
        armorLayer_->setVisible(false);
    } else {
        loadArmor(equipmentItem);
        weaponLayer_->setVisible(false);
        armorLayer_->setVisible(true);
    }
    lastSelectedEquipmentItem_ = equipmentItem;
}

void
MCEnhancingEquipmentUI::equipmentType_clicked(CCObject *obj)
{
    CCMenuItemLabel *menuItem = dynamic_cast<CCMenuItemLabel *>(obj);
    
    if (lastSelectedEquipmentTypeMenuItem_) {
        dynamic_cast<CCLabelTTF *>(lastSelectedEquipmentTypeMenuItem_->getLabel())->setColor(kMCUnselectedColor);
    }
    dynamic_cast<CCLabelTTF *>(menuItem->getLabel())->setColor(kMCSelectedColor);
    lastSelectedEquipmentTypeMenuItem_ = menuItem;
    
    equipments_= dynamic_cast<CCArray *>(menuItem->getUserObject());
    tableView_->reloadData();
    weaponLayer_->setVisible(false);
    armorLayer_->setVisible(false);
}

void
MCEnhancingEquipmentUI::levelUp_click(CCObject *aSender)
{
    MCEquipmentItem *equipmentItem = lastSelectedEquipmentItem_;
    MCOre *nextLevelOre = equipmentItem->getOre()->getNextLevel();

    if (nextLevelOre) {
        MCConfirm::confirm(this,
                           this,
                           CCString::createWithFormat("升级需要%d马克，确定升级？",
                                                      equipmentItem->getPrice()
                                                      + nextLevelOre->getPrice())->getCString());
    } else {
        MCToast::make(this, "等级已満，不能再升级！");
    }
}

void
MCEnhancingEquipmentUI::backward(CCObject* aSender) /* 关闭.... */
{
    detach();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCEnhancingEquipmentUIDidHideNotification);
}

void
MCEnhancingEquipmentUI::destroy()
{
    MCScene *scene = dynamic_cast<MCScene *>(m_pParent);
    
    scene->resumeScene();
    removeFromParentAndCleanup(true);
}

void
MCEnhancingEquipmentUI::loadWeapon(MCEquipmentItem *aWeapon)
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *spriteFrame;
    /* icon */
    spriteFrame = cache->spriteFrameByName(aWeapon->getName()->getCString()); /* 名字为key */
    if (! spriteFrame) {
        float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
        CCRect spriteFrameRect = CCRectMake(0, 0,
                                            64 / contentScaleFactor,
                                            64 / contentScaleFactor);
        spriteFrame = CCSpriteFrame::create(aWeapon->getIcon()->getCString(), spriteFrameRect);
        cache->addSpriteFrame(spriteFrame, aWeapon->getName()->getCString());
    }
    /* icon */
    weaponIcon_->setDisplayFrame(spriteFrame);
    /* name */
    weaponName_->setString(aWeapon->getName()->getCString());
    /* ore */
    MCOre *ore = aWeapon->getOre();
    weaponOre_->setString(ore->getName()->getCString());
    /* dexterity */
    MCWeapon *weaponEquipment = dynamic_cast<MCWeapon *>(aWeapon->getEquipment());
    mc_dexterity_t dexterity = weaponEquipment->dexterity + ore->getWeaponDexterity();
    weaponDexterity_->setString(CCString::createWithFormat("%s%hi",
                                                           dexterity >= 0 ? "+" : "",
                                                           dexterity)->getCString());
    
    /* damage */
    weaponDamage_->setString(CCString::createWithFormat("%hiD%hi%s",
                                                        MCDiceCount(weaponEquipment->damage),
                                                        MCDiceSize(weaponEquipment->damage),
                                                        (ore->getDamage() > 0
                                                         ? CCString::createWithFormat("+%hi", ore->getDamage())->getCString()
                                                         : ""))->getCString());
    /* distance */
    weaponDistance_->setString(CCString::createWithFormat("%hi",
                                                          weaponEquipment->distance)->getCString());
    /* critical hit */
    mc_critical_hit_t criticalHit = weaponEquipment->criticalHit + ore->getCriticalHit();
    weaponCriticalHit_->setString(CCString::createWithFormat("x %.1f",
                                                             criticalHit)->getCString());
    /* critical hit visible */
    MCDiceRange criticalHitVisible = weaponEquipment->criticalHitVisible;
    CCString *ccstring;
    if (criticalHitVisible.min == criticalHitVisible.max) {
        ccstring = CCString::createWithFormat("%hu", criticalHitVisible.max);
    } else {
        ccstring = CCString::createWithFormat("%hu-%hu", criticalHitVisible.min, criticalHitVisible.max);
    }
    weaponCriticalHitVisible_->setString(ccstring->getCString());
    /* critical hit invisible */
    MCDiceRange criticalHitInvisible = weaponEquipment->criticalHitInvisible;
    if (criticalHitInvisible.min == criticalHitInvisible.max) {
        ccstring = CCString::createWithFormat("%hu", criticalHitVisible.max);
    } else {
        ccstring = CCString::createWithFormat("%hu-%hu", criticalHitInvisible.min, criticalHitInvisible.max);
    }
    weaponCriticalHitInvisible_->setString(ccstring->getCString());
    /* effect */
    if (weaponEquipment->effect == MCNormalState) {
        weaponEffect_->setString("无");
    } else {
        weaponEffect_->setString(MCRoleStateGetName(weaponEquipment->effect));
    }
    /* effect check */
    if (weaponEquipment->effect == MCNormalState) {
        weaponEffectCheckLabel_->setVisible(false);
        weaponEffectCheck_->setVisible(false);
    } else {
        weaponEffectCheckLabel_->setVisible(true);
        weaponEffectCheck_->setVisible(true);
        weaponEffectCheck_->setString(CCString::createWithFormat("%hu-%hu",
                                                                 weaponEquipment->effectCheck.min,
                                                                 weaponEquipment->effectCheck.max)->getCString());
    }
    /* price */
    MCOre *nextLevelOre = ore->getNextLevel();
    if (nextLevelOre) {
        weaponPrice_->setString(CCString::createWithFormat("%d",
                                                          aWeapon->getPrice() + nextLevelOre->getPrice())->getCString());
        weaponPriceLabel_->setVisible(true);
        weaponPrice_->setVisible(true);
        levelUpButton_->setVisible(true);
    } else {
        weaponPriceLabel_->setVisible(false);
        weaponPrice_->setVisible(false);
        levelUpButton_->setVisible(false);
    }
}

void
MCEnhancingEquipmentUI::loadArmor(MCEquipmentItem *anArmor)
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *spriteFrame;
    /* icon */
    spriteFrame = cache->spriteFrameByName(anArmor->getName()->getCString()); /* 名字为key */
    if (! spriteFrame) {
        float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
        CCRect spriteFrameRect = CCRectMake(0, 0,
                                            64 / contentScaleFactor,
                                            64 / contentScaleFactor);
        spriteFrame = CCSpriteFrame::create(anArmor->getIcon()->getCString(), spriteFrameRect);
        cache->addSpriteFrame(spriteFrame, anArmor->getName()->getCString());
    }
    /* icon */
    armorIcon_->setDisplayFrame(spriteFrame);
    /* name */
    armorName_->setString(anArmor->getName()->getCString());
    /* ore */
    MCOre *ore = anArmor->getOre();
    armorOre_->setString(ore->getName()->getCString());
    /* dexterity */
    MCArmor *armorEquipment = dynamic_cast<MCArmor *>(anArmor->getEquipment());
    mc_dexterity_t dexterity = armorEquipment->dexterity + ore->getArmorDexterity();
    armorDexterity_->setString(CCString::createWithFormat("%s%hi",
                                                          dexterity >= 0 ? "+" : "",
                                                          dexterity)->getCString());
    /* defense */
    armorDefense_->setString(CCString::createWithFormat("+%hu",
                                                        armorEquipment->defense)->getCString());
    /* armor check penalty */
    armorArmorCheckPenalty_->setString(CCString::createWithFormat("%hi",
                                                                  armorEquipment->armorCheckPenalty
                                                                  + ore->getArmorCheckPenalty())->getCString());
    /* price */
    MCOre *nextLevelOre = ore->getNextLevel();
    if (nextLevelOre) {
        armorPrice_->setString(CCString::createWithFormat("%d",
                                                          anArmor->getPrice() + nextLevelOre->getPrice())->getCString());
        armorPriceLabel_->setVisible(true);
        armorPrice_->setVisible(true);
        levelUpButton_->setVisible(true);
    } else {
        armorPriceLabel_->setVisible(false);
        armorPrice_->setVisible(false);
        levelUpButton_->setVisible(false);
    }
}
