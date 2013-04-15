//
//  MCConfirm.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright(c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCConfirm.h"

static const ccColor3B Color_blue = ccc3 (54, 128, 241);
static const ccColor3B Color_write = ccc3 (241, 241, 241);

//static MCConfirm *__default_confirm = NULL;

MCConfirm::~MCConfirm()
{
}

bool
MCConfirm::init()
{
    CCSize winSize;
    CCSize dialogSize;
    
    if (CCLayerColor::initWithColor(ccc4(32, 32, 32, 64))) {
        winSize = CCDirector::sharedDirector()->getWinSize();
        dialogSize.width = winSize.width / 2;
        dialogSize.height = winSize.height / 2;
        
        CCLabelTTF *confirm = CCLabelTTF::create("(null)", "Arial", 32);
        confirm->setColor(ccc3(214, 214, 214));
        this->addChild(confirm, 1);
        confirmLocation_ = ccp(winSize.width / 2,
                                  winSize.height / 2 + confirm->getContentSize().height / 2);
        confirm->setPosition(confirmLocation_);
        
        CCLabelTTF *label = CCLabelTTF::create("YES", "Arial", 32);
        CCMenuItemLabel *yesMenuItem = CCMenuItemLabel::create(label);
        label = CCLabelTTF::create("NO", "Arial", 32);
        CCMenuItemLabel *noMenuItem = CCMenuItemLabel::create(label);
        
        CCMenu *menu = CCMenu::create(yesMenuItem, noMenuItem, NULL);
        menu->alignItemsHorizontallyWithPadding(64);
        this->addChild(menu, 1);
        CCPoint point = menu->getPosition();
        point.y =(winSize.height - dialogSize.height) / 2 + label->getContentSize().height;
        actionButtonsLocation_ = point;
        menu->setPosition(point);
        
        yesMenuItem->setUserObject(this);
        noMenuItem->setUserObject(this);
        confirm_ = confirm;
        yes_ = yesMenuItem;
        no_ = noMenuItem;
        actionButtonsMenu_ = menu;
        
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,
                                                                                menu->getTouchPriority(),
                                                                                true);
        
        return true;
    }
    
    return false;
}

bool
MCConfirm::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    /* 截取触摸事件 */
    return true;
}

MCConfirm *
MCConfirm::confirm(CCNode *aParent, MCConfirmDelegate *aDelegate, const char *aMessage)
{
    MCConfirm *confirm = new MCConfirm;
    
//    if (__default_confirm == NULL) {
//        __default_confirm = new MCConfirm;
//        if (__default_confirm && __default_confirm->init()) {
//        } else {
//            delete __default_confirm;
//            __default_confirm = NULL;
//        }
//    }
//    confirm = __default_confirm;
    if (confirm && confirm->init()) {
        confirm->autorelease();
        confirm->setConfirm(aMessage);
        confirm->setDelegate(aDelegate);
        aParent->addChild(confirm);
        confirm->open();
    } else {
        delete confirm;
        confirm = NULL;
    }
    
    return confirm;
}

bool
MCConfirm::hasParent()
{
//    return __default_confirm && __default_confirm->m_pParent;
    return false;
}

void
MCConfirm::onEnter()
{
    CCLayerColor::onEnter();
}

void
MCConfirm::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

void
MCConfirm::open()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    confirm_->setPosition(ccp(-confirm_->getContentSize().width / 2,
                                   confirmLocation_.y));
    actionButtonsMenu_->setPosition(ccp(winSize.width + actionButtonsMenu_->getContentSize().width / 2,
                                            actionButtonsLocation_.y));
    
    runAction(CCSequence::create(CCFadeIn::create(0.2),
                                 CCCallFuncN::create(this,
                                                     callfuncN_selector(MCConfirm::showConfirm)),
                                 CCCallFuncN::create(this,
                                                     callfuncN_selector(MCConfirm::showActionButton)),
                                 NULL));
}

void
MCConfirm::close()
{
    runAction(CCSequence::create(CCCallFuncN::create(this,
                                                     callfuncN_selector(MCConfirm::hideConfirm)),
                                 CCCallFuncN::create(this,
                                                     callfuncN_selector(MCConfirm::hideActionButton)),
                                 CCDelayTime::create(0.3),
                                 CCFadeOut::create(0.2),
                                 CCCallFunc::create(this, callfunc_selector(MCConfirm::destroy)),
                                 NULL));
}

void
MCConfirm::showConfirm(CCNode* pSender)
{
    confirm_->runAction(CCMoveTo::create(0.3, confirmLocation_));
}

void
MCConfirm::hideConfirm(CCNode* pSender)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    confirm_->runAction(CCMoveTo::create(0.3,
                                            ccp(winSize.width + confirm_->getContentSize().width / 2,
                                                confirmLocation_.y)));
}

void
MCConfirm::showActionButton(CCNode* pSender)
{
    actionButtonsMenu_->runAction(CCMoveTo::create(0.3, actionButtonsLocation_));
}

void
MCConfirm::hideActionButton(CCNode* pSender)
{
    actionButtonsMenu_->runAction(CCMoveTo::create(0.3,
                                                     ccp(-actionButtonsMenu_->getContentSize().width / 2,
                                                         actionButtonsLocation_.y)));
}

void
MCConfirm::destroy()
{
    removeFromParentAndCleanup(false);
}

MCConfirmDelegate *
MCConfirm::getDelegate()
{
    return delegate_;
}

void
MCConfirm::setDelegate(MCConfirmDelegate *var)
{
    delegate_ = var;
    if (var) {
        yes_->setTarget(this, menu_selector(MCConfirm::yesButtonDidClick));
        no_->setTarget(this, menu_selector(MCConfirm::noButtonDidClick));
    }
}


void
MCConfirm::yesButtonDidClick(CCObject *obj) {
    if (delegate_) {
        delegate_->confirmDidClickYesButton(this);
        close();
    }
}

void
MCConfirm::noButtonDidClick(CCObject *obj) {
    if (delegate_) {
        delegate_->confirmDidClickNoButton(this);
        close();
    }
}
