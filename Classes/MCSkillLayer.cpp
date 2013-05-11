//
//  MCSkillLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "JsonBox.h"
#include "MCHero.h"
#include "MCSkillLayer.h"
#include "MCSkillManager.h"
#include "MCSkill.h"
#include "MCTableViewTextFieldCell.h"

#include <cocos-ext.h>
USING_NS_CC_EXT;

bool
MCSkillLayer::init()
{
    if (MCBaseInfoLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        CCMenu *menu;
        CCLabelTTF *label;
        CCMenuItem *menuItem;
        float fontSize = 24.0f / contentScaleFactor;
        float offsetXLeft = 200 / contentScaleFactor;
        
        content_.push_back(JsonBox::Value("name"));
        
        content_.push_back(JsonBox::Value("releaseTime"));
        content_.push_back(JsonBox::Value("activeTime"));
        content_.push_back(JsonBox::Value("coldTime"));
        
        content_.push_back(JsonBox::Value("consume"));
        content_.push_back(JsonBox::Value("requiredProficiency"));
        
        content_.push_back(JsonBox::Value("revise"));
        content_.push_back(JsonBox::Value("count"));
        
        content_.push_back(JsonBox::Value("effect"));
        
        selectedSkill_ = NULL;
        
        /* 技能种类 */
        menu = CCMenu::create();
        
        label = CCLabelTTF::create("剑", "", fontSize);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCSkillLayer::selectSkillType));
        menuItem->setTag(MCSwordSkill);
        menu->addChild(menuItem);
        
        label = CCLabelTTF::create("锤", "", fontSize);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCSkillLayer::selectSkillType));
        menuItem->setTag(MCHammerSkill);
        menu->addChild(menuItem);
        
        label = CCLabelTTF::create("斧", "", fontSize);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCSkillLayer::selectSkillType));
        menuItem->setTag(MCAxeSkill);
        menu->addChild(menuItem);
        
        label = CCLabelTTF::create("枪", "", fontSize);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCSkillLayer::selectSkillType));
        menuItem->setTag(MCSpearSkill);
        menu->addChild(menuItem);
        
        label = CCLabelTTF::create("弓", "", fontSize);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCSkillLayer::selectSkillType));
        menuItem->setTag(MCBowSkill);
        menu->addChild(menuItem);
        
        menu->alignItemsVerticallyWithPadding(4);
        addChild(menu);
        
        menu->setAnchorPoint(ccp(0, 0.5));
        menu->setPosition(ccp(offsetXLeft, winSize.height / 2));
        
        /* 技能表格 */
        tableViewSize_ = CCSizeMake(180 / contentScaleFactor,
                                    320 / contentScaleFactor);
        tableView_ = CCTableView::create(this, tableViewSize_);
        addChild(tableView_);
        tableView_->setDirection(kCCScrollViewDirectionVertical);
        tableView_->setDelegate(this);
        tableView_->setVerticalFillOrder(kCCTableViewFillTopDown);
        
        tableView_->setAnchorPoint(ccp(0, 0.5));
        tableView_->setPosition(ccp(menu->getPositionX() + menuItem->getContentSize().width,
                                    (winSize.height - tableViewSize_.height) / 2));
        
        contentTableView_ = CCTableView::create(this, tableViewSize_);
        addChild(contentTableView_);
        contentTableView_->setDirection(kCCScrollViewDirectionVertical);
        contentTableView_->setDelegate(this);
        contentTableView_->setVerticalFillOrder(kCCTableViewFillTopDown);
        
        contentTableView_->setAnchorPoint(ccp(0, 0.5));
        contentTableView_->setPosition(ccp(tableView_->getPositionX() + tableViewSize_.width,
                                           (winSize.height - tableViewSize_.height) / 2));
        contentTableView_->setVisible(false);
        
        icon_ = CCSprite::create();
        addChild(icon_);
        icon_->setAnchorPoint(ccp(1, 1));
        icon_->setPosition(ccp(winSize.width - 64.0f / contentScaleFactor,
                               winSize.height - 64.0f / contentScaleFactor));
        
        selectedCell_ = NULL;
        selectedSkill_ = NULL;
        
        return true;
    }
    
    return false;
}


/* CCTableViewDataSource */
CCSize
MCSkillLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(400, 32);
}

CCTableViewCell *
MCSkillLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    CCString *ccstring;
    MCSkill *skill;
    
    if (table == tableView_) { /* 技能 */
        skill = dynamic_cast<MCSkill *>(skills_->objectAtIndex(idx));
        if (skill) {
            ccstring = CCString::createWithFormat("名称: %s",
                                                  skill->getName()->getCString());
        } else {
            ccstring = CCString::create("");
        }
    } else {
        std::string field = content_.at(idx).getString();
        
        skill = selectedSkill_;
        if (! skill) {
            ccstring = CCString::create("");
        } else if (field.compare("name") == 0) {
            ccstring = CCString::createWithFormat("名称: %s",
                                                  skill->getName()->getCString());
        } else if (field.compare("releaseTime") == 0) {
            ccstring = CCString::createWithFormat("释放时间: %.1fs",
                                                  skill->releaseTime);
        } else if (field.compare("activeTime") == 0) {
            ccstring = CCString::createWithFormat("完成时间: %.1fs",
                                                  skill->activeTime);
        } else if (field.compare("coldTime") == 0) {
            ccstring = CCString::createWithFormat("冷却时间: %.1fs",
                                                  skill->coldTime);
        } else if (field.compare("consume") == 0) {
            ccstring = CCString::createWithFormat("体力消耗: %.0f",
                                                  skill->consume);
        } else if (field.compare("requiredProficiency") == 0) {
            ccstring = CCString::createWithFormat("需要熟练度: %hu",
                                                  skill->requiredProficiency);
        } else if (field.compare("revise") == 0) {
            ccstring = CCString::createWithFormat("伤害调整: %.1f",
                                                  skill->revise);
        } else if (field.compare("count") == 0) {
            ccstring = CCString::createWithFormat("释放次数: %",
                                                  skill->count);
        } else if (field.compare("effect") == 0) {
            ccstring = CCString::createWithFormat("附带效果: %s",
                                                  skill->effect != MCNormalState ? MCRoleStateGetName(skill->effect) : "无");
        } else {
            ccstring = CCString::create("");
        }
    }
    
    if (! cell) {
        cell = MCTableViewTextFieldCell::create(ccstring->getCString(),
                                                "Helvetica",
                                                21.0f / CC_CONTENT_SCALE_FACTOR());
    } else {
        dynamic_cast<MCTableViewTextFieldCell *>(cell)->setString(ccstring->getCString());
    }
    
    
    return cell;
}

unsigned int
MCSkillLayer::numberOfCellsInTableView(CCTableView *table)
{
    if (table == tableView_) { /* 技能 */
        return skills_ ? skills_->count() : 0;
    }
    
    return content_.size();
}

/* CCTableViewDelegate */
void
MCSkillLayer::tableCellTouched(CCTableView *table, CCTableViewCell *cell)
{
    if (table != tableView_) {
        return;
    }
    
    if (selectedCell_) {
        selectedCell_->unselected();
    }
    selectedCell_ = dynamic_cast<MCTableViewTextFieldCell *>(cell);
    selectedCell_->selected();
    
    selectedSkill_ = dynamic_cast<MCSkill *>(skills_->objectAtIndex(cell->getIdx()));
    
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *frame = cache->spriteFrameByName(selectedSkill_->getName()->getCString());
    if (! frame) {
        frame = CCSpriteFrame::create(selectedSkill_->getIcon()->getCString(),
                                      CCRectMake(0, 0, 64 / CC_CONTENT_SCALE_FACTOR(), 64 / CC_CONTENT_SCALE_FACTOR()));
        cache->addSpriteFrame(frame, selectedSkill_->getName()->getCString());
    }
    icon_->setDisplayFrame(frame);
    icon_->setVisible(true);
    
    contentTableView_->reloadData();
    contentTableView_->setVisible(true);
}

void
MCSkillLayer::loadData()
{
    icon_->setVisible(false);
    contentTableView_->setVisible(false);
}

void
MCSkillLayer::selectSkillType(CCObject *anObject)
{
    CCMenuItemLabel *menuItem = dynamic_cast<CCMenuItemLabel *>(anObject);
    MCSkillType skillType = (MCSkillType) menuItem->getTag();
    
    icon_->setVisible(false);
    contentTableView_->setVisible(false);
    skills_ = MCSkillManager::sharedSkillManager()->skillsForSkillType(skillType);
    tableView_->reloadData();
}
