//
//  MCSplashScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSplashScene.h"

static const CCSize kMCLogoSize = CCSizeMake(623, 424);

int Tag_logo = 1;

bool
MCSplash::init()
{
    if (CCLayerColor::initWithColor(ccc4(0, 0, 0, 255))) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        CCSprite *logo = CCSprite::create("logo.jpg");
        float expectedHeight = 240.0f / contentScaleFactor;
        float scale = expectedHeight / kMCLogoSize.height;
        
        logo->runAction(CCScaleTo::create(0.0f, scale));
    
        addChild(logo);
        logo->setTag(Tag_logo);
        logo->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        logo->setOpacity(0);
        
        return true;
    }
    
    return false;
}

void
MCSplash::onEnter()
{
    CCLayer::onEnter();
    CCNode *logo = getChildByTag(Tag_logo);
    logo->runAction(CCSequence::create(CCDelayTime::create(0.5f),
                                       CCFadeIn::create(1.0f),
                                       CCFadeOut::create(1.5f),
                                       CCCallFuncN::create(this,
                                                           callfuncN_selector(MCSplash::finish)),
                                       NULL
                                       )
                    );
}

void
MCSplash::finish(CCNode* node)
{
    CCDirector::sharedDirector()->popScene();
}
