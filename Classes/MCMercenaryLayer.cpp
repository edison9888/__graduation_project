//
//  MCMercenaryLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCHero.h"
#include "MCMercenaryLayer.h"
#include "MCTeam.h"
#include "MCMercenary.h"
#include "MCMercenaryManager.h"
#include "MCTableViewTextFieldCell.h"
#include "MCToast.h"

#include <cocos-ext.h>
USING_NS_CC_EXT;

bool
MCMercenaryLayer::init()
{
    if (MCBaseInfoLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        float offsetXLeft = 200 / contentScaleFactor;
        float fontSize = 24.0f / contentScaleFactor;
        
        content_.push_back(JsonBox::Value("name")); /* 名字后面显示是否是胆小的 */
        content_.push_back(JsonBox::Value("dexterity"));
        content_.push_back(JsonBox::Value("ac"));
        content_.push_back(JsonBox::Value("armorCheckPenalty"));
        content_.push_back(JsonBox::Value("damage")); /* 包含加成 */
        content_.push_back(JsonBox::Value("criticalHit"));
        content_.push_back(JsonBox::Value("distance"));
        content_.push_back(JsonBox::Value("skills"));
        content_.push_back(JsonBox::Value("effect"));
        
        teamMenu_ = CCMenu::create();
        addChild(teamMenu_);
        
        teamMenu_->setAnchorPoint(ccp(0, 0.5f));
        teamMenu_->setPosition(ccp(offsetXLeft, winSize.height / 2));
        
        dismiss_ = CCMenu::create();
        addChild(dismiss_);
        dismiss_->addChild(CCMenuItemLabel::create(CCLabelTTF::create("解雇", "", fontSize),
                                                   this,
                                                   menu_selector(MCMercenaryLayer::dismiss_click)));
        dismiss_->setAnchorPoint(ccp(0, 0.5f));
        dismiss_->setPosition(ccp(winSize.width - offsetXLeft, winSize.height / 2));
        
        /* 佣兵表格 */
        tableView_ = CCTableView::create(this, CCSizeMake(180 / contentScaleFactor,
                                                          240 / contentScaleFactor));
        addChild(tableView_);
        tableView_->setDirection(kCCScrollViewDirectionVertical);
        tableView_->setDelegate(this);
        tableView_->setVerticalFillOrder(kCCTableViewFillTopDown);
        
        tableView_->setAnchorPoint(ccp(0, 0.5));
        
        return true;
    }
    
    return false;
}

/* CCTableViewDataSource */
CCSize
MCMercenaryLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(96, 32);
}

CCTableViewCell *
MCMercenaryLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    CCString *ccstring;
    
    std::string field = content_.at(idx).getString();
    
    MCMercenary *mercenary = selectedMercenary_;
    
    if (! mercenary) {
        ccstring = CCString::create("");
    } else if (field.compare("name") == 0) {
        ccstring = CCString::createWithFormat("名称: %s%s",
                                              mercenary->getName()->getCString(),
                                              mercenary->getID().sub_class_ == '8' ? "[胆小]" : "");
    } else if (field.compare("dexterity") == 0) {
        ccstring = CCString::createWithFormat("敏捷: %hi",
                                              mercenary->getDexterity());
    } else if (field.compare("ac") == 0) {
        ccstring = CCString::createWithFormat("防御等级: %hi",
                                              mercenary->getAC());
    } else if (field.compare("armorCheckPenalty") == 0) {
        ccstring = CCString::createWithFormat("防具检定减值: %hi",
                                              mercenary->getArmorCheckPenalty());
    } else if (field.compare("damage") == 0) {
        MCDice *damage = mercenary->getDamage();
        mc_bonus_t damageBonus = mercenary->getDamageBonus();
        ccstring = CCString::createWithFormat("伤害值: %hiD%hi%s",
                                              damage->count(),
                                              damage->size(),
                                              damageBonus > 0
                                              ? CCString::createWithFormat("+%hi", damageBonus)->getCString()
                                              : "");
    } else if (field.compare("criticalHit") == 0) {
        ccstring = CCString::createWithFormat("重击倍数: %.1f",
                                              mercenary->getCriticalHit());
    } else if (field.compare("distance") == 0) {
        ccstring = CCString::createWithFormat("攻击距离: %hu",
                                              mercenary->getDistance());
    } else if (field.compare("skills") == 0) {
        CCArray *skills = mercenary->getSkills();
        CCObject *obj;
        MCSkill *skill;
        std::string strSkills;
        
        CCARRAY_FOREACH(skills, obj) {
            skill = dynamic_cast<MCSkill *>(obj);
            strSkills.append(skill->getName()->getCString());
            strSkills.append(",");
        }
        if (strSkills.size() > 0) {
            strSkills.erase(strSkills.end() - 1);
        }
        ccstring = CCString::createWithFormat("技能: %s", strSkills.c_str());
    } else if (field.compare("effect") == 0) {
        MCRoleState state = mercenary->getEffect();
        ccstring = CCString::createWithFormat("附带效果: %s",
                                              state != MCNormalState ? MCRoleStateGetName(state) : "无");
    } else {
        ccstring = CCString::create("");
    }
    
    if (! cell) {
        cell = MCTableViewTextFieldCell::create(ccstring->getCString(),
                                                "Helvetica",
                                                table == tableView_
                                                ? 32.0f / CC_CONTENT_SCALE_FACTOR()
                                                : 21.0f / CC_CONTENT_SCALE_FACTOR());
    } else {
        dynamic_cast<MCTableViewTextFieldCell *>(cell)->setString(ccstring->getCString());
    }
    
    return cell;
}

unsigned int
MCMercenaryLayer::numberOfCellsInTableView(CCTableView *table)
{
    return content_.size();
}

void
MCMercenaryLayer::confirmDidClickYesButton(MCConfirm *aConfirm)
{
    if (selectedMercenary_) {
        MCMercenaryManager::sharedMercenaryManager()->dismiss(selectedMercenary_);
        selectedMercenary_ = NULL;
        loadData();
        MCToast::make(this, "已解雇！")->show();
    }
}

void
MCMercenaryLayer::loadData()
{
    CCMenu *menu = teamMenu_;
    CCLabelTTF *label;
    CCMenuItem *menuItem = NULL;
    float fontSize = 24.0f / CC_CONTENT_SCALE_FACTOR();
    CCObject *obj;
    CCArray *team = MCTeam::sharedTeam()->getRoles();
    MCRole *role;
    
    teamMenu_->removeAllChildren();
    CCARRAY_FOREACH(team, obj) {
        role = dynamic_cast<MCRole *>(obj);
        if (role->isHero()) {
            continue;
        }
        label = CCLabelTTF::create(role->getName()->getCString(), "", fontSize);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCMercenaryLayer::mercenary_click));
        menuItem->setUserData(role);
        menu->addChild(menuItem);
    }
    menu->alignItemsVerticallyWithPadding(4);
    
    if (menuItem != NULL) {
        tableView_->setPosition(ccp(menu->getPositionX() + menuItem->getContentSize().width + 13 / CC_CONTENT_SCALE_FACTOR(),
                                    (CCDirectorGetWindowsSize().height - tableView_->getContentSize().height) / 2));
    }
    
    tableView_->setVisible(false);
    dismiss_->setVisible(false);
}

void
MCMercenaryLayer::mercenary_click(CCObject *anObject)
{
    MCMercenary *mercenary = (MCMercenary *) dynamic_cast<CCMenuItemLabel *>(anObject)->getUserData();
    selectedMercenary_ = mercenary;
    tableView_->reloadData();
    tableView_->setVisible(true);
    dismiss_->setVisible(true);
}


void
MCMercenaryLayer::dismiss_click(CCObject *anObject)
{
    MCConfirm::confirm(this, this, "确定解雇?");
}
