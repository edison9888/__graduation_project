//
//  MCSplashScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSplashScene__
#define __Military_Confrontation__MCSplashScene__

#include <cocos2d.h>

USING_NS_CC;

class MCSplash : public CCLayerColor {
    
public:
    bool init();
    
    CREATE_FUNC(MCSplash);
    
    static CCScene *scene()
    {
        CCScene *scene = CCScene::create();
        
        scene->addChild(MCSplash::create());
        
        return scene;
    }
    
    void onEnter();
    
protected:
    void finish(CCNode* node);
    
private:
};

#endif /* defined(__Military_Confrontation__MCSplashScene__) */
