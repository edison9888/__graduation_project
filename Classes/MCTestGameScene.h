//
//  MCTestGameScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestGameScene__
#define __Military_Confrontation__MCTestGameScene__

#include "MCGameScene.h"
#include "MCHero.h"
#include "MCTestLayer.h"
#include "MCSceneManager.h"

//class _MCScenePackage : public MCScenePackage {
//public:
//    void setTMXTiledMapPath(const char *path) {
//        tmxTiledMapPath_ = CCString::create(path);
//        tmxTiledMapPath_->retain();
//    }
//    void setBackgroundMusic(const char *path) {
//        backgroundMusicPath_ = CCString::create(path);
//        backgroundMusicPath_->retain();
//    }
//};
//
//class _MCGameScene : public MCGameScene {
//public:
//    void init() {
//        _MCScenePackage *p = new _MCScenePackage;
//        p->setTMXTiledMapPath(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("maps/village.tmx"));
//        p->setBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sounds/bgm-b.mp3"));
//        this->initWithScenePackage(p);
//        this->setSilent();
//        MCHero::sharedHero()->getEntity()->setPosition(ccp(160, 160));
//    }
//    
//    void setSilent() {
//        background_->setPlayBackgroundMusicImmediately(false);
//    }
//};

class MCTestGameScene : public MCTestLayer {
public:
    
    static CCScene *scene() {
        mc_object_id_t m_id = {
            'M', '0', '0', '1'
        };
        CCScene *scene = MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);

        scene->init();
        MCTestGameScene *s = new MCTestGameScene;
        s->initWithColor(ccc4(0, 0, 0, 0));
        scene->addChild(s);
        
        return scene;
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

#endif /* defined(__Military_Confrontation__MCTestGameScene__) */
