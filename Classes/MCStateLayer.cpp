//
//  MCStateLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCStateLayer.h"
#include "MCHero.h"
#include "MCBackpack.h"
#include "MCEquipmentManager.h"

#include <cocos-ext.h>
USING_NS_CC_EXT;

#define kMCValueOffsetX 13

bool
MCStateLayer::init()
{
    if (MCBaseInfoLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCLabelTTF *label;
        CCScale9Sprite *line;
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        float fontSize = 36 / contentScaleFactor;
        float valueFontSize = 24 / contentScaleFactor;
        float contentHeight = winSize.height - 90;
        float contentHeightWithoutBottom = contentHeight * 2 / 3;
        float offsetX = 180 / contentScaleFactor;
        float offsetY = contentHeightWithoutBottom / 8 + 4;
        float offsetYInc = 45 / contentScaleFactor;
        float valuePositionX;
        MCHero *hero = MCHero::sharedHero();
        
        /* HP */
        label = CCLabelTTF::create("生命值", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetX, winSize.height - offsetYInc));
        offsetYInc += offsetY;
        valuePositionX = label->getPosition().x + label->getContentSize().width * 8 / 3;
        
        hp_ = CCLabelTTF::create("0/0", "Marker Felt", valueFontSize);
        addChild(hp_);
        hp_->setAnchorPoint(ccp(0.5, 1));
        hp_->setPosition(ccp(valuePositionX,
                             label->getPosition().y - (label->getContentSize().height - hp_->getContentSize().height) / 2));
        
        /* PP */
        label = CCLabelTTF::create("体力值", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetX, winSize.height - offsetYInc));
        offsetYInc += offsetY;
        
        pp_ = CCLabelTTF::create("0/0", "Marker Felt", valueFontSize);
        addChild(pp_);
        pp_->setAnchorPoint(ccp(0.5, 1));
        pp_->setPosition(ccp(valuePositionX,
                             label->getPosition().y - (label->getContentSize().height - pp_->getContentSize().height) / 2));
        
        /* state */
        label = CCLabelTTF::create("状态", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetX, winSize.height - offsetYInc));
        offsetYInc += offsetY;
        
        state_ = CCLabelTTF::create(kMCNormalState, "Marker Felt", valueFontSize);
        addChild(state_);
        state_->setAnchorPoint(ccp(0.5, 1));
        state_->setPosition(ccp(valuePositionX,
                                label->getPosition().y - (label->getContentSize().height - state_->getContentSize().height) / 2));
        offsetYInc += offsetY;
        
        /* 当前武器 */
        label = CCLabelTTF::create("当前武器", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetX, winSize.height - offsetYInc));
        offsetYInc += offsetY;
        
        weapon_ = CCLabelTTF::create("未知武器", "Marker Felt", valueFontSize);
        addChild(weapon_);
        weapon_->setAnchorPoint(ccp(0.5, 1));
        weapon_->setPosition(ccp(valuePositionX,
                                 label->getPosition().y - (label->getContentSize().height - weapon_->getContentSize().height) / 2));
        
        /* damage */
        label = CCLabelTTF::create("武器伤害", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetX, winSize.height - offsetYInc));
        offsetYInc += offsetY;
        
        damage_ = CCLabelTTF::create("0", "", valueFontSize);
        addChild(damage_);
        damage_->setAnchorPoint(ccp(0.5, 1));
        damage_->setPosition(ccp(valuePositionX,
                                 label->getPosition().y - (label->getContentSize().height - damage_->getContentSize().height) / 2));
        
        /* AC */
        label = CCLabelTTF::create("防御等级", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetX, winSize.height - offsetYInc));
//        offsetYInc += offsetY;
        
        ac_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        addChild(ac_);
        ac_->setAnchorPoint(ccp(0.5, 1));
        ac_->setPosition(ccp(valuePositionX,
                             label->getPosition().y - (label->getContentSize().height - ac_->getContentSize().height) / 2));
                
        /* money */
        label = CCLabelTTF::create("马克", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(1, 0)); /* 右下角 */
        label->setPosition(ccp(winSize.width - 48 / contentScaleFactor, 48 / contentScaleFactor));
        
        money_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        addChild(money_);
        money_->setAnchorPoint(ccp(1, 0)); /* 右下角 */
        money_->setPosition(ccp(label->getPosition().x - label->getContentSize().width - kMCValueOffsetX,
                                48 / contentScaleFactor));
        
        /* line */
        CCSize separatorSize = CCSizeMake(winSize.width - offsetX - 45 / contentScaleFactor, 4);
        line = CCScale9Sprite::create("UI/separator.png");
        addChild(line);
        line->setContentSize(separatorSize);
        line->setAnchorPoint(ccp(0, 0)); /* 左下角 */
        line->setPosition(ccp(offsetX, 45 / contentScaleFactor));
        
        icon_ = CCSprite::create(hero->getFace()->getCString());
        icon_->setAnchorPoint(ccp(1, 1)); /* 右上角 */
        icon_->setPosition(ccp(winSize.width - 54 / contentScaleFactor,
                               winSize.height - 54 / contentScaleFactor));
        
        CCRect iconBoxRect = CCRectMake(0, 0, 114 / contentScaleFactor, 114 / contentScaleFactor);
        CCScale9Sprite *iconBox = CCScale9Sprite::create("UI/face_box.png", iconBoxRect);
        addChild(iconBox);
        addChild(icon_);
        iconBox->setAnchorPoint(ccp(1, 1)); /* 右上角 */
        iconBox->setPosition(ccp(icon_->getPosition().x + 9 / contentScaleFactor,
                                 icon_->getPosition().y + 9 / contentScaleFactor));
        
        name_ = CCLabelTTF::create("男猪脚", "Marker Felt", valueFontSize);
        addChild(name_);
        name_->setColor(ccc3(204, 204, 204));
        name_->setAnchorPoint(ccp(1, 1));
        name_->setPosition(ccp(iconBox->getPosition().x - (iconBox->getContentSize().width - name_->getContentSize().width) / 2,
                               iconBox->getPosition().y - iconBox->getContentSize().height));
        
        return true;
    }
    
    return false;
}

void
MCStateLayer::loadData()
{
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    MCHero *hero = MCHero::sharedHero();
    
    /* HP */
    hp_->setString(CCString::createWithFormat("%hi/%hi", hero->getHP(), hero->getMaxHP())->getCString());
    
    /* PP */
    pp_->setString(CCString::createWithFormat("%.0f/%.0f", hero->getPP(), hero->getMaxPP())->getCString());
    
    /* state */
    MCRoleState roleState = hero->getRoleState();
    if (MCNormalState == roleState) {
        state_->setString(kMCNormalState);
    } else {
        std::string state;
        if ((roleState & MCCurseState) == MCCurseState) {
            state.append(kMCCurseState);
        }
        if ((roleState & MCParalysisState) == MCParalysisState) {
            if (state.size() > 0) {
                state.append("、");
            }
            state.append(kMCParalysisState);
        }
        if ((roleState & MCVertigoState) == MCVertigoState) {
            if (state.size() > 0) {
                state.append("、");
            }
            state.append(kMCVertigoState);
        }
        if ((roleState & MCPoisonedState) == MCPoisonedState) {
            if (state.size() > 0) {
                state.append("、");
            }
            state.append(kMCPoisonedState);
        }
        if ((roleState & MCBlindingState) == MCBlindingState) {
            if (state.size() > 0) {
                state.append("、");
            }
            state.append(kMCBlindingState);
        }
        if ((roleState & MCChaosState) == MCChaosState) {
            if (state.size() > 0) {
                state.append("、");
            }
            state.append(kMCChaosState);
        }
        state_->setString(state.c_str());
    }
    
    /* 当前武器 */
    MCEquipmentManager *equipmentManager = MCEquipmentManager::sharedEquipmentManager();
    MCEquipmentItem *currentWeapon = equipmentManager->getCurrentWeapon();
    MCOre *ore = currentWeapon->getOre();
    weapon_->setString(CCString::createWithFormat("%s[%s]",
                                                  currentWeapon->getName()->getCString(),
                                                  ore->getName()->getCString())->getCString());
    /* damage */
    MCWeapon *weapon = dynamic_cast<MCWeapon *>(currentWeapon->getEquipment());
    damage_->setString(CCString::createWithFormat("%hiD%hi%s",
                                                  MCDiceCount(weapon->damage),
                                                  MCDiceSize(weapon->damage),
                                                  (ore->getDamage() > 0
                                                   ? CCString::createWithFormat("+%hi", ore->getDamage())->getCString()
                                                   : ""))->getCString());
    /* AC */
    ac_->setString(CCString::createWithFormat("%hi", equipmentManager->getAC())->getCString());
    /* money */
    money_->setString(CCString::createWithFormat("%hi", backpack->getMoney())->getCString());
}
