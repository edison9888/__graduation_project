//
//  MCBackgroundLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBackgroundLayer__
#define __Military_Confrontation__MCBackgroundLayer__

#include <cocos2d.h>
USING_NS_CC;

class MCTeam;

class MCBackgroundLayer : public CCLayer {
    
public:
    MCBackgroundLayer()
    : isPlayBackgroundMusicImmediately_(true)
    , enemyShadows_(NULL)
    , shadows_(NULL) {}
    
    ~MCBackgroundLayer();
    bool init(const char *aMapFilePath, const char *aBackgroundMusicFilePath);
    
    static MCBackgroundLayer *create(const char *aMapFilePath, const char *aBackgroundMusicFilePath);
    
    void onEnter();
    void onExit();
    
    void update(float dt);
    
    void playMusic();
    void stopMusic();
    
    void loadEnemies(CCArray *objects);
    void loadTeam(MCTeam *aTeam);
    
    inline bool isPlayBackgroundMusicImmediately() {
        return isPlayBackgroundMusicImmediately_;
    }
    
    inline void setPlayBackgroundMusicImmediately(bool var) {
        isPlayBackgroundMusicImmediately_ = var;
    }
    
    CC_SYNTHESIZE_READONLY(CCTMXTiledMap *, map_, Map);
    CC_SYNTHESIZE_READONLY(CCString *, backgroundMusic_, BackgroundMusic);
    
private:
    bool isPlayBackgroundMusicImmediately_;
    
    CCArray *enemyShadows_; /* 敌人影子 */
    CCArray *shadows_; /* 主角和佣兵的影子 */
};

#endif /* defined(__Military_Confrontation__MCBackgroundLayer__) */
