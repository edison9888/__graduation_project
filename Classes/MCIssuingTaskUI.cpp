//
//  MCIssuingTaskUI.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCIssuingTaskUI.h"
#include "MCScene.h"
#include "MCTaskManager.h"
#include "MCRegionManager.h"
#include "MCRoleManager.h"
#include "MCTaskTarget.h"
#include "MCToast.h"
#include "MCBackpack.h"
#include "MCTableViewTextFieldCell.h"

const char *kMCIssuingTaskUIDidHideNotification = "kMCIssuingTaskUIDidHideNotification";
static const float kMCActionDuration = 0.1f;

static const int kMCTagTaskName = 0x12;

static const char *kMCBackgroundFilepath = "bg.png";

static const ccColor3B kMCUnselectedColor = ccc3(255, 255, 255);
static const ccColor3B kMCSelectedColor = ccc3(240, 196, 64);

enum {
    kMCUnknown = 0,
    kMCAbortLastTask,
    kMCAcceptTask
};

bool
MCIssuingTaskUI::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCMenu *menu;
        CCMenuItem *menuItem;
        CCLabelTTF *label;
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        float fontSize = 18 / contentScaleFactor;
        float valueFontSize = 16 / contentScaleFactor;
        float contentHeight = winSize.height - 90 / contentScaleFactor;
        float contentHeightWithoutBottom = contentHeight * 2 / 3;
        float offsetXLeft = 200 / contentScaleFactor;
        float offsetXRight;
        float offsetY = contentHeightWithoutBottom / 4 + 4;
        float offsetYInc = offsetY;
        float valuePositionXLeft;
        float valuePositionXRight;
        
        CCSprite *bg = CCSprite::create(kMCBackgroundFilepath);
        addChild(bg);
        bg->setAnchorPoint(CCPointZero);
        bg->setPosition(CCPointZero);
        
        CCMenuItemImage *backwardMenuItem = CCMenuItemImage::create("back.png", "back_pressed.png");
        backwardMenuItem->setTarget(this, menu_selector(MCIssuingTaskUI::backward));
        menu = CCMenu::create();
        addChild(menu);
        menu->addChild(backwardMenuItem);
        menu->setPosition(ccp(backwardMenuItem->getContentSize().width / 2 - 16 / contentScaleFactor,
                              winSize.height - backwardMenuItem->getContentSize().height / 2 + 16 / contentScaleFactor));
        
        taskLayer_ = CCLayer::create();
        addChild(taskLayer_);
        
        /* 任务名称 */
        label = CCLabelTTF::create("任务名称", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        valuePositionXLeft = label->getPosition().x + label->getContentSize().width * 2;
        
        name_ = CCLabelTTF::create("任务名称", "", valueFontSize);
        taskLayer_->addChild(name_);
        name_->setAnchorPoint(ccp(0.5, 1));
        name_->setPosition(ccp(valuePositionXLeft,
                               label->getPositionY() - (label->getContentSize().height - name_->getContentSize().height) / 2));
        offsetXRight = name_->getPosition().x + name_->getContentSize().width;
        
        /* 陷阱类型限制 */
        label = CCLabelTTF::create("陷阱类型限制", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXRight, winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        valuePositionXRight = label->getPosition().x + label->getContentSize().width * 3 / 2;
        
        trapType_ = CCLabelTTF::create("陷阱类型限制", "", valueFontSize);
        taskLayer_->addChild(trapType_);
        trapType_->setAnchorPoint(ccp(0.5, 1));
        trapType_->setPosition(ccp(valuePositionXRight,
                                   label->getPositionY() - (label->getContentSize().height - trapType_->getContentSize().height) / 2));
        
        /* 任务押金 */
        label = CCLabelTTF::create("任务押金", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        
        cashPledge_ = CCLabelTTF::create("任务押金", "", valueFontSize);
        taskLayer_->addChild(cashPledge_);
        cashPledge_->setAnchorPoint(ccp(0.5, 1));
        cashPledge_->setPosition(ccp(valuePositionXLeft,
                                     label->getPositionY() - (label->getContentSize().height - cashPledge_->getContentSize().height) / 2));
        
        /* 陷阱奖励 */
        label = CCLabelTTF::create("陷阱奖励", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXRight, winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        
        bonus_ = CCLabelTTF::create("陷阱奖励", "", valueFontSize);
        taskLayer_->addChild(bonus_);
        bonus_->setAnchorPoint(ccp(0.5, 1));
        bonus_->setPosition(ccp(valuePositionXRight,
                                label->getPositionY() - (label->getContentSize().height - bonus_->getContentSize().height) / 2));
        
        /* 道具限制 */
        CCLabelTTF *header;
        header = CCLabelTTF::create("道具限制", "", fontSize);
        taskLayer_->addChild(header);
        header->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        header->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        /* 陷阱 */
        label = CCLabelTTF::create("大范围陷阱", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        trapWideMaxSize_ = CCLabelTTF::create("大范围陷阱", "", valueFontSize);
        taskLayer_->addChild(trapWideMaxSize_);
        trapWideMaxSize_->setAnchorPoint(ccp(0.5, 1));
        trapWideMaxSize_->setPosition(ccp(label->getPositionX() + label->getContentSize().width * 3 / 2,
                                          label->getPositionY() - (label->getContentSize().height - trapWideMaxSize_->getContentSize().height) / 2));
        
        label = CCLabelTTF::create("大伤害陷阱", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(trapWideMaxSize_->getPositionX() + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        trapDamageMaxSize_ = CCLabelTTF::create("大伤害陷阱", "", valueFontSize);
        taskLayer_->addChild(trapDamageMaxSize_);
        trapDamageMaxSize_->setAnchorPoint(ccp(0.5, 1));
        trapDamageMaxSize_->setPosition(ccp(label->getPositionX() + label->getContentSize().width * 3 / 2,
                                            label->getPositionY() - (label->getContentSize().height - trapDamageMaxSize_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        /* 药水 */
        label = CCLabelTTF::create("生命药水", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(trapWideMaxSize_->getPositionX() + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        healthPotionMaxSize_ = CCLabelTTF::create("生命药水", "", valueFontSize);
        taskLayer_->addChild(healthPotionMaxSize_);
        healthPotionMaxSize_->setAnchorPoint(ccp(0.5, 1));
        healthPotionMaxSize_->setPosition(ccp(trapWideMaxSize_->getPositionX(),
                                              label->getPositionY() - (label->getContentSize().height - healthPotionMaxSize_->getContentSize().height) / 2));
        
        label = CCLabelTTF::create("体力药水", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        physicalPotionMaxSize_ = CCLabelTTF::create("体力药水", "", valueFontSize);
        taskLayer_->addChild(physicalPotionMaxSize_);
        physicalPotionMaxSize_->setAnchorPoint(ccp(0.5, 1));
        physicalPotionMaxSize_->setPosition(ccp(trapDamageMaxSize_->getPositionX(),
                                                label->getPositionY() - (label->getContentSize().height - physicalPotionMaxSize_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        /* 道具赠送 */
        header = CCLabelTTF::create("道具赠送", "", fontSize);
        taskLayer_->addChild(header);
        header->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        header->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        /* 陷阱 */
        label = CCLabelTTF::create("大范围陷阱", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        trapWideBonus_ = CCLabelTTF::create("大范围陷阱", "", valueFontSize);
        taskLayer_->addChild(trapWideBonus_);
        trapWideBonus_->setAnchorPoint(ccp(0.5, 1));
        trapWideBonus_->setPosition(ccp(label->getPositionX() + label->getContentSize().width * 3 / 2,
                                        label->getPositionY() - (label->getContentSize().height - trapWideBonus_->getContentSize().height) / 2));
        
        label = CCLabelTTF::create("大伤害陷阱", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(trapWideMaxSize_->getPositionX() + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        trapDamageBonus_ = CCLabelTTF::create("大伤害陷阱", "", valueFontSize);
        taskLayer_->addChild(trapDamageBonus_);
        trapDamageBonus_->setAnchorPoint(ccp(0.5, 1));
        trapDamageBonus_->setPosition(ccp(label->getPositionX() + label->getContentSize().width * 3 / 2,
                                          label->getPositionY() - (label->getContentSize().height - trapDamageBonus_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        /* 药水 */
        label = CCLabelTTF::create("生命药水", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(trapWideMaxSize_->getPositionX() + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        healthPotionBonus_ = CCLabelTTF::create("生命药水", "", valueFontSize);
        taskLayer_->addChild(healthPotionBonus_);
        healthPotionBonus_->setAnchorPoint(ccp(0.5, 1));
        healthPotionBonus_->setPosition(ccp(trapWideBonus_->getPositionX(),
                                            label->getPositionY() - (label->getContentSize().height - healthPotionBonus_->getContentSize().height) / 2));
        
        label = CCLabelTTF::create("体力药水", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        physicalPotionBonus_ = CCLabelTTF::create("体力药水", "", valueFontSize);
        taskLayer_->addChild(physicalPotionBonus_);
        physicalPotionBonus_->setAnchorPoint(ccp(0.5, 1));
        physicalPotionBonus_->setPosition(ccp(trapDamageBonus_->getPositionX(),
                                              label->getPositionY() - (label->getContentSize().height - physicalPotionBonus_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        
        /* 任务地点 */
        label = CCLabelTTF::create("任务地点", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        
        description_ = CCLabelTTF::create("任务地点", "", valueFontSize);
        taskLayer_->addChild(description_);
        description_->setAnchorPoint(ccp(0.5, 1));
        description_->setPosition(ccp(valuePositionXLeft,
                                      label->getPositionY() - (label->getContentSize().height - description_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        
        /* 任务目标 */
        label = CCLabelTTF::create("任务目标", "", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        
        targets_ = CCLabelTTF::create("任务目标任务目标任务目标任务目标任务目标任务目标任务目标任务目标任务目标任务目标", "", valueFontSize);
        taskLayer_->addChild(targets_);
        targets_->setDimensions(CCSizeMake(winSize.width / 2, 0));
        targets_->setHorizontalAlignment(kCCTextAlignmentLeft);
        targets_->setAnchorPoint(ccp(0.5, 1));
        targets_->setPosition(ccp(valuePositionXLeft + label->getContentSize().width * 3 / 2 + 24 / contentScaleFactor,
                                  label->getPositionY()));
        
        /* 接受任务 */
        menu = CCMenu::create();
        taskLayer_->addChild(menu);
        label = CCLabelTTF::create("接受任务", "", fontSize);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCIssuingTaskUI::acceptTask_click));
        menu->addChild(menuItem);
        menu->setPosition(ccp(winSize.width - label->getContentSize().width,
                              label->getContentSize().height * 3));
        
        tableViewSize_ = CCSizeMake(180 / contentScaleFactor,
                                    320 / contentScaleFactor);
        tableView_ = CCTableView::create(this, tableViewSize_);
        addChild(tableView_);
        tableView_->setDirection(kCCScrollViewDirectionVertical);
        tableView_->setDelegate(this);
        tableView_->setVerticalFillOrder(kCCTableViewFillTopDown);
        tableView_->reloadData();
        selectedCell_ = NULL;
        
        tasks_ = NULL;
        currentTask_ = NULL;
        confirmType_ = kMCUnknown;
        lastSelectedRegionMenuItem_ = NULL;
        lastSelectedTaskMenuItem_ = NULL;
        taskLayer_->setVisible(false);
        
        return true;
    }
    
    return false;
}

void
MCIssuingTaskUI::initPosition()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    setAnchorPoint(CCPointZero);
    setPosition(ccp(0, winSize.height));
}

void
MCIssuingTaskUI::attach(MCScene *aScene)
{
    generate();
    aScene->addChild(this);
    aScene->pauseScene();
    initPosition();
    
    runAction(CCMoveTo::create(kMCActionDuration, CCPointZero));
}

void
MCIssuingTaskUI::detach()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    runAction(CCSequence::createWithTwoActions(CCMoveTo::create(kMCActionDuration, ccp(0, winSize.height)),
                                               CCCallFunc::create(this, callfunc_selector(MCIssuingTaskUI::destroy))));
}

/* 点击显示该区域的任务列表 */
void
MCIssuingTaskUI::region_clicked(CCObject* aSender)
{
    CCMenuItemLabel *menuItem = dynamic_cast<CCMenuItemLabel *>(aSender);
    
    if (lastSelectedRegionMenuItem_) {
        dynamic_cast<CCLabelTTF *>(lastSelectedRegionMenuItem_->getLabel())->setColor(kMCUnselectedColor);
    }
    dynamic_cast<CCLabelTTF *>(menuItem->getLabel())->setColor(kMCSelectedColor);
    lastSelectedRegionMenuItem_ = menuItem;
    
    tasks_ = dynamic_cast<CCArray *>(menuItem->getUserObject());
    tableView_->reloadData();
    if (selectedCell_) {
        selectedCell_->unselected();
    }
    taskLayer_->setVisible(false);
}

/* 关闭.... */
void
MCIssuingTaskUI::backward(CCObject* aSender)
{
    detach();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCIssuingTaskUIDidHideNotification);
}

void
MCIssuingTaskUI::acceptTask_click(CCObject* aSender)
{
    MCTaskManager *taskManager = MCTaskManager::sharedTaskManager();
    if (taskManager->getCurrentTask()) {
        confirmType_ = kMCAbortLastTask;
        MCConfirm::confirm(this, this, "当前已接受任务，需要放弃并接受此新任务吗？");
    } else {
        confirmType_ = kMCAcceptTask;
        MCConfirm::confirm(this, this, "确定接受任务？");
    }
}

void
MCIssuingTaskUI::confirmDidClickYesButton(MCConfirm *aConfirm)
{
    MCTaskManager *taskManager = MCTaskManager::sharedTaskManager();
    if (confirmType_ == kMCAbortLastTask) {
        taskManager->abortCurrentTask();
    }
    if (currentTask_) {
        int result = taskManager->acceptTask(currentTask_);

        if (result == kMCHandleSucceed) {
            MCToast::make(this, "已接受新任务")->show();
        }
        else if (result == kMCNotEnoughMoney) {
            MCToast::make(this, "兄台，你不够钱~")->show();
        }
    }
}

/* CCTableViewDataSource */
CCSize
MCIssuingTaskUI::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(96, 32);
}

CCTableViewCell *
MCIssuingTaskUI::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    MCTask *task = dynamic_cast<MCTask *>(tasks_->objectAtIndex(idx));
    CCString *ccstring;
    if (task->getTaskStatus() == MCTaskDone) {
        ccstring = CCString::createWithFormat("%2d. %s[已完成]", idx + 1, task->getName()->getCString());
    } else {
        ccstring = CCString::createWithFormat("%2d. %s", idx + 1, task->getName()->getCString());
    }
    
    CCTableViewCell *cell = table->dequeueCell();
    if (! cell) {
        cell = MCTableViewTextFieldCell::create(ccstring->getCString(), "Helvetica", 18.0f);
    } else {
        dynamic_cast<MCTableViewTextFieldCell *>(cell)->setString(ccstring->getCString());
    }
    
    
    return cell;
}

unsigned int
MCIssuingTaskUI::numberOfCellsInTableView(CCTableView *table)
{
    return tasks_ ? tasks_->count() : 0;
}

/* CCTableViewDelegate */
void
MCIssuingTaskUI::tableCellTouched(CCTableView *table, CCTableViewCell *cell)
{
    if (selectedCell_) {
        selectedCell_->unselected();
    }
    selectedCell_ = dynamic_cast<MCTableViewTextFieldCell *>(cell);
    selectedCell_->selected();
    
    loadTask(dynamic_cast<MCTask *>(tasks_->objectAtIndex(cell->getIdx())));
}

/* 生成任务列表 */
void
MCIssuingTaskUI::generate()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    MCTaskManager *taskManager = MCTaskManager::sharedTaskManager();
    CCLabelTTF *label;
    CCMenu *menu;
    CCMenuItem *menuItem;
    MCRegion *region;
    CCArray *tasks;
    MCRegionManager *regionManager = MCRegionManager::sharedRegionManager();
    float menuLabelFontSize = 27;
    
    /* regions */
    menu = CCMenu::create();
    addChild(menu);
    if ((sceneRegion_ & MCForest) == MCForest) {
        region = regionManager->forestRegion();
        label = CCLabelTTF::create(region->getName()->getCString(), "", menuLabelFontSize);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCIssuingTaskUI::region_clicked));
        menuItem->setUserData(region); /* region和task获取到的都是原始数据，不是复制的，用userdata比较好，没有被retain */
        menu->addChild(menuItem);
        
        tasks = taskManager->tasksForRegion(region);
        menuItem->setUserObject(tasks);
        tasks->release();
    }
    if ((sceneRegion_ & MCDesert) == MCDesert) {
        region = regionManager->desertRegion();
        label = CCLabelTTF::create(region->getName()->getCString(), "", menuLabelFontSize);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCIssuingTaskUI::region_clicked));
        menuItem->setUserData(region);
        menu->addChild(menuItem);
        
        tasks = taskManager->tasksForRegion(region);
        menuItem->setUserObject(tasks);
        tasks->release();
    }
    if ((sceneRegion_ & MCGlacier) == MCGlacier) {
        region = regionManager->glacierRegion();
        label = CCLabelTTF::create(region->getName()->getCString(), "", menuLabelFontSize);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCIssuingTaskUI::region_clicked));
        menuItem->setUserData(region);
        menu->addChild(menuItem);
        
        tasks = taskManager->tasksForRegion(region);
        menuItem->setUserObject(tasks);
        tasks->release();
    }
    if ((sceneRegion_ & MCDungeon) == MCDungeon) {
        region = regionManager->dungeonRegion();
        label = CCLabelTTF::create(region->getName()->getCString(), "", menuLabelFontSize);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCIssuingTaskUI::region_clicked));
        menuItem->setUserData(region);
        menu->addChild(menuItem);
        
        tasks = taskManager->tasksForRegion(region);
        menuItem->setUserObject(tasks);
        tasks->release();
    }
    
    if (menu->getChildrenCount() > 0) {
        menu->alignItemsVertically();
        menu->setPosition(ccp(menuItem->getContentSize().width, winSize.height / 2));
        
        tableView_->setPosition(ccp(menu->getPositionX() + menuItem->getContentSize().width - 24 / CC_CONTENT_SCALE_FACTOR(),
                                    (winSize.height - tableViewSize_.height) / 2));
    }
}

void
MCIssuingTaskUI::destroy()
{
    MCScene *scene = dynamic_cast<MCScene *>(m_pParent);
    
    scene->resumeScene();
    removeFromParentAndCleanup(true);
}

void
MCIssuingTaskUI::loadTask(MCTask *aTask)
{
    currentTask_ = aTask;
    if (aTask) {
        /* 任务名称 */
        name_->setString(aTask->getName()->getCString());
        
        /* 陷阱类型限制 */
        trapType_->setString(MCTrapGetNameWithTrapType(aTask->getTrapType()));
        
        /* 任务押金 */
        cashPledge_->setString(CCString::createWithFormat("%hi马克", aTask->getCashPledge())->getCString());
        
        /* 任务奖励 */
        bonus_->setString(CCString::createWithFormat("%hi马克", aTask->getBonus())->getCString());
        
        /* 道具限制 */
        trapWideMaxSize_->setString(CCString::createWithFormat("%hi", aTask->getTrapWideMaxSize())->getCString());
        trapDamageMaxSize_->setString(CCString::createWithFormat("%hi", aTask->getTrapDamageMaxSize())->getCString());
        healthPotionMaxSize_->setString(CCString::createWithFormat("%hi", aTask->getHealthPotionMaxSize())->getCString());
        physicalPotionMaxSize_->setString(CCString::createWithFormat("%hi", aTask->getPhysicalPotionMaxSize())->getCString());
        
        /* 道具赠送 */
        trapWideBonus_->setString(CCString::createWithFormat("%hi", aTask->getTrapWideBonus())->getCString());
        trapDamageBonus_->setString(CCString::createWithFormat("%hi", aTask->getTrapDamageBonus())->getCString());
        healthPotionBonus_->setString(CCString::createWithFormat("%hi", aTask->getHealthPotionBonus())->getCString());
        physicalPotionBonus_->setString(CCString::createWithFormat("%hi", aTask->getPhysicalPotionBonus())->getCString());
        
        /* 任务地点 */
        description_->setString(aTask->getDescription()->getCString());
        
        /* 任务目标 */
        MCRoleManager *roleManager = MCRoleManager::sharedRoleManager();
        CCArray *targets = aTask->getTargets();
        CCObject *obj;
        MCTaskTarget *target;
        std::string targetsList;
        CCARRAY_FOREACH(targets, obj) {
            target = dynamic_cast<MCTaskTarget *>(obj);
            targetsList.append(CCString::createWithFormat("%s %u \n",
                                                          roleManager->protoEnemyForObjectId(target->objectID)
                                                          ->getName()->getCString(),
                                                          target->count)->getCString());
        }
        targets_->setString(targetsList.c_str());
        
        taskLayer_->setVisible(true);
    } else {
        taskLayer_->setVisible(false);
    }
}
