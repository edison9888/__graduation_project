//
//  MCBackgroundLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "SimpleAudioEngine.h"
#include "MCBackgroundLayer.h"
#include "MCShadow.h"
#include "MCTeam.h"

using namespace CocosDenshion;

MCBackgroundLayer::~MCBackgroundLayer()
{
    CC_SAFE_RELEASE(shadows_);
    CC_SAFE_RELEASE(enemyShadows_);
    CC_SAFE_RELEASE(backgroundMusic_);
    CC_SAFE_RELEASE(map_);
}

bool
MCBackgroundLayer::init(const char *aMapFilePath, const char *aBackgroundMusicFilePath)
{
    if (CCLayer::init()) {
        map_ = CCTMXTiledMap::create(aMapFilePath);
        if (!map_) {
            return false;
        }
        addChild(map_);

        backgroundMusic_ = CCString::create(aBackgroundMusicFilePath);
        backgroundMusic_->retain();
        
        enemyShadows_ = CCArray::create();
        enemyShadows_->retain();
        shadows_ = CCArray::create();
        shadows_->retain();
        
        return true;
    }
    
    return false;
}

MCBackgroundLayer *
MCBackgroundLayer::create(const char *aMapFilePath, const char *aBackgroundMusicFilePath)
{
    MCBackgroundLayer *layer = new MCBackgroundLayer;
    
    if (layer && layer->init(aMapFilePath, aBackgroundMusicFilePath)) {
        layer->autorelease();
    } else {
        CC_SAFE_DELETE(layer);
        layer = NULL;
    }
    
    return layer;
}

void
MCBackgroundLayer::onEnter()
{
    CCLayer::onEnter();
    schedule(schedule_selector(MCBackgroundLayer::update));
    if (isPlayBackgroundMusicImmediately_) {
        playMusic();
    }
}

void
MCBackgroundLayer::onExit()
{
    unschedule(schedule_selector(MCBackgroundLayer::update));
    stopMusic();
    CCLayer::onExit();
}

void
MCBackgroundLayer::update(float dt)
{
    CCLayer::update(dt);
    CCObject *obj;
    CCARRAY_FOREACH(enemyShadows_, obj) {
        MCShadow *shadow = dynamic_cast<MCShadow *>(obj);
        shadow->updatePosition();
    }
    CCARRAY_FOREACH(shadows_, obj) {
        MCShadow *shadow = dynamic_cast<MCShadow *>(obj);
        shadow->updatePosition();
    }
}

void
MCBackgroundLayer::playMusic()
{
    SimpleAudioEngine *audioEngine = SimpleAudioEngine::sharedEngine();
    if (audioEngine->isBackgroundMusicPlaying()) {
        audioEngine->stopBackgroundMusic(true);
    }
    audioEngine->preloadBackgroundMusic(backgroundMusic_->getCString());
    audioEngine->playBackgroundMusic(backgroundMusic_->getCString(), true);
}

void
MCBackgroundLayer::stopMusic()
{
    SimpleAudioEngine *audioEngine = SimpleAudioEngine::sharedEngine();
    if (audioEngine->isBackgroundMusicPlaying()) {
        audioEngine->stopBackgroundMusic(true);
    }
}

void
MCBackgroundLayer::loadEnemies(CCArray *objects)
{
    CCObject *obj;
    enemyShadows_->removeAllObjects();
    CCARRAY_FOREACH(objects, obj) {
        MCShadow *shadow = MCShadow::create();
        shadow->bind(dynamic_cast<MCRole *>(obj));
        addChild(shadow);
        enemyShadows_->addObject(shadow);
    }
}

void
MCBackgroundLayer::loadTeam(MCTeam *aTeam)
{
    CCArray *objects = aTeam->getRoles();
    CCObject *obj;
    shadows_->removeAllObjects();
    CCARRAY_FOREACH(objects, obj) {
        MCShadow *shadow = MCShadow::create();
        shadow->bind(dynamic_cast<MCRole *>(obj));
        addChild(shadow);
        shadows_->addObject(shadow);
    }
}
