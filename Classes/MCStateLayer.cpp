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

#include <cocos-ext.h>
USING_NS_CC_EXT;

#define MCValueOffsetX 13

bool
MCStateLayer::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCLabelTTF *label;
        CCScale9Sprite *line;
        float fontSize = 36;
        float valueFontSize = 24;
        float contentHeight = winSize.height - 90;
        float contentHeightWithoutBottom = contentHeight * 2 / 3;
        float offsetX = 180;
        float offsetY = contentHeightWithoutBottom / 8 + 4;
        float offsetYInc = 45;
        float valuePositionX;
        
        /* HP */
        label = CCLabelTTF::create("生命值", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetX, winSize.height - offsetYInc));
        offsetYInc += offsetY;
        valuePositionX = label->getPosition().x + label->getContentSize().width * 4 / 3;
        
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
        
        state_ = CCLabelTTF::create("健康", "Marker Felt", valueFontSize);
        addChild(state_);
        state_->setAnchorPoint(ccp(0.5, 1));
        state_->setPosition(ccp(valuePositionX,
                                label->getPosition().y - (label->getContentSize().height - state_->getContentSize().height) / 2));
        offsetYInc += offsetY;
        
        /* damage */
        label = CCLabelTTF::create("武器伤害", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetX, winSize.height - offsetYInc));
        offsetYInc += offsetY;
        
        damage_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        addChild(damage_);
        damage_->setAnchorPoint(ccp(0.5, 1));
        damage_->setPosition(ccp(valuePositionX,
                                 label->getPosition().y - (label->getContentSize().height - damage_->getContentSize().height) / 2));
        
        /* AC */
        label = CCLabelTTF::create("防御等级", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetX, winSize.height - offsetYInc));
        offsetYInc += offsetY;
        
        ac_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        addChild(ac_);
        ac_->setAnchorPoint(ccp(0.5, 1));
        ac_->setPosition(ccp(valuePositionX,
                             label->getPosition().y - (label->getContentSize().height - ac_->getContentSize().height) / 2));
                
        /* money */
        label = CCLabelTTF::create("马克", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(1, 0)); /* 右下角 */
        label->setPosition(ccp(winSize.width - 48, 48));
        
        money_ = CCLabelTTF::create("0", "Marker Felt", valueFontSize);
        addChild(money_);
        money_->setAnchorPoint(ccp(1, 0)); /* 右下角 */
        money_->setPosition(ccp(label->getPosition().x - label->getContentSize().width - MCValueOffsetX,
                                48));
        
        /* line */
        CCSize separatorSize = CCSizeMake(winSize.width - offsetX - 45, 1);
        line = CCScale9Sprite::create("UI/separator.png");
        addChild(line);
        line->setContentSize(separatorSize);
        line->setAnchorPoint(ccp(0, 0)); /* 左下角 */
        line->setPosition(ccp(offsetX, 45));
        
        icon_ = CCSprite::create("faces/x-000.png");
        icon_->setAnchorPoint(ccp(1, 1)); /* 右上角 */
        icon_->setPosition(ccp(winSize.width - 45 - 9, winSize.height - 45 - 9));
        
        CCRect iconBoxRect = CCRectMake(0, 0, 114, 114);
        CCScale9Sprite *iconBox = CCScale9Sprite::create("UI/face_box.png", iconBoxRect);
        addChild(iconBox);
        addChild(icon_);
        iconBox->setAnchorPoint(ccp(1, 1)); /* 右上角 */
        iconBox->setPosition(ccp(icon_->getPosition().x + 9, icon_->getPosition().y + 9));
        
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
    pp_->setString(CCString::createWithFormat("%hi/%hi", hero->getPP(), hero->getMaxPP())->getCString());
    
    /* money */
    money_->setString(CCString::createWithFormat("%hi", backpack->getMoney())->getCString());
}
