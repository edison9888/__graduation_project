//
//  MCTestViewport.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestViewport__
#define __Military_Confrontation__MCTestViewport__

#include "MCGameScene.h"
#include "MCHero.h"
#include "MCTestLayer.h"

class _viewport_MCScenePackage : public MCScenePackage {
public:
    void setTMXTiledMapPath(const char *path) {
        tmxTiledMapPath_ = CCString::create(path);
        tmxTiledMapPath_->retain();
    }
    void setBackgroundMusic(const char *path) {
        backgroundMusicPath_ = CCString::create(path);
        backgroundMusicPath_->retain();
    }
};

class _viewport_MCGameScene : public MCGameScene {
public:
    void init() {
        _viewport_MCScenePackage *p = new _viewport_MCScenePackage;
        p->setTMXTiledMapPath(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("maps/village.tmx"));
        p->setBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/bgm-b.mp3"));
        this->initWithScenePackage(p);
        this->setSilent();
        MCHero::sharedHero()->getEntity()->setPosition(ccp(160, 160));
        
        MCNPC *role = new MCNPC;
        role->init();
        addChild(role->getEntity()->getSpriteSheet());
        role->getEntity()->setPosition(ccp(200, 200));
    }
    
    void setSilent() {
        background_->setPlayBackgroundMusicImmediately(false);
    }
};

class MCTestViewport : public MCTestLayer {
public:
    
    static CCScene *scene() {
        _viewport_MCGameScene *scene = new _viewport_MCGameScene;
        
        scene->init();
        MCTestViewport *s = new MCTestViewport;
        s->initWithColor(ccc4(0, 0, 0, 0));
        scene->addChild(s);
        
        return scene;
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

#endif /* defined(__Military_Confrontation__MCTestViewport__) */
