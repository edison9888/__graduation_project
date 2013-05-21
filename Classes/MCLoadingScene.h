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
    ~MCLoading();
    bool init();
    
    static MCLoading *sharedLoading();
    
    /**
     * 执行过程
     * 1.注册通知中心
     * 2.显示loading
     * 3.加载场景
     * 4.发出加载完成的通知
     * 5.隐藏loading
     * 6.切换场景
     */
    void loadNewScene();
    
protected:
    void loading(float dt);
    void sceneDidLoad(CCObject *obj);
    void didHide();
    
    CCLabelTTF *points_;
};


#endif /* defined(__Military_Confrontation__MCLoadingScene__) */
