//
//  MCSkillBar.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCSkillBar.h"
#include "MCRole.h"
#include "MCEquipmentManager.h"
#include "MCSkillManager.h"
#include "MCMercenary.h"
#include "MCTeam.h"

/* extern */
const GLubyte kMCSkillBarItemColdTimeOpacity     = 196;
const GLubyte kMCSkillBarItemPassiveSkillOpacity = 128;
const GLubyte kMCSkillBarItemActiveSkillOpacity  = 255;

const char *kMCSkillBarVisibleDidChangeNotification = "kMCSkillBarVisibleDidChangeNotification";

static const char *kMCColdTimeSkillIconFilepath = "icons/m-000.png";
static const char *kMCInvalidSkillIconFilepath = "icons/x-000.png";

static const char *kMCSkillBarItemOpenedFilepath = "UI/abi_closed.png";
static const char *kMCSkillBarItemOpenedSelectedFilepath = "UI/abi_closed_selected.png";
static const char *kMCSkillBarItemClosedFilepath = "UI/abi_opened.png";
static const char *kMCSkillBarItemClosedSelectedFilepath = "UI/abi_opened_selected.png";

static const float kMCActionDuration = 0.2f;

#pragma mark *** MCSkillBarItem ***

MCSkillBarItem::~MCSkillBarItem()
{
    CC_SAFE_RELEASE(skill_);
}

bool
MCSkillBarItem::init(MCSkill *aSkill)
{
    CCString *icon = aSkill ? aSkill->getIcon() : NULL;
    
    if (CCSprite::initWithFile(icon ? icon->getCString() : kMCInvalidSkillIconFilepath)) {
        CCPoint anchorPoint = ccp(0.5f, 0.0f);
        
        pp_ = CCLabelTTF::create(CCString::createWithFormat("%.0f", aSkill->consume)->getCString(),
                                 "",
                                 24);
        addChild(pp_);
        pp_->setAnchorPoint(anchorPoint);
        pp_->setColor(ccc3(32, 32, 32));
        pp_->setPosition(ccp(this->getContentSize().width / 2, 2));
        
        if (aSkill->triggerType == MCSkill::MCPassiveSkill) {
            setOpacity(kMCSkillBarItemPassiveSkillOpacity);
        }
    
        skill_ = aSkill;
        isSelected_ = false;
        
        return true;
    }
    
    return false;
}

MCSkillBarItem *
MCSkillBarItem::create(MCSkill *aSkill)
{
    MCSkillBarItem *item = new MCSkillBarItem;
    
    if (item && item->init(aSkill)) {
        item->autorelease();
    } else {
        CC_SAFE_DELETE(item);
        item = NULL;
    }
    
    return item;
}

void
MCSkillBarItem::intoColdTime()
{
    CCSprite *coldSprite = CCSprite::create(kMCColdTimeSkillIconFilepath);
    CCProgressTimer *progressTimer = CCProgressTimer::create(coldSprite);
    
    progressTimer->setReverseProgress(true);
    progressTimer->setAnchorPoint(CCPointZero);
    addChild(progressTimer);
    
    setOpacity(kMCSkillBarItemColdTimeOpacity);
    progressTimer->runAction(CCSequence::createWithTwoActions(CCProgressFromTo::create(skill_->coldTime, 100.f, 0.0f),
                                                              CCCallFuncO::create(this,
                                                                                  callfuncO_selector(MCSkillBarItem::coldTimeDidFinish),
                                                                                  progressTimer)));
}

bool
MCSkillBarItem::inColdTime()
{
    if (skill_->lastUsedTime.tv_sec == 0
        && skill_->lastUsedTime.tv_usec == 0) {
        return false;
    }
    
    struct cc_timeval now;
    
    CCTime::gettimeofdayCocos2d(&now, NULL);
    double elapsed = CCTime::timersubCocos2d(&skill_->lastUsedTime, &now);
    
    return elapsed < (double) skill_->coldTime * 1000;
}

bool
MCSkillBarItem::selectable()
{
    return skill_
            && skill_->triggerType == MCSkill::MCActiveSkill
            && !inColdTime();
}

void
MCSkillBarItem::coldTimeDidFinish(CCObject *anObject)
{
    CCProgressTimer *progressTimer = dynamic_cast<CCProgressTimer *>(anObject);
    
    progressTimer->removeFromParentAndCleanup(true);
    setOpacity(kMCSkillBarItemActiveSkillOpacity);
}

#pragma mark -
#pragma mark *** MCSkillBarItemGroup ***

MCSkillBarItemGroup::~MCSkillBarItemGroup()
{
    CC_SAFE_RELEASE(skillBarItemA_);
    CC_SAFE_RELEASE(skillBarItemB_);
    CC_SAFE_RELEASE(skillBarItemC_);
    CC_SAFE_RELEASE(skillBarItemD_);
}

void
MCSkillBarItemGroup::init(MCRole *aRole)
{
    MCSkill *skillA = NULL;
    MCSkill *skillB = NULL;
    MCSkill *skillC = NULL;
    MCSkill *skillD = NULL;
    CCArray *skills;
    mc_size_t count;
    
    skills = aRole->getSkills();
    
    count = skills->count();
    /* 主角和佣兵都至少有1个技能 */
    /* skill A */
    skillA = dynamic_cast<MCSkill *>(dynamic_cast<MCSkill *>(skills->objectAtIndex(0))->copy());
    
    /* skill B */
    if (count > 1) {
        skillB = dynamic_cast<MCSkill *>(dynamic_cast<MCSkill *>(skills->objectAtIndex(1))->copy());
    }
    /* skill C */
    if (count > 2) {
        skillC = dynamic_cast<MCSkill *>(dynamic_cast<MCSkill *>(skills->objectAtIndex(2))->copy());
    }
    /* skill D */
    if (count > 3) {
        skillD = dynamic_cast<MCSkill *>(dynamic_cast<MCSkill *>(skills->objectAtIndex(3))->copy());
    }
    
    if (skillA && skillA->masteredForRole(aRole)) {
        skillBarItemA_ = new MCSkillBarItem;
        skillBarItemA_->init(skillA);
    } else {
        skillBarItemA_ = NULL;
    }
    if (skillB && skillB->masteredForRole(aRole)) {
        skillBarItemB_ = new MCSkillBarItem;
        skillBarItemB_->init(skillB);
    } else {
        skillBarItemB_ = NULL;
    }
    if (skillC && skillC->masteredForRole(aRole)) {
        skillBarItemC_ = new MCSkillBarItem;
        skillBarItemC_->init(skillC);
    } else {
        skillBarItemC_ = NULL;
    }
    if (skillD && skillD->masteredForRole(aRole)) {
        skillBarItemD_ = new MCSkillBarItem;
        skillBarItemD_->init(skillD);
    } else {
        skillBarItemD_ = NULL;
    }
}

MCSkillBarItemGroup *
MCSkillBarItemGroup::create(MCRole *aRole)
{
    MCSkillBarItemGroup *group = new MCSkillBarItemGroup;
    
    if (group) {
        group->init(aRole);
        group->autorelease();
    }
    
    return group;
}

#pragma mark -
#pragma mark *** MCSkillBar ***

MCSkillBar::~MCSkillBar()
{
    CC_SAFE_RELEASE(skillBarItemGroups_);
}

bool
MCSkillBar::init()
{
    if (CCLayer::init()) {
        CCPoint anchorPoint = ccp(0.5f, 0.0f);
        
        CCArray *roles = MCTeam::sharedTeam()->getRoles();
        CCObject *obj;
        MCRole *role;
        mc_tag_t tag = 0x13;
        
        skillBarItemGroups_ = CCDictionary::create();
        skillBarItemGroups_->retain();
        CCARRAY_FOREACH(roles, obj) {
            role = dynamic_cast<MCRole *>(obj);
            role->setTag(tag++);
            skillBarItemGroups_->setObject(MCSkillBarItemGroup::create(role),
                                           role->getTag());
        }
        
        CCMenuItemImage *opened = CCMenuItemImage::create(kMCSkillBarItemOpenedFilepath,
                                                          kMCSkillBarItemOpenedSelectedFilepath);
        CCMenuItemImage *closed = CCMenuItemImage::create(kMCSkillBarItemClosedFilepath,
                                                          kMCSkillBarItemClosedSelectedFilepath);
        CCMenuItemToggle *toggleButton = CCMenuItemToggle::createWithTarget(this,
                                                                            menu_selector(MCSkillBar::toggle),
                                                                            opened,
                                                                            closed,
                                                                            NULL);
        toggleButton_ = CCMenu::createWithItem(toggleButton);
        addChild(toggleButton_);
        toggleButton->setAnchorPoint(anchorPoint);
        
        currentSkillBarItemGroup_ = NULL;
        
        align();
        
        return true;
    }
    
    return false;
}

void
MCSkillBar::toggle()
{
    if (!currentSkillBarItemGroup_
        || !currentSkillBarItemGroup_->skillBarItemA_) {
        return;
    }
    
    CCSize size = currentSkillBarItemGroup_->skillBarItemA_->getContentSize();
    CCPoint offset = ccp(0, -size.height * 2);

    if (currentSkillBarItemGroup_->skillBarItemA_->getPositionY() <= CCDirectorGetWindowsSize().height) { /* 将要隐藏 */
        offset.y = -offset.y;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCSkillBarVisibleDidChangeNotification);
    currentSkillBarItemGroup_->skillBarItemA_->runAction(CCMoveBy::create(kMCActionDuration, offset));
    if (currentSkillBarItemGroup_->skillBarItemB_) {
        currentSkillBarItemGroup_->skillBarItemB_->runAction(CCMoveBy::create(kMCActionDuration, offset));
    }
    if (currentSkillBarItemGroup_->skillBarItemC_) {
        currentSkillBarItemGroup_->skillBarItemC_->runAction(CCMoveBy::create(kMCActionDuration, offset));
    }
    if (currentSkillBarItemGroup_->skillBarItemD_) {
        currentSkillBarItemGroup_->skillBarItemD_->runAction(CCMoveBy::create(kMCActionDuration, offset));
    }
}

bool
MCSkillBar::isHidden()
{
    return !(currentSkillBarItemGroup_
             && currentSkillBarItemGroup_->skillBarItemA_->getPositionY() <= CCDirectorGetWindowsSize().height);
}

void
MCSkillBar::showSkillsForRole(MCRole *aRole)
{
    cleanOld();
    currentSkillBarItemGroup_ = aRole
                                ? dynamic_cast<MCSkillBarItemGroup *>(skillBarItemGroups_->objectForKey(aRole->getTag()))
                                : NULL;
    align();
}

MCSkillBarItem *
MCSkillBar::itemForTouch(CCTouch *pTouch)
{
    if (! currentSkillBarItemGroup_) {
        return NULL;
    }
    
    CCPoint touchLocation = pTouch->getLocation();
    
    /* skillBarItemA_ */
    MCSkillBarItem *item = currentSkillBarItemGroup_->skillBarItemA_;
    if (! item) {
        return NULL;
    }
    CCPoint local = item->convertToNodeSpace(touchLocation);
    CCSize s = item->getContentSize();
    CCRect r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return item;
    }
    /* skillBarItemB_ */
    item = currentSkillBarItemGroup_->skillBarItemB_;
    if (! item) {
        return NULL;
    }
    local = item->convertToNodeSpace(touchLocation);
    s = item->getContentSize();
    r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return item;
    }
    /* skillBarItemC_ */
    item = currentSkillBarItemGroup_->skillBarItemC_;
    if (! item) {
        return NULL;
    }
    local = item->convertToNodeSpace(touchLocation);
    s = item->getContentSize();
    r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return item;
    }
    /* skillBarItemD_ */
    item = currentSkillBarItemGroup_->skillBarItemD_;
    if (! item) {
        return NULL;
    }
    local = item->convertToNodeSpace(touchLocation);
    s = item->getContentSize();
    r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return item;
    }
    
    return NULL;
}

void
MCSkillBar::cleanOld()
{
    if (currentSkillBarItemGroup_) {
        if (currentSkillBarItemGroup_->skillBarItemA_) {
            currentSkillBarItemGroup_->skillBarItemA_->removeFromParentAndCleanup(false);
        }
        if (currentSkillBarItemGroup_->skillBarItemB_) {
            currentSkillBarItemGroup_->skillBarItemB_->removeFromParentAndCleanup(false);
        }
        if (currentSkillBarItemGroup_->skillBarItemC_) {
            currentSkillBarItemGroup_->skillBarItemC_->removeFromParentAndCleanup(false);
        }
        if (currentSkillBarItemGroup_->skillBarItemD_) {
            currentSkillBarItemGroup_->skillBarItemD_->removeFromParentAndCleanup(false);
        }
    }
}

void
MCSkillBar::align()
{
    if (currentSkillBarItemGroup_) {
        if (! currentSkillBarItemGroup_->skillBarItemA_) {
            return;
        }
        CCSize winSize = CCDirectorGetWindowsSize();
        CCSize itemSize = currentSkillBarItemGroup_->skillBarItemA_->getContentSize();
        CCPoint centerPoint = ccp(winSize.width / 2, winSize.height - itemSize.height / 2);
        CCPoint itemPosition = ccp(centerPoint.x - itemSize.width * 1.5, centerPoint.y);
        
        currentSkillBarItemGroup_->skillBarItemA_->setPosition(itemPosition);
        addChild(currentSkillBarItemGroup_->skillBarItemA_);
        
        if (currentSkillBarItemGroup_->skillBarItemB_) {
            itemPosition = ccp(centerPoint.x - itemSize.width * 0.5, centerPoint.y);
            currentSkillBarItemGroup_->skillBarItemB_->setPosition(itemPosition);
            addChild(currentSkillBarItemGroup_->skillBarItemB_);
        }
        
        if (currentSkillBarItemGroup_->skillBarItemC_) {
            itemPosition = ccp(centerPoint.x + itemSize.width * 0.5, centerPoint.y);
            currentSkillBarItemGroup_->skillBarItemC_->setPosition(itemPosition);
            addChild(currentSkillBarItemGroup_->skillBarItemC_);
        }
        
        if (currentSkillBarItemGroup_->skillBarItemD_) {
            itemPosition = ccp(centerPoint.x + itemSize.width * 1.5, centerPoint.y);
            currentSkillBarItemGroup_->skillBarItemD_->setPosition(itemPosition);
            addChild(currentSkillBarItemGroup_->skillBarItemD_);
        }
        
        itemPosition = ccp(centerPoint.x + itemSize.width * 2.5, centerPoint.y - 32);
        toggleButton_->setPosition(itemPosition);
        
        setVisible(true);
    } else {
        dynamic_cast<CCMenuItemToggle *>(toggleButton_->getChildren()->objectAtIndex(0))->setSelectedIndex(0);
        setVisible(false);
    }
}
