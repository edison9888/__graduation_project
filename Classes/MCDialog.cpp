//
//  MCDialog.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCDialog.h"

class __mc_other_dialog : public MCDialog {
public:
    bool init();
};

class __mc_hero_dialog : public MCDialog {
public:
    bool init();
};

static __mc_other_dialog *__shared_other_dialog = NULL;
static __mc_hero_dialog *__shared_hero_dialog = NULL;

MCDialog *
MCDialog::sharedDialog(MCDialogType aType)
{
    MCDialog *dialog = NULL;
    if (aType == MCOtherDialog) {
        if (__shared_other_dialog == NULL) {
            __shared_other_dialog = new __mc_other_dialog;
            if (__shared_other_dialog && __shared_other_dialog->init()) {
                __shared_other_dialog->autorelease();
            } else {
                delete __shared_other_dialog;
                __shared_other_dialog = NULL;
            }
            dialog = __shared_other_dialog;
        }
    } else if (aType == MCHeroDialog) {
        if (__shared_hero_dialog == NULL) {
            __shared_hero_dialog = new __mc_hero_dialog;
            if (__shared_hero_dialog && __shared_hero_dialog->init()) {
                __shared_hero_dialog->autorelease();
            } else {
                delete __shared_hero_dialog;
                __shared_hero_dialog = NULL;
            }
            dialog = __shared_hero_dialog;
        }
    }
    
    return dialog;
}

/* 某人想说几句话 */
void
MCDialog::someoneWannaSaySomething(MCRole *role)
{
    dialogue_->setString(role->nextSentence());
    dialogue_->setString(role->getName()->getCString());
}

bool
__mc_other_dialog::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        
        CCRect skinRect = CCRectMake(0, 0, 128, 128);
        CCRect skinCapInsets = CCRectMake(0, 0, 128, 128); /* 对半分 left,right,width,height */
        
        skin_ = CCScale9Sprite::create("dialog/wskin.png", skinRect, skinCapInsets);
        addChild(skin_);
        skin_->setContentSize(CCSizeMake(winSize.width - 4, winSize.height / 3));
        skin_->setPosition(ccp(winSize.width / 2 - 2, winSize.height / 6));
        
        CCRect edgeRect = CCRectMake(0, 0, 64, 64);
        
        edge_ = CCScale9Sprite::create("dialog/edge.png", edgeRect);
        addChild(edge_);
        edge_->setContentSize(CCSizeMake(winSize.width, winSize.height / 3));
        edge_->setPosition(ccp(winSize.width / 2, winSize.height / 6));
        
        dialogue_ = CCLabelTTF::create();
        dialogue_->setFontName("Marker Felt");
        dialogue_->setFontSize(24);
        addChild(dialogue_);
        dialogue_->setDimensions(CCSizeMake(edge_->getContentSize().width - 32, 0));
        dialogue_->setHorizontalAlignment(kCCTextAlignmentLeft);
        dialogue_->setAnchorPoint(ccp(0, 1)); /* 左上角为锚点 */
        dialogue_->setPosition(ccp(16, edge_->getContentSize().height - 16));
        
        speakerName_ = CCLabelTTF::create();
        speakerName_->setFontName("Marker Felt");
        speakerName_->setFontSize(18);
        speakerName_->setColor(ccc3(204, 204, 204));
        addChild(speakerName_);
        speakerName_->setAnchorPoint(ccp(0, 0)); /* 左下角 */
        speakerName_->setPosition(ccp(26,
                                      edge_->getContentSize().height + 4));
        
        
        faceBox_ = MCFaceBox::create("faces/other.png", "UI/face_box.png");
        faceBox_->setAnchorPoint(ccp(0, 0)); /* 左下角 */
        addChild(faceBox_);
        faceBox_->setPosition(ccp(4,
                                  skin_->getContentSize().height + speakerName_->getFontSize() * 1.5 - 4));
        
        speakerName_->setAnchorPoint(ccp(0.5, 0.5));
        speakerName_->setPosition(ccp(faceBox_->getContentSize().width / 2,
                                      (faceBox_->getPosition().y + edge_->getContentSize().height) / 2 + 4));
        
        return true;
    }
    return false;
}

bool
__mc_hero_dialog::init()
{
    if (CCLayer::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        
        CCRect skinRect = CCRectMake(0, 0, 128, 128);
        CCRect skinCapInsets = CCRectMake(0, 0, 128, 128); /* 对半分 left,right,width,height */
        
        skin_ = CCScale9Sprite::create("dialog/wskin.png", skinRect, skinCapInsets);
        addChild(skin_);
        skin_->setContentSize(CCSizeMake(winSize.width - 4, winSize.height / 3));
        skin_->setPosition(ccp(winSize.width / 2 - 2, winSize.height / 6));
        
        CCRect edgeRect = CCRectMake(0, 0, 64, 64);
        
        edge_ = CCScale9Sprite::create("dialog/edge.png", edgeRect);
        addChild(edge_);
        edge_->setContentSize(CCSizeMake(winSize.width, winSize.height / 3));
        edge_->setPosition(ccp(winSize.width / 2, winSize.height / 6));
        
        dialogue_ = CCLabelTTF::create();
        dialogue_->setFontName("Marker Felt");
        dialogue_->setFontSize(24);
        addChild(dialogue_);
        dialogue_->setDimensions(CCSizeMake(edge_->getContentSize().width - 32, 0));
        dialogue_->setHorizontalAlignment(kCCTextAlignmentLeft);
        dialogue_->setAnchorPoint(ccp(0, 1)); /* 左上角为锚点 */
        dialogue_->setPosition(ccp(16, edge_->getContentSize().height - 16));
        
        speakerName_ = CCLabelTTF::create();
        speakerName_->setFontName("Marker Felt");
        speakerName_->setFontSize(18);
        speakerName_->setColor(ccc3(204, 204, 204));
        addChild(speakerName_);
        
        faceBox_ = MCFaceBox::create("faces/hero.png", "UI/face_box.png");
        faceBox_->setAnchorPoint(ccp(1, 0)); /* 右下角 */
        addChild(faceBox_);
        faceBox_->setPosition(ccp(winSize.width - 4,
                                  skin_->getContentSize().height + speakerName_->getFontSize() * 1.5 - 4));
        
        speakerName_->setAnchorPoint(ccp(0.5, 0.5));
        speakerName_->setPosition(ccp(winSize.width - faceBox_->getContentSize().width / 2,
                                      (faceBox_->getPosition().y + edge_->getContentSize().height) / 2 + 4));
        return true;
    }
    return false;
}
