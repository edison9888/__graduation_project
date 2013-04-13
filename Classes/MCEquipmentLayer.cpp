//
//  MCEquipmentLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCEquipmentLayer.h"
#include "MCHero.h"
#include "MCEquipmentManager.h"

#include <cocos-ext.h>
USING_NS_CC_EXT;

static const int kMCTagWeapon = 0x13;
static const ccColor3B kMCItemColor = ccc3(192, 192, 192);
static const char *kMCDetailBackground = "UI/pl_detail_background.png";
static const CCRect kMCDetailBackgroundSourceRect = CCRectMake(0, 0, 400, 400);
static const CCRect kMCDetailBackgroundInsetRect = CCRectMake(13, 13, 387, 387);

bool
MCEquipmentLayer::init()
{
    if (MCDetailLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCLabelTTF *label;
        float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
        float fontSize = 24 / contentScaleFactor;
        float valueFontSize = 24 / contentScaleFactor;
        float contentHeight = winSize.height - 90 / contentScaleFactor;
        float contentHeightWithoutBottom = contentHeight * 2 / 3;
        float offsetXLeft = 180 / contentScaleFactor;
        float offsetXRight = 180 / contentScaleFactor;
        float offsetY = contentHeightWithoutBottom / 7 + 4;
        float offsetYInc = offsetY;
        float valuePositionXLeft;
        float valuePositionXRight;
        MCEquipmentManager *equipmentManager = MCEquipmentManager::sharedEquipmentManager();
        
        /* 武器 */
        label = CCLabelTTF::create("武器", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        valuePositionXLeft = label->getPosition().x + label->getContentSize().width * 16 / 5;
        
        CCMenu *button = CCMenu::create();
        CCLabelTTF *buttonLabel = CCLabelTTF::create("武器武器[奥利哈刚]", "Marker Felt", valueFontSize);
        buttonLabel->setColor(kMCItemColor);
        addChild(button);
        CCMenuItemLabel *buttonCell = CCMenuItemLabel::create(buttonLabel, this, menu_selector(MCEquipmentLayer::itemDidClick));
        buttonCell->setTag(kMCTagWeapon);
        button->addChild(buttonCell);
        button->setAnchorPoint(ccp(0, 1));
        button->setPosition(ccp(valuePositionXLeft,
                                label->getPositionY() - buttonCell->getContentSize().height / 2));
        weapon_ = buttonLabel;
        offsetXRight = button->getPosition().x + buttonCell->getContentSize().width * 2 / 3;
        
        /* 头盔 */
        label = CCLabelTTF::create("头盔", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXRight, winSize.height - offsetYInc));
        valuePositionXRight = label->getPosition().x + label->getContentSize().width * 16 / 5;
        
        button = CCMenu::create();
        buttonLabel = CCLabelTTF::create("头盔头盔[奥利哈刚]", "Marker Felt", valueFontSize);
        buttonLabel->setColor(kMCItemColor);
        addChild(button);
        buttonCell = CCMenuItemLabel::create(buttonLabel, this, menu_selector(MCEquipmentLayer::itemDidClick));
        buttonCell->setUserObject(equipmentManager->getHelmet());
        button->addChild(buttonCell);
        button->setAnchorPoint(ccp(0, 1));
        button->setPosition(ccp(valuePositionXRight,
                                label->getPositionY() - buttonCell->getContentSize().height / 2));
        helmet_ = buttonLabel;
        offsetYInc += buttonCell->getContentSize().height;
        
        /* 铠甲 */
        label = CCLabelTTF::create("铠甲", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        
        button = CCMenu::create();
        buttonLabel = CCLabelTTF::create("铠甲铠甲[奥利哈刚]", "Marker Felt", valueFontSize);
        buttonLabel->setColor(kMCItemColor);
        addChild(button);
        buttonCell = CCMenuItemLabel::create(buttonLabel, this, menu_selector(MCEquipmentLayer::itemDidClick));
        buttonCell->setUserObject(equipmentManager->getArmor());
        button->addChild(buttonCell);
        button->setAnchorPoint(ccp(0, 1));
        button->setPosition(ccp(valuePositionXLeft,
                                label->getPositionY() - buttonCell->getContentSize().height / 2));
        armor_ = buttonLabel;
        
        /* 护胫 */
        label = CCLabelTTF::create("护胫", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXRight, winSize.height - offsetYInc));
        
        button = CCMenu::create();
        buttonLabel = CCLabelTTF::create("护胫护胫[奥利哈刚]", "Marker Felt", valueFontSize);
        buttonLabel->setColor(kMCItemColor);
        addChild(button);
        buttonCell = CCMenuItemLabel::create(buttonLabel, this, menu_selector(MCEquipmentLayer::itemDidClick));
        buttonCell->setUserObject(equipmentManager->getShinGuard());
        button->addChild(buttonCell);
        button->setAnchorPoint(ccp(0, 1));
        button->setPosition(ccp(valuePositionXRight,
                                label->getPositionY() - buttonCell->getContentSize().height / 2));
        shinguard_ = buttonLabel;
        offsetYInc += buttonCell->getContentSize().height;
        
        CCScale9Sprite *detailBackground = CCScale9Sprite::create(kMCDetailBackground,
                                                                  kMCDetailBackgroundSourceRect,
                                                                  kMCDetailBackgroundInsetRect);
        detailBackground->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        detailBackground->setPosition(ccp(offsetXLeft,
                                          winSize.height - offsetYInc));
        detailBackground->setContentSize(CCSizeMake(winSize.width - offsetXLeft * 3 / 2,
                                                    winSize.height - offsetYInc));
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
        
        /* price */
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
        weaponCriticalHitVisible_->setPosition(ccp(label->getPositionX() + label->getContentSize().width + 13,
                                                   winSize.height - offsetYInc
                                                   - (temp - label->getContentSize().height) / 2));
        
        /* critical hit invisible */
        label = CCLabelTTF::create("非可视区域内", "Marker Felt", valueFontSize - 3);
        weaponLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXRight,
                               winSize.height - offsetYInc
                               - (temp - label->getContentSize().height) / 2));
        weaponCriticalHitInvisible_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize - 3);
        weaponLayer_->addChild(weaponCriticalHitInvisible_);
        weaponCriticalHitInvisible_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponCriticalHitInvisible_->setPosition(ccp(label->getPositionX() + label->getContentSize().width + 13,
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
        label->setPosition(ccp(offsetXRight,
                               winSize.height - offsetYInc));
        weaponEffectCheckLabel_ = label;
        weaponEffectCheck_ = CCLabelTTF::create("无", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(weaponEffectCheck_);
        weaponEffectCheck_->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        weaponEffectCheck_->setPosition(ccp(valuePositionXRight,
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
        
        /* 切换武器 */
        button = CCMenu::create();
        buttonLabel = CCLabelTTF::create("切换武器", "Marker Felt", valueFontSize);
        weaponLayer_->addChild(button);
        buttonCell = CCMenuItemLabel::create(buttonLabel, this, menu_selector(MCEquipmentLayer::changeWeapon));
        button->addChild(buttonCell);
        button->setPosition(ccp(detailBackground->getPositionX()
                                + detailBackground->getContentSize().width
                                - buttonCell->getContentSize().width,
                                detailBackground->getPositionY()
                                - detailBackground->getContentSize().height
                                + buttonCell->getContentSize().height));
        
        return true;
    }
    
    return false;
}

void
MCEquipmentLayer::loadData()
{
    MCEquipmentManager *equipmentManager = MCEquipmentManager::sharedEquipmentManager();
    /* 武器 */
    MCEquipmentItem *equipment = equipmentManager->getCurrentWeapon();
    weapon_->setString(CCString::createWithFormat("%s[%s]",
                                                  equipment->getName()->getCString(),
                                                  equipment->getOre()->getName()->getCString())->getCString());
    /* 头盔 */
    equipment = equipmentManager->getHelmet();
    helmet_->setString(CCString::createWithFormat("%s[%s]",
                                                  equipment->getName()->getCString(),
                                                  equipment->getOre()->getName()->getCString())->getCString());
    /* 铠甲 */
    equipment = equipmentManager->getArmor();
    armor_->setString(CCString::createWithFormat("%s[%s]",
                                                 equipment->getName()->getCString(),
                                                 equipment->getOre()->getName()->getCString())->getCString());
    /* 护胫 */
    equipment = equipmentManager->getShinGuard();
    shinguard_->setString(CCString::createWithFormat("%s[%s]",
                                                     equipment->getName()->getCString(),
                                                     equipment->getOre()->getName()->getCString())->getCString());
    weaponLayer_->setVisible(false);
    armorLayer_->setVisible(false);
}

void
MCEquipmentLayer::showWeapon()
{
    MCEquipmentManager *equipmentManager = MCEquipmentManager::sharedEquipmentManager();
    MCEquipmentItem *weapon = equipmentManager->getCurrentWeapon();
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *spriteFrame;
    /* icon */
    spriteFrame = cache->spriteFrameByName(weapon->getName()->getCString()); /* 名字为key */
    if (! spriteFrame) {
        float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
        CCRect spriteFrameRect = CCRectMake(0, 0,
                                            64 / contentScaleFactor,
                                            64 / contentScaleFactor);
        spriteFrame = CCSpriteFrame::create(weapon->getIcon()->getCString(), spriteFrameRect);
        cache->addSpriteFrame(spriteFrame, weapon->getName()->getCString());
    }
    /* icon */
    weaponIcon_->setDisplayFrame(spriteFrame);
    /* name */
    weaponName_->setString(weapon->getName()->getCString());
    /* ore */
    MCOre *ore = weapon->getOre();
    weaponOre_->setString(ore->getName()->getCString());
    /* dexterity */
    MCWeapon *weaponEquipment = dynamic_cast<MCWeapon *>(weapon->getEquipment());
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
}

void
MCEquipmentLayer::showArmor(MCEquipmentItem *anArmor)
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
}

void
MCEquipmentLayer::itemDidClick(CCObject *obj)
{
    CCMenuItem *buttonCell = dynamic_cast<CCMenuItem *>(obj);
    int tag = buttonCell->getTag();
    if (tag == kMCTagWeapon) {
        showWeapon();
        weaponLayer_->setVisible(true);
        armorLayer_->setVisible(false);
    } else {
        showArmor(dynamic_cast<MCEquipmentItem *>(buttonCell->getUserObject()));
        weaponLayer_->setVisible(false);
        armorLayer_->setVisible(true);
    }
}

void
MCEquipmentLayer::changeWeapon(CCObject *obj)
{
    MCWeaponSelector::weaponSelector(this, this);
}

void
MCEquipmentLayer::weaponSelectorDidFinishSelecting(MCWeaponSelector *aWeaponSelector, MCEquipmentItem *aWeapon)
{
    MCEquipmentManager::sharedEquipmentManager()->changeWeaponTo(aWeapon);
    loadData();
}

void
MCEquipmentLayer::weaponSelectorDidCancelSelecting(MCWeaponSelector *aWeaponSelector)
{
}

void
MCEquipmentLayer::weaponWasSelected(CCObject *obj)
{
}