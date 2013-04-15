//
//  MCLoadingScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-6.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCLoadingScene.h"
#include "MCSceneController.h"

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

/**
 * 执行过程
 * 1.注册通知中心
 * 2.显示loading
 * 3.加载场景
 * 4.发出加载完成的通知
 * 5.隐藏loading
 * 6.切换场景
 */
void
MCLoading::loadNewScene()
{
    setOpacity(0);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(MCLoading::sceneDidLoad),
                                                                  kMCSceneDidLoadNotification,
                                                                  NULL);
    CCDirector::sharedDirector()->pushScene(MCLoadingSharedScene());
    schedule(schedule_selector(MCLoading::loading));
    runAction(CCFadeIn::create(kMCActionDuration));
    
    MCSceneController::sharedSceneController()->__loadScene();
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
void 
MCLoading::sceneDidLoad(CCObject *obj)
{
    runAction(CCSequence::create(CCFadeOut::create(kMCActionDuration),
                                 CCCallFunc::create(this, callfunc_selector(MCLoading::didHide)),
                                 NULL));
}

void
MCLoading::didHide()
{
    unschedule(schedule_selector(MCLoading::loading));
    CCDirector::sharedDirector()->popScene();
    MCSceneController::sharedSceneController()->__changeScene();
}
