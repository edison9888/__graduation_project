//
//  MCBaseInfoLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCBaseInfoLayer.h"

static const float kMCActionDuration = 0.1f;

void
MCBaseInfoLayer::onEnter()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    
    CCLayer::onEnter();
    setPosition(ccp(winSize.width, 0));
}

void
MCBaseInfoLayer::show()
{
    loadData();
    runAction(CCMoveTo::create(kMCActionDuration, CCPointZero));
}

void
MCBaseInfoLayer::hide()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    runAction(CCMoveTo::create(kMCActionDuration, ccp(winSize.width, 0)));
}
