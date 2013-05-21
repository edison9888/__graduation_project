//
//  MCTaskLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCTaskLayer.h"
#include "MCHero.h"
#include "MCTaskManager.h"
#include "MCTaskTarget.h"
#include "MCRoleManager.h"
#include "MCEnemy.h"
#include "MCConfirm.h"

MCTaskLayer::~MCTaskLayer()
{
    CC_SAFE_RELEASE(taskLayer_);
    CC_SAFE_RELEASE(noTaskLayer_);
}

bool
MCTaskLayer::init()
{
    if (MCBaseInfoLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        CCLabelTTF *label;
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        float fontSize = 21 / contentScaleFactor;
        float valueFontSize = 24 / contentScaleFactor;
        float contentHeight = winSize.height - 90 / contentScaleFactor;
        float contentHeightWithoutBottom = contentHeight * 2 / 3;
        float offsetXLeft = 180 / contentScaleFactor;
        float offsetXRight;
        float offsetY = contentHeightWithoutBottom / 4 + 4;
        float offsetYInc = offsetY;
        float valuePositionXLeft;
        float valuePositionXRight;
        
        taskLayer_ = CCLayer::create();
        addChild(taskLayer_);
        noTaskLayer_ = CCLayer::create();
        addChild(noTaskLayer_);

        /* 任务名称 */
        label = CCLabelTTF::create("任务名称", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        valuePositionXLeft = label->getPosition().x + label->getContentSize().width * 2;
        
        name_ = CCLabelTTF::create("任务名称", "Marker Felt", valueFontSize);
        taskLayer_->addChild(name_);
        name_->setAnchorPoint(ccp(0.5, 1));
        name_->setPosition(ccp(valuePositionXLeft,
                               label->getPositionY() - (label->getContentSize().height - name_->getContentSize().height) / 2));
        offsetXRight = name_->getPosition().x + name_->getContentSize().width;
        
        /* 陷阱类型限制 */
        label = CCLabelTTF::create("陷阱类型限制", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXRight, winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        valuePositionXRight = label->getPosition().x + label->getContentSize().width * 3 / 2;
        
        trapType_ = CCLabelTTF::create("陷阱类型限制", "Marker Felt", valueFontSize);
        taskLayer_->addChild(trapType_);
        trapType_->setAnchorPoint(ccp(0.5, 1));
        trapType_->setPosition(ccp(valuePositionXRight,
                                   label->getPositionY() - (label->getContentSize().height - trapType_->getContentSize().height) / 2));
        
        /* 任务押金 */
        label = CCLabelTTF::create("任务押金", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        
        cashPledge_ = CCLabelTTF::create("任务押金", "Marker Felt", valueFontSize);
        taskLayer_->addChild(cashPledge_);
        cashPledge_->setAnchorPoint(ccp(0.5, 1));
        cashPledge_->setPosition(ccp(valuePositionXLeft,
                               label->getPositionY() - (label->getContentSize().height - cashPledge_->getContentSize().height) / 2));
        
        /* 陷阱奖励 */
        label = CCLabelTTF::create("陷阱奖励", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXRight, winSize.height - offsetYInc));
        offsetYInc += label->getContentSize().height;
        
        bonus_ = CCLabelTTF::create("陷阱奖励", "Marker Felt", valueFontSize);
        taskLayer_->addChild(bonus_);
        bonus_->setAnchorPoint(ccp(0.5, 1));
        bonus_->setPosition(ccp(valuePositionXRight,
                                   label->getPositionY() - (label->getContentSize().height - bonus_->getContentSize().height) / 2));
        
        /* 道具限制 */
        CCLabelTTF *header;
        header = CCLabelTTF::create("道具限制", "Marker Felt", fontSize);
        taskLayer_->addChild(header);
        header->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        header->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        /* 陷阱 */
        label = CCLabelTTF::create("大范围陷阱", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        trapWideMaxSize_ = CCLabelTTF::create("大范围陷阱", "Marker Felt", valueFontSize);
        taskLayer_->addChild(trapWideMaxSize_);
        trapWideMaxSize_->setAnchorPoint(ccp(0.5, 1));
        trapWideMaxSize_->setPosition(ccp(label->getPositionX() + label->getContentSize().width * 3 / 2,
                                          label->getPositionY() - (label->getContentSize().height - trapWideMaxSize_->getContentSize().height) / 2));
        
        label = CCLabelTTF::create("大伤害陷阱", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(trapWideMaxSize_->getPositionX() + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        trapDamageMaxSize_ = CCLabelTTF::create("大伤害陷阱", "Marker Felt", valueFontSize);
        taskLayer_->addChild(trapDamageMaxSize_);
        trapDamageMaxSize_->setAnchorPoint(ccp(0.5, 1));
        trapDamageMaxSize_->setPosition(ccp(label->getPositionX() + label->getContentSize().width * 3 / 2,
                                            label->getPositionY() - (label->getContentSize().height - trapDamageMaxSize_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        /* 药水 */
        label = CCLabelTTF::create("生命药水", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(trapWideMaxSize_->getPositionX() + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        healthPotionMaxSize_ = CCLabelTTF::create("生命药水", "Marker Felt", valueFontSize);
        taskLayer_->addChild(healthPotionMaxSize_);
        healthPotionMaxSize_->setAnchorPoint(ccp(0.5, 1));
        healthPotionMaxSize_->setPosition(ccp(trapWideMaxSize_->getPositionX(),
                                              label->getPositionY() - (label->getContentSize().height - healthPotionMaxSize_->getContentSize().height) / 2));
        
        label = CCLabelTTF::create("体力药水", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        physicalPotionMaxSize_ = CCLabelTTF::create("体力药水", "Marker Felt", valueFontSize);
        taskLayer_->addChild(physicalPotionMaxSize_);
        physicalPotionMaxSize_->setAnchorPoint(ccp(0.5, 1));
        physicalPotionMaxSize_->setPosition(ccp(trapDamageMaxSize_->getPositionX(),
                                                label->getPositionY() - (label->getContentSize().height - physicalPotionMaxSize_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        /* 道具赠送 */
        header = CCLabelTTF::create("道具赠送", "Marker Felt", fontSize);
        taskLayer_->addChild(header);
        header->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        header->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        /* 陷阱 */
        label = CCLabelTTF::create("大范围陷阱", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        trapWideBonus_ = CCLabelTTF::create("大范围陷阱", "Marker Felt", valueFontSize);
        taskLayer_->addChild(trapWideBonus_);
        trapWideBonus_->setAnchorPoint(ccp(0.5, 1));
        trapWideBonus_->setPosition(ccp(label->getPositionX() + label->getContentSize().width * 3 / 2,
                                          label->getPositionY() - (label->getContentSize().height - trapWideBonus_->getContentSize().height) / 2));
        
        label = CCLabelTTF::create("大伤害陷阱", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(trapWideMaxSize_->getPositionX() + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        trapDamageBonus_ = CCLabelTTF::create("大伤害陷阱", "Marker Felt", valueFontSize);
        taskLayer_->addChild(trapDamageBonus_);
        trapDamageBonus_->setAnchorPoint(ccp(0.5, 1));
        trapDamageBonus_->setPosition(ccp(label->getPositionX() + label->getContentSize().width * 3 / 2,
                                            label->getPositionY() - (label->getContentSize().height - trapDamageBonus_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        /* 药水 */
        label = CCLabelTTF::create("生命药水", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(trapWideMaxSize_->getPositionX() + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        healthPotionBonus_ = CCLabelTTF::create("生命药水", "Marker Felt", valueFontSize);
        taskLayer_->addChild(healthPotionBonus_);
        healthPotionBonus_->setAnchorPoint(ccp(0.5, 1));
        healthPotionBonus_->setPosition(ccp(trapWideBonus_->getPositionX(),
                                            label->getPositionY() - (label->getContentSize().height - healthPotionBonus_->getContentSize().height) / 2));
        
        label = CCLabelTTF::create("体力药水", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft + header->getContentSize().width * 3 / 2,
                               winSize.height - offsetYInc));
        
        physicalPotionBonus_ = CCLabelTTF::create("体力药水", "Marker Felt", valueFontSize);
        taskLayer_->addChild(physicalPotionBonus_);
        physicalPotionBonus_->setAnchorPoint(ccp(0.5, 1));
        physicalPotionBonus_->setPosition(ccp(trapDamageBonus_->getPositionX(),
                                              label->getPositionY() - (label->getContentSize().height - physicalPotionBonus_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        
        /* 任务地点 */
        label = CCLabelTTF::create("任务地点", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        
        description_ = CCLabelTTF::create("任务地点", "Marker Felt", valueFontSize);
        taskLayer_->addChild(description_);
        description_->setAnchorPoint(ccp(0.5, 1));
        description_->setPosition(ccp(valuePositionXLeft,
                                      label->getPositionY() - (label->getContentSize().height - description_->getContentSize().height) / 2));
        offsetYInc += label->getContentSize().height;
        
        /* 任务目标 */
        label = CCLabelTTF::create("任务目标", "Marker Felt", fontSize);
        taskLayer_->addChild(label);
        label->setAnchorPoint(ccp(0, 1)); /* 左上角 */
        label->setPosition(ccp(offsetXLeft, winSize.height - offsetYInc));
        
        targets_ = CCLabelTTF::create("任务目标", "Marker Felt", valueFontSize);
        taskLayer_->addChild(targets_);
        targets_->setDimensions(CCSizeMake(winSize.width / 2, 0));
        targets_->setHorizontalAlignment(kCCTextAlignmentLeft);
        targets_->setAnchorPoint(ccp(0.5, 1));
        targets_->setPosition(ccp(valuePositionXLeft + label->getContentSize().width * 3 / 2 + 24 / contentScaleFactor,
                                  label->getPositionY() - (label->getContentSize().height - targets_->getContentSize().height) / 2));
        
        /* 放弃任务 */
        CCMenu *menu = CCMenu::create();
        taskLayer_->addChild(menu);
        label = CCLabelTTF::create("放弃任务", "Marker Felt", fontSize);
        CCMenuItemLabel *menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCTaskLayer::abortTask));
        menu->addChild(menuItem);
        menu->setPosition(ccp(winSize.width - label->getContentSize().width,
                              label->getContentSize().height * 3));
        abortTaskButton_ = menu;
        
        /* 木有任务 */
        label = CCLabelTTF::create("目前木有接受任务", "Marker Felt", 48);
        noTaskLayer_->addChild(label);
        label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        
        return true;
    }
    
    return false;
}

MCTaskLayer *
MCTaskLayer::create()
{
    MCTaskLayer *layer = new MCTaskLayer;
    
    if (layer && layer->init()) {
        layer->MCBaseInfoLayer::autorelease();
    } else {
        CC_SAFE_DELETE(layer);
        layer = NULL;
    }
    
    return layer;
}

void
MCTaskLayer::loadData()
{
    MCTask *task = MCTaskManager::sharedTaskManager()->getCurrentTask();
    
    if (task) {
        /* 任务名称 */
        name_->setString(task->getName()->getCString());
        
        /* 陷阱类型限制 */
        trapType_->setString(MCTrapGetNameWithTrapType(task->getTrapType()));
        
        /* 任务押金 */
        cashPledge_->setString(CCString::createWithFormat("%hi马克", task->getCashPledge())->getCString());
        
        /* 任务奖励 */
        bonus_->setString(CCString::createWithFormat("%hi马克", task->getBonus())->getCString());
        
        /* 道具限制 */
        trapWideMaxSize_->setString(CCString::createWithFormat("%hi", task->getTrapWideMaxSize())->getCString());
        trapDamageMaxSize_->setString(CCString::createWithFormat("%hi", task->getTrapDamageMaxSize())->getCString());
        healthPotionMaxSize_->setString(CCString::createWithFormat("%hi", task->getHealthPotionMaxSize())->getCString());
        physicalPotionMaxSize_->setString(CCString::createWithFormat("%hi", task->getPhysicalPotionMaxSize())->getCString());
        
        /* 道具赠送 */
        trapWideBonus_->setString(CCString::createWithFormat("%hi", task->getTrapWideBonus())->getCString());
        trapDamageBonus_->setString(CCString::createWithFormat("%hi", task->getTrapDamageBonus())->getCString());
        healthPotionBonus_->setString(CCString::createWithFormat("%hi", task->getHealthPotionBonus())->getCString());
        physicalPotionBonus_->setString(CCString::createWithFormat("%hi", task->getPhysicalPotionBonus())->getCString());
        
        /* 任务地点 */
        description_->setString(task->getDescription()->getCString());
        
        /* 任务目标 */
        MCRoleManager *roleManager = MCRoleManager::sharedRoleManager();
        CCArray *targets = task->getTargets();
        CCObject *obj;
        MCTaskTarget *target;
        std::string targetsList;
        CCARRAY_FOREACH(targets, obj) {
            target = dynamic_cast<MCTaskTarget *>(obj);
            targetsList.append(CCString::createWithFormat("%s %u/%u  \n",
                                                          roleManager->protoEnemyForObjectId(target->objectID)
                                                                     ->getName()->getCString(),
                                                          target->remaining,
                                                          target->count)->getCString());
        }
        targets_->setString(targetsList.c_str());
        
        /* 放弃任务 */
        MCTaskStatus status = task->getTaskStatus();
        if (status == MCTaskAccepted) {
            abortTaskButton_->setVisible(true);
        } else {
            abortTaskButton_->setVisible(false);
        }
        
        taskLayer_->setVisible(true);
        noTaskLayer_->setVisible(false);
    } else {
        taskLayer_->setVisible(false);
        noTaskLayer_->setVisible(true);
    }
}

void
MCTaskLayer::confirmDidClickYesButton(MCConfirm *aConfirm)
{
    /* 放弃任务 */
    MCTaskManager::sharedTaskManager()->abortCurrentTask();
    loadData();
}

void
MCTaskLayer::abortTask(CCObject *obj)
{
    MCConfirm::confirm(this, this, "确认放弃？");
}
