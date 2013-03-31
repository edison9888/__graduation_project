//
//  MCTestParticle.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-31.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestParticle__
#define __Military_Confrontation__MCTestParticle__

#include "MCTestLayer.h"
#include "MCSceneManager.h"
#include "MCHero.h"

class MCTestParticle : public MCTestLayer {
public:
    
    static CCScene *scene() {
        mc_object_id_t m_id = {
            'M', '0', '0', '1'
        };

        CCScene *scene = (CCScene *) MCSceneManager::sharedSceneManager()->sceneWithObjectId(m_id);
        
        MCTestParticle *s = new MCTestParticle;
        s->initWithColor(ccc4(0, 0, 0, 0));
        scene->addChild(s);
        
        return scene;
    }
    
    void onEnter() {
        MCTestLayer::onEnter();
        
        CCParticleSun *galaxy = CCParticleSun::createWithTotalParticles(200);
        galaxy->setEmitterMode(kCCParticleModeRadius);
        galaxy->setEndRadius(64.0f);
        galaxy->setPosition(ccp(200, 200));
        CCSize s = galaxy->getContentSize();
        addChild(galaxy);
    }
    
    void onExit() {
        MCTestLayer::onExit();
        MCHero::sharedHero()->getEntity()->getSpriteSheet()->removeFromParent();
    }
};

#endif /* defined(__Military_Confrontation__MCTestParticle__) */
