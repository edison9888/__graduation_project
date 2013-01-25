//
//  MCSplashScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSplashScene.h"

int Tag_logo = 1;

bool
MCSplashLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(24, 24, 24, 255)))
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *logo = CCSprite::create("logo.jpg");
        addChild(logo);
        logo->setTag(Tag_logo);
        logo->setScaleX(winSize.width / logo->getContentSize().width);
        logo->setScaleY(winSize.height / logo->getContentSize().height);
        logo->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        logo->setOpacity(0);
        
        return true;
    }
    
    return false;
}

void
MCSplashLayer::onEnter()
{
    CCLayer::onEnter();
    CCNode *logo = getChildByTag(Tag_logo);
    logo->runAction(CCSequence::create(CCDelayTime::create(0.5f),
                                       CCFadeIn::create(1.0f),
                                       CCFadeOut::create(1.5f),
                                       CCCallFuncN::create(this,
                                                           callfuncN_selector(MCSplashLayer::finish)),
                                       NULL
                                       )
                    );
}

void
MCSplashLayer::finish(CCNode* node)
{
    CCDirector::sharedDirector()->popScene();
}