//
//  MCBackgroundLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "SimpleAudioEngine.h"
#include "MCBackgroundLayer.h"

using namespace CocosDenshion;

MCBackgroundLayer::~MCBackgroundLayer()
{
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
    if (isPlayBackgroundMusicImmediately_) {
        playMusic();
    }
}

void
MCBackgroundLayer::onExit()
{
    stopMusic();
    CCLayer::onExit();
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
