//
//  MCLoadingScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-6.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCLoadingScene.h"

static const char *kMCPoints[] = {
    ".",
    "..",
    "...",
    "....",
};
static int kMCPointsIndex = 0;
static int kMCPointsCount = 0;
static int kMCPointsRate = 60 / 4;
static int kMCPointsSize = sizeof(kMCPoints) / sizeof(const char *);

static const float kMCActionDuration = 0.1f;

static MCLoading *__shared_loading = NULL;
static CCScene *__shared_loading_scene = NULL;

static CCScene *
MCLoadingSharedScene()
{
    if (__shared_loading_scene == NULL) {
        __shared_loading_scene = new CCScene;
        __shared_loading_scene->init();
        __shared_loading_scene->addChild(MCLoading::sharedLoading());
    }
    
    return __shared_loading_scene;
}

bool
MCLoading::init()
{
    if (CCLayerColor::initWithColor(ccc4(24, 24, 24, 255))) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCLabelTTF *label = CCLabelTTF::create("Loading", "Marker Felt", 36);
        addChild(label);
        label->setAnchorPoint(ccp(1, 0));
        CCSize labelSize = label->getContentSize();
        label->setPosition(ccp(winSize.width - labelSize.width * 2,
                                 labelSize.height));
        
        points_ = CCLabelTTF::create(kMCPoints[0], "Marker Felt", 36);
        addChild(points_);
        points_->setAnchorPoint(ccp(0, 0));
        points_->setPosition(ccp(label->getPositionX() + 4,
                                 label->getPositionY()));
        
        return true;
    }
    
    return false;
}

MCLoading *
MCLoading::sharedLoading()
{
    if (__shared_loading == NULL) {
        __shared_loading = new MCLoading;
        if (__shared_loading && __shared_loading->init()) {
        } else {
            delete __shared_loading;
            __shared_loading = NULL;
        }
    }
    
    return __shared_loading;
}

void
MCLoading::show()
{
    setOpacity(0);
    CCDirector::sharedDirector()->pushScene(MCLoadingSharedScene());
    schedule(schedule_selector(MCLoading::loading));
    runAction(CCFadeIn::create(kMCActionDuration));
}

void
MCLoading::hide()
{
    CCDirector *director = CCDirector::sharedDirector();
    
    CCLog("will hide");
//    if (director->getRunningScene() == MCLoadingSharedScene()) {
        CCLog("hide");
        unschedule(schedule_selector(MCLoading::loading));
        runAction(CCFadeOut::create(kMCActionDuration));
        director->popScene();
//    }
}

void
MCLoading::loading(float dt)
{
    if (++kMCPointsCount % kMCPointsRate != 0) {
        return;
    }
    if (kMCPointsIndex == kMCPointsSize) {
        kMCPointsIndex = 0;
    }
    points_->setString(kMCPoints[kMCPointsIndex++]);
}

