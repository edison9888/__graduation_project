//
//  MCSimpleAudioEngine.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSimpleAudioEngine.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

static MCSimpleAudioEngine *__shared_simple_audio_engine = NULL;

MCSimpleAudioEngine::MCSimpleAudioEngine()
{
    SimpleAudioEngine *audioEngine = SimpleAudioEngine::sharedEngine();
    
    lastPlayingMusicFilepath_.assign("");
    expectedMusicFilepath_.assign("");
    
    audioEngine->setBackgroundMusicVolume(0.6f);
    audioEngine->setEffectsVolume(0.8f);
}

MCSimpleAudioEngine *
MCSimpleAudioEngine::sharedSimpleAudioEngine()
{
    if (__shared_simple_audio_engine == NULL) {
        __shared_simple_audio_engine = new MCSimpleAudioEngine;
    }
    
    return __shared_simple_audio_engine;
}

void
MCSimpleAudioEngine::setExpectedMusic(const char *aMusicFilepath)
{
    expectedMusicFilepath_.assign(aMusicFilepath);
}

void
MCSimpleAudioEngine::playMusic()
{
    SimpleAudioEngine *audioEngine = SimpleAudioEngine::sharedEngine();
    const char *expectedMusicFilepath = expectedMusicFilepath_.c_str();
    
    audioEngine->preloadBackgroundMusic(expectedMusicFilepath);
    audioEngine->playBackgroundMusic(expectedMusicFilepath, true);
    lastPlayingMusicFilepath_.assign(expectedMusicFilepath_);
}

void
MCSimpleAudioEngine::stopMusic()
{
    SimpleAudioEngine *audioEngine = SimpleAudioEngine::sharedEngine();
    if (audioEngine->isBackgroundMusicPlaying()) {
        audioEngine->stopBackgroundMusic(true);
    }
}

void
MCSimpleAudioEngine::pauseMusic()
{
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void
MCSimpleAudioEngine::resumeMusic()
{
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void
MCSimpleAudioEngine::playEffect(const char *anEffectFilepath)
{
    SimpleAudioEngine::sharedEngine()->playEffect(anEffectFilepath);
}
