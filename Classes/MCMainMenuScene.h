//
//  MCMainMenuScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMainMenuScene__
#define __Military_Confrontation__MCMainMenuScene__

#include <cocos2d.h>

USING_NS_CC;

class MCMainMenu : public CCLayerColor {
    
public:
    bool init();
    
    CREATE_FUNC(MCMainMenu);
    
    static CCScene *scene() {
        CCScene *scene = CCScene::create();
        
        scene->addChild(MCMainMenu::create());
        
        return scene;
    }
    
protected:
    void playGame_clicked(CCObject* sender);    /* 单人游戏 */
    void multiplayer_clicked(CCObject* sender); /* 多人对战 */
    void about_clicked(CCObject* sender);       /* 关于 */
    void quit_clicked(CCObject* sender);        /* 退出 */
    
private:
};

#endif /* defined(__Military_Confrontation__MCMainMenuScene__) */
