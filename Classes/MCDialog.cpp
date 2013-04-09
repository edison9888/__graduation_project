//
//  MCDialog.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCDialog.h"

class __mc_npc_dialog : public MCDialog {
    friend class MCDialog;
public:
    bool init();
};

class __mc_dm_dialog : public MCDialog {
    friend class MCDialog;
public:
    bool init();
};

static __mc_npc_dialog *__shared_npc_dialog = NULL;
static __mc_dm_dialog *__shared_dm_dialog = NULL;

MCDialog::~MCDialog()
{
    CC_SAFE_RELEASE(edge_);
    CC_SAFE_RELEASE(skin_);
    CC_SAFE_RELEASE(dialogue_);
    CC_SAFE_RELEASE(speakerName_);
    CC_SAFE_RELEASE(speakerFace_);
    CC_SAFE_RELEASE(speakerFaceBox_);
}

MCDialog *
MCDialog::sharedDialog(MCDialogType aType)
{
    MCDialog *dialog = NULL;
    if (aType == MCNPCDialog) {
        if (__shared_npc_dialog == NULL) {
            __shared_npc_dialog = new __mc_npc_dialog;
            if (__shared_npc_dialog && __shared_npc_dialog->init()) {
            } else {
                delete __shared_npc_dialog;
                __shared_npc_dialog = NULL;
            }
        }
        dialog = __shared_npc_dialog;
    } else if (aType == MCDMDialog) {
        if (__shared_dm_dialog == NULL) {
            __shared_dm_dialog = new __mc_dm_dialog;
            if (__shared_dm_dialog && __shared_dm_dialog->init()) {
            } else {
                delete __shared_dm_dialog;
                __shared_dm_dialog = NULL;
            }
        }
        dialog = __shared_dm_dialog;
    }
    
    return dialog;
}

bool
MCDialog::init()
{
    if (CCLayer::init()) {
        setTouchEnabled(true);
        return true;
    }
    
    return false;
}

/* 某人想说几句话 */
void
MCDialog::setMessage(const char *aMessage)
{
    dialogue_->setString(aMessage);
}

void
MCDialog::attach(CCNode *aParent)
{
    aParent->addChild(this);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void
MCDialog::detach()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    removeFromParentAndCleanup(false);
    (target_->*dismissSelector_)(userdata_);
}

bool
MCDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    bool in = false;
    CCArray *children = getChildren();
    CCObject *obj;
    CCNode *child;
    CCPoint touchLocation = pTouch->getLocation();
    
    CCARRAY_FOREACH(children, obj) {
        child = dynamic_cast<CCNode *>(obj);
        CCPoint local = child->convertToNodeSpace(touchLocation);
        CCSize size = child->getContentSize();
        CCRect r = CCRectMake(0, 0, size.width, size.height);
        if (r.containsPoint(touchLocation)) {
            in = true;
        }
    }
    
    return in;
}

void
MCDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    detach();
}

#pragma mark -
#pragma mark *** __mc_npc_dialog ***

bool
__mc_npc_dialog::init()
{
    if (MCDialog::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        float contentScaleFactor = CCDirectorGetContentScaleFactor();
        
        CCRect skinRect = CCRectMake(0, 0, 128, 128);
        CCRect skinCapInsets = CCRectMake(0, 0, 128, 128);
        
        skin_ = CCScale9Sprite::create("dialog/wskin.png", skinRect, skinCapInsets);
        addChild(skin_);
        skin_->setContentSize(CCSizeMake(winSize.width - 4 / contentScaleFactor,
                                         winSize.height / 3));
        skin_->setPosition(ccp(winSize.width / 2 - 2 / contentScaleFactor,
                               winSize.height / 6));
        
        CCRect edgeRect = CCRectMake(0, 0, 64, 64);
        
        edge_ = CCScale9Sprite::create("dialog/edge.png", edgeRect);
        addChild(edge_);
        edge_->setContentSize(CCSizeMake(winSize.width, winSize.height / 3));
        edge_->setPosition(ccp(winSize.width / 2, winSize.height / 6));
        
        dialogue_ = CCLabelTTF::create();
        dialogue_->setFontName("Marker Felt");
        dialogue_->setFontSize(24);
        addChild(dialogue_);
        dialogue_->setDimensions(CCSizeMake(edge_->getContentSize().width - 32 / contentScaleFactor,
                                            0));
        dialogue_->setHorizontalAlignment(kCCTextAlignmentLeft);
        dialogue_->setAnchorPoint(ccp(0, 1)); /* 左上角为锚点 */
        dialogue_->setPosition(ccp(16 / contentScaleFactor,
                                   edge_->getContentSize().height - 16 / contentScaleFactor));
        
        speakerName_ = CCLabelTTF::create();
        speakerName_->setFontName("Marker Felt");
        speakerName_->setFontSize(18 / contentScaleFactor);
        speakerName_->setColor(ccc3(204, 204, 204));
        addChild(speakerName_);
        speakerName_->setAnchorPoint(ccp(0, 0)); /* 左下角 */
        speakerName_->setPosition(ccp(26 / contentScaleFactor,
                                      edge_->getContentSize().height + 4 / contentScaleFactor));
        
        
//        faceBox_ = MCFaceBox::create("faces/other.png", "UI/face_box.png");
//        faceBox_->setAnchorPoint(ccp(0, 0)); /* 左下角 */
//        addChild(faceBox_);
//        faceBox_->setPosition(ccp(4 / contentScaleFactor,
//                                  skin_->getContentSize().height + speakerName_->getFontSize() * 1.5 - 4 / contentScaleFactor));
        
//        speakerName_->setAnchorPoint(ccp(0.5, 0.5));
//        speakerName_->setPosition(ccp(faceBox_->getContentSize().width / 2,
//                                      (faceBox_->getPosition().y + edge_->getContentSize().height) / 2 + 4 / contentScaleFactor));
        
        return true;
    }
    return false;
}

#pragma mark -
#pragma mark *** __mc_dm_dialog ***

bool
__mc_dm_dialog::init()
{
    if (MCDialog::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        float contentScaleFactor = CCDirectorGetContentScaleFactor();
        
        CCRect skinRect = CCRectMake(0, 0, 128, 128);
        CCRect skinCapInsets = CCRectMake(0, 0, 128, 128);
        
        skin_ = CCScale9Sprite::create("dialog/wskin.png", skinRect, skinCapInsets);
        addChild(skin_);
        skin_->setContentSize(CCSizeMake(winSize.width - 4 / contentScaleFactor,
                                         winSize.height / 3));
        skin_->setPosition(ccp(winSize.width / 2 - 2 / contentScaleFactor,
                               winSize.height / 6));
        
        CCRect edgeRect = CCRectMake(0, 0, 64, 64);
        
        edge_ = CCScale9Sprite::create("dialog/edge.png", edgeRect);
        addChild(edge_);
        edge_->setContentSize(CCSizeMake(winSize.width, winSize.height / 3));
        edge_->setPosition(ccp(winSize.width / 2, winSize.height / 6));
        
        dialogue_ = CCLabelTTF::create();
        dialogue_->setFontName("Marker Felt");
        dialogue_->setFontSize(24);
        addChild(dialogue_);
        dialogue_->setDimensions(CCSizeMake(edge_->getContentSize().width - 32 / contentScaleFactor,
                                            0));
        dialogue_->setHorizontalAlignment(kCCTextAlignmentLeft);
        dialogue_->setAnchorPoint(ccp(0, 1)); /* 左上角为锚点 */
        dialogue_->setPosition(ccp(16 / contentScaleFactor,
                                   edge_->getContentSize().height - 16 / contentScaleFactor));
        
        speakerName_ = NULL;
        faceBox_ = NULL;
        
        return true;
    }
    return false;
}
