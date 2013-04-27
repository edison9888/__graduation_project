//
//  MCTestEffect.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestEffect__
#define __Military_Confrontation__MCTestEffect__

#include "MCTestLayer.h"
#include "MCSceneManager.h"
#include "MCHero.h"
#include "MCGameState.h"

class MCTestEffect : public MCTestLayer {
public:
    
    static CCScene *scene() {
        MCGameState::sharedGameState()->load();
        mc_object_id_t m_id = {
            'M', '0', '0', '2'
        };
        
        CCScene *scene = (CCScene *) MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);
        
        MCTestEffect *s = new MCTestEffect;
        s->initWithColor(ccc4(0, 0, 0, 0));
        scene->addChild(s);
        
        return scene;
    }
    
    void onEnter() {
        MCTestLayer::onEnter();
        
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        cache->addSpriteFramesWithFile("effects/curse1.plist");
        CCSpriteFrame *frame;
        CCArray *animFrames = CCArray::create();
        char str[100] = {0};
        
        for(int i = 1; ; i++) {
            sprintf(str, "curse1_%02d", i);
            frame = cache->spriteFrameByName( str );
            CC_BREAK_IF(! frame);
            animFrames->addObject(frame);
        }
        
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames);
        animation->setDelayPerUnit(1.0 / animFrames->count());
        CCAnimate *animate = CCAnimate::create(animation);
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("curse1_01");
        addChild(sprite);
        sprite->setPosition(ccp(100, 100));
        sprite->runAction(animate);
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

#endif /* defined(__Military_Confrontation__MCTestEffect__) */
