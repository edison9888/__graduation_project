//
//  MCHiringMercenaryUI.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCHiringMercenaryUI.h"
#include "MCScene.h"
#include "MCMercenaryManager.h"
#include "MCTableViewTextFieldCell.h"
#include "MCBackpack.h"
#include "MCToast.h"

#define kMCValueOffsetX 13

static const float kMCActionDuration = 0.1f;

static const char *kMCBackgroundFilepath = "bg.png";

static const ccColor3B kMCUnselectedColor = ccc3(255, 255, 255);
static const ccColor3B kMCSelectedColor = ccc3(240, 196, 64);

static int kMCNervousMercenaryTag = 0x24;

bool
MCHiringMercenaryUI::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        CCMenu *menu;
        CCLabelTTF *label;
        CCMenuItem *menuItem;
        float fontSize = 32.0f / contentScaleFactor;
        float offsetX = 24.0f / contentScaleFactor;
        
        CCSprite *bg = CCSprite::create(kMCBackgroundFilepath);
        addChild(bg);
        bg->setAnchorPoint(CCPointZero);
        bg->setPosition(CCPointZero);
        
        CCMenuItemImage *backwardMenuItem = CCMenuItemImage::create("back.png", "back_pressed.png");
        backwardMenuItem->setTarget(this, menu_selector(MCHiringMercenaryUI::backward));
        menu = CCMenu::create();
        addChild(menu);
        menu->addChild(backwardMenuItem);
        menu->setPosition(ccp(backwardMenuItem->getContentSize().width / 2 - 16 / contentScaleFactor,
                              winSize.height - backwardMenuItem->getContentSize().height / 2 + 16 / contentScaleFactor));
        
        menu = CCMenu::create();
        addChild(menu);
        
        label = CCLabelTTF::create("通常佣兵", "", fontSize);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCHiringMercenaryUI::changeMercenaryType));
        menuItem->setAnchorPoint(ccp(0, 0.5));
        menu->addChild(menuItem);
        
        label = CCLabelTTF::create("胆小佣兵", "", fontSize);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCHiringMercenaryUI::changeMercenaryType));
        menuItem->setTag(kMCNervousMercenaryTag);
        menuItem->setAnchorPoint(ccp(0, 0.5));
        menu->addChild(menuItem);
        
        menu->alignItemsVerticallyWithPadding(4);
        menu->setAnchorPoint(ccp(0, 0.5));
        menu->setPosition(ccp(4, winSize.height / 2));
        
        /* normal */
        for (unsigned int i = 1; i < 10; ++i) {
            char c_str_m_id[5] = {
                'M',
                '9',
                '0',
                '0' + i,
                0
            };
            JsonBox::Object mercenaryObject;
            
            mercenaryObject["id"] = JsonBox::Value(c_str_m_id);
            mercenaryObject["name"] = JsonBox::Value(MCMercenaryManagerChoiceMercenaryName());
            mercenaries_.push_back(mercenaryObject);
        }
        
        /* nervous */
        for (unsigned int i = 1; i < 10; ++i) {
            char c_str_m_id[5] = {
                'M',
                '8',
                '0',
                '0' + i,
                0
            };
            JsonBox::Object nervousMercenaryObject;
            
            nervousMercenaryObject["id"] = JsonBox::Value(c_str_m_id);
            nervousMercenaryObject["name"] = JsonBox::Value(MCMercenaryManagerChoiceMercenaryName());
            nervousMercenaries_.push_back(nervousMercenaryObject);
        }
        
        content_.push_back(JsonBox::Value("name")); /* 名字后面显示是否是胆小的 */
        content_.push_back(JsonBox::Value("cost"));
        content_.push_back(JsonBox::Value("dexterity"));
        content_.push_back(JsonBox::Value("ac"));
        content_.push_back(JsonBox::Value("armorCheckPenalty"));
        content_.push_back(JsonBox::Value("damage")); /* 包含加成 */
        content_.push_back(JsonBox::Value("criticalHit"));
        content_.push_back(JsonBox::Value("distance"));
        content_.push_back(JsonBox::Value("skills"));
        content_.push_back(JsonBox::Value("effect"));
        
        showNervous_ = false;
        selectedMercenary_ = NULL;
        
        /* 佣兵表格 */
        tableViewSize_ = CCSizeMake(160 / contentScaleFactor,
                                    320 / contentScaleFactor);
        tableView_ = CCTableView::create(this, tableViewSize_);
        addChild(tableView_);
        tableView_->setDirection(kCCScrollViewDirectionVertical);
        tableView_->setDelegate(this);
        tableView_->setVerticalFillOrder(kCCTableViewFillTopDown);
        
        tableView_->setAnchorPoint(ccp(0, 0.5));
        tableView_->setPosition(ccp(menu->getPositionX() + menuItem->getContentSize().width,
                                    (winSize.height - tableViewSize_.height) / 2));
        
        contentTableView_ = CCTableView::create(this, CCSizeMake(240 / contentScaleFactor,
                                                                 360 / contentScaleFactor));
        addChild(contentTableView_);
        contentTableView_->setDirection(kCCScrollViewDirectionVertical);
        contentTableView_->setDelegate(this);
        contentTableView_->setVerticalFillOrder(kCCTableViewFillTopDown);
        
        contentTableView_->setAnchorPoint(ccp(0, 0.5));
        contentTableView_->setPosition(ccp(tableView_->getPositionX() + tableViewSize_.width + 13 / contentScaleFactor,
                                           (winSize.height - tableViewSize_.height) / 2));
        contentTableView_->setVisible(false);
        
        selectedCell_ = NULL;        
        
        /* money */
        label = CCLabelTTF::create("马克", "Marker Felt", fontSize);
        addChild(label);
        label->setAnchorPoint(ccp(1, 0)); /* 右下角 */
        label->setPosition(ccp(winSize.width - 48 / contentScaleFactor, 48 / contentScaleFactor));
        
        money_ = CCLabelTTF::create(CCString::createWithFormat("%d", MCBackpack::sharedBackpack()->getMoney())->getCString(),
                                    "Marker Felt",
                                    fontSize);
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
        
        menu = CCMenu::create();
        addChild(menu);
        hire_ = menu;
        
        label = CCLabelTTF::create("雇佣", "", fontSize);
        menuItem = CCMenuItemLabel::create(label,
                                           this,
                                           menu_selector(MCHiringMercenaryUI::hire_click));
        menu->addChild(menuItem);
        menu->setPosition(ccp(winSize.width - 96 / contentScaleFactor,
                              winSize.height / 2));
        menu->setVisible(false);

        
        return true;
    }
    
    return false;
}

void
MCHiringMercenaryUI::initPosition()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    setAnchorPoint(CCPointZero);
    setPosition(ccp(0, winSize.height));
}

void
MCHiringMercenaryUI::attach(MCScene *aScene)
{
    aScene->addChild(this);
    aScene->pauseScene();
    initPosition();
    
    runAction(CCMoveTo::create(kMCActionDuration, CCPointZero));
}

void
MCHiringMercenaryUI::detach()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    runAction(CCSequence::createWithTwoActions(CCMoveTo::create(kMCActionDuration, ccp(0, winSize.height)),
                                               CCCallFunc::create(this, callfunc_selector(MCHiringMercenaryUI::destroy))));
}

/* CCTableViewDataSource */
CCSize
MCHiringMercenaryUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(96, 48);
}

CCTableViewCell *
MCHiringMercenaryUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    CCString *ccstring;
    
    if (table == tableView_) { /* 佣兵 */
        JsonBox::Object mercenaryObject = showNervous_ ? nervousMercenaries_.at(idx).getObject() : mercenaries_.at(idx).getObject();
        ccstring = CCString::create(mercenaryObject["name"].getString());
    } else {
        std::string field = content_.at(idx).getString();

        MCMercenary *mercenary = selectedMercenary_;

        if (! mercenary) {
            ccstring = CCString::create("");
        } else if (field.compare("name") == 0) {
            JsonBox::Object mercenaryObject = showNervous_
                                                ? nervousMercenaries_.at(selectedCell_->getIdx()).getObject()
                                                : mercenaries_.at(selectedCell_->getIdx()).getObject();
            ccstring = CCString::createWithFormat("名称: %s%s",
                                                  mercenaryObject["name"].getString().c_str(),
                                                  mercenary->getID().sub_class_ == '8' ? "[胆小]" : "");
        } else if (field.compare("cost") == 0) {
            ccstring = CCString::createWithFormat("雇佣费: %hu",
                                                  mercenary->getCost());
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
MCHiringMercenaryUI::numberOfCellsInTableView(CCTableView *table)
{
    return table == tableView_
            ? (showNervous_ ? nervousMercenaries_.size() : mercenaries_.size())
            : content_.size();
}

/* CCTableViewDelegate */
void
MCHiringMercenaryUI::tableCellTouched(CCTableView *table, CCTableViewCell *cell)
{
    if (table != tableView_) {
        return;
    }
    
    if (selectedCell_) {
        selectedCell_->unselected();
    }
    selectedCell_ = dynamic_cast<MCTableViewTextFieldCell *>(cell);
    selectedCell_->selected();
    
    MCMercenaryManager *mercenaryManager = MCMercenaryManager::sharedMercenaryManager();
    const char *c_str_m_id;
    if (showNervous_) {
        JsonBox::Object mercenaryObject = nervousMercenaries_.at(cell->getIdx()).getObject();
        c_str_m_id = mercenaryObject["id"].getString().c_str();
    } else {
        JsonBox::Object mercenaryObject = mercenaries_.at(cell->getIdx()).getObject();
        c_str_m_id = mercenaryObject["id"].getString().c_str();
    }
    mc_object_id_t m_id = {
        c_str_m_id[0],
        c_str_m_id[1],
        c_str_m_id[2],
        c_str_m_id[3]
    };
    selectedMercenary_ = dynamic_cast<MCMercenary *>(mercenaryManager->protoMercenaryForObjectId(m_id));
    
    contentTableView_->reloadData();
    contentTableView_->setVisible(true);
    hire_->setVisible(true);
}

void
MCHiringMercenaryUI::confirmDidClickYesButton(MCConfirm *aConfirm)
{
    unsigned int index = selectedCell_->getIdx();
    JsonBox::Object mercenaryObject = showNervous_
                                        ? nervousMercenaries_.at(index).getObject()
                                        : mercenaries_.at(index).getObject();
    MCMercenaryManager::sharedMercenaryManager()->hire(selectedMercenary_->getID());
    MCMercenary *mercenary = dynamic_cast<MCMercenary *>(MCTeam::sharedTeam()->lastRole());
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    mc_price_t money = backpack->getMoney();
    mc_cost_t cost = mercenary->getCost();
    CCString *name = CCString::create(mercenaryObject["name"].getString());
    float fontSize = 32.0f / CC_CONTENT_SCALE_FACTOR();
    
    backpack->setMoney(money - (mc_price_t) cost);
    mercenary->setName(name);
    name->retain();
    MCToast::make(this, "雇佣成功！")->show();
    
    /* change name */
    JsonBox::Value newName = JsonBox::Value(MCMercenaryManagerChoiceMercenaryName());
    if (showNervous_) {
        JsonBox::Value &v = nervousMercenaries_.at(index);
        mercenaryObject["name"] = newName;
        v.setObject(mercenaryObject);
    } else {
        JsonBox::Value &v = mercenaries_.at(index);
        mercenaryObject["name"] = newName;
        v.setObject(mercenaryObject);
    }
    
    tableView_->reloadData();
    money_->setString(CCString::createWithFormat("%d", backpack->getMoney())->getCString());
}

void
MCHiringMercenaryUI::hire_click(CCObject* aSender)
{
    MCMercenary *mercenary = selectedMercenary_;
    
    if (MCConfirm::hasParent()
        || !mercenary) {
        return;
    }
    MCBackpack *backpack = MCBackpack::sharedBackpack();
    mc_price_t money = backpack->getMoney();
    mc_cost_t cost = mercenary->getCost();
    
    if ((mc_price_t) cost > money) {
        MCToast::make(this, "不够马克！")->show();
    } else if (MCTeam::sharedTeam()->isFull()) {
        MCToast::make(this, "队伍已満，不能继续雇佣！")->show();
    } else {
        MCConfirm::confirm(this,
                           this,
                           CCString::createWithFormat("雇佣需要%hu马克，确定雇佣？", cost)->getCString());
    }
}

void
MCHiringMercenaryUI::changeMercenaryType(CCObject* aSender)
{
    CCMenuItemLabel *menuItem = dynamic_cast<CCMenuItemLabel *>(aSender);
    
    showNervous_ = menuItem->getTag() == kMCNervousMercenaryTag;
    
    if (selectedCell_) {
        selectedCell_->unselected();
    }
    
    tableView_->reloadData();
    contentTableView_->setVisible(false);
    hire_->setVisible(false);
}

/* 关闭.... */
void
MCHiringMercenaryUI::backward(CCObject* aSender)
{
    detach();
}

void
MCHiringMercenaryUI::destroy()
{
    MCScene *scene = dynamic_cast<MCScene *>(m_pParent);
    
    scene->resumeScene();
    removeFromParentAndCleanup(true);
}
