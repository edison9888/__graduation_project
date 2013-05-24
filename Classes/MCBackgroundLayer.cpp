//
//  MCBackgroundLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCSimpleAudioEngine.h"
#include "MCBackgroundLayer.h"
#include "MCShadow.h"
#include "MCTeam.h"

const bool kMCIsPlayBackgroundMusicImmediately = true;

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
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        map_ = CCTMXTiledMap::create(aMapFilePath);
        if (!map_) {
            return false;
        }
        addChild(map_);
        
        CCSize mapSize = map_->getMapSize();
        CCSize tileSize = map_->getTileSize();
        sceneSize_ = CCSizeMake(mapSize.width * tileSize.width / contentScaleFactor,
                                mapSize.height * tileSize.height / contentScaleFactor);

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
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(MCBackgroundLayer::roleWasDied),
                                                                  kMCRoleDiedNotification,
                                                                  NULL);
    schedule(schedule_selector(MCBackgroundLayer::update));
    if (kMCIsPlayBackgroundMusicImmediately) {
        MCSimpleAudioEngine::sharedSimpleAudioEngine()->setExpectedMusic(backgroundMusic_->getCString());
    }
}

void
MCBackgroundLayer::onExit()
{
    unschedule(schedule_selector(MCBackgroundLayer::update));
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,
                                                                     kMCRoleDiedNotification);
    stopMusic();
    
    CCObject *obj;
    CCARRAY_FOREACH(enemyShadows_, obj) {
        dynamic_cast<MCShadow *>(obj)->unbind();
    }
    enemyShadows_->removeAllObjects();
    CCARRAY_FOREACH(shadows_, obj) {
        dynamic_cast<MCShadow *>(obj)->unbind();
    }
    shadows_->removeAllObjects();
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
    MCSimpleAudioEngine::sharedSimpleAudioEngine()->playMusic();
}

void
MCBackgroundLayer::stopMusic()
{
    MCSimpleAudioEngine::sharedSimpleAudioEngine()->stopMusic();
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

void
MCBackgroundLayer::roleWasDied(CCObject *aRoleObject)
{
    MCRole *role = dynamic_cast<MCRole *>(aRoleObject);
    CCObject *obj;
    MCShadow *shadow;
    MCShadow *shadowForRemove = NULL;
    CCArray *shadows = enemyShadows_;
    
    /* objects */
    CCARRAY_FOREACH(shadows, obj) {
        shadow = dynamic_cast<MCShadow *>(obj);
        if (shadow->getRole() == role) {
            shadow->unbind();
            shadowForRemove = shadow;
            break;
        }
    }
    if (shadowForRemove != NULL) {
        enemyShadows_->removeObject(shadowForRemove);
        return;
    }
    /* team */
    shadows = shadows_;
    CCARRAY_FOREACH(shadows, obj) {
        shadow = dynamic_cast<MCShadow *>(obj);
        if (shadow->getRole() == role) {
            shadow->unbind();
            shadowForRemove = shadow;
            break;
        }
    }
    if (shadowForRemove != NULL) {
        shadows_->removeObject(shadowForRemove);
    }
}
