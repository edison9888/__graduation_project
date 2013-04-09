//
//  MCCamera.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCCamera__
#define __Military_Confrontation__MCCamera__

#include <cocos2d.h>
USING_NS_CC;

class MCSceneDelegate;
class MCRole;

class MCCamera {
public:
    MCCamera();
    
    /**
     * 重置位置，必须先设置场景代理
     */
    void restore();
    
    /**
     * 设置位置，必须先设置场景代理
     */
    void locate();
    
    /**
     * 聚焦到某人身上，必须先设置场景代理
     */
    void focus(MCRole *aRole);
    void focusHero();
    
    inline void translate(float x, float y) {
        viewport_.origin.x -= x;
        viewport_.origin.y -= y;
    }
    
    inline void translate(const CCPoint &anOffset) {
        viewport_.origin = ccpSub(viewport_.origin, anOffset);
    }
    
    CC_SYNTHESIZE(CCPoint, location_, Location); /* 场景默认位置 */
    CC_SYNTHESIZE_READONLY(CCRect, viewport_, Viewport); /* 宽高为窗口大小，坐标是窗口在地图的坐标 */
    CC_PROPERTY(MCSceneDelegate *, sceneDelegate_, SceneDelegate);
    
private:
    DISALLOW_COPY_AND_ASSIGN(MCCamera);
};

#endif /* defined(__Military_Confrontation__MCCamera__) */
