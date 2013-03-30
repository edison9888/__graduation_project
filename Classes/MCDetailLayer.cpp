//
//  MCDetailLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCDetailLayer.h"

static const float kMCActionDuration = 0.1f;

void
MCDetailLayer::onEnter()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    
    CCLayer::onEnter();
    setPosition(ccp(winSize.width, 0));
}

void
MCDetailLayer::show()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    loadData();
    runAction(CCMoveTo::create(kMCActionDuration, CCPointZero));
}

void
MCDetailLayer::hide()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    runAction(CCMoveTo::create(kMCActionDuration, ccp(winSize.width, 0)));
}
