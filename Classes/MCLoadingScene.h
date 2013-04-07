//
//  MCLoadingScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-6.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCLoadingScene__
#define __Military_Confrontation__MCLoadingScene__

#include <cocos2d.h>

USING_NS_CC;

class MCLoading : public CCLayerColor {
public:
    bool init();
    
    static MCLoading *sharedLoading();
    
    void show();
    void hide();
    
protected:
    void loading(float dt);
    
    CCLabelTTF *points_;
};


#endif /* defined(__Military_Confrontation__MCLoadingScene__) */
