//
//  MCMainMenuScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMainMenuScene__
#define __Military_Confrontation__MCMainMenuScene__

#include "MCConfirm.h"
#include "MCType.h"

enum {
    MCUnknownOperation      = 0,
    MCQuitOperation         = MCMakeEnum(0),
    MCStartNewGameOperation = MCMakeEnum(1)
};
typedef mc_enum_t MCMainMenuConfirmOperation;

class MCMainMenu : public CCLayerColor, public MCConfirmDelegate {
    
public:
    bool init();
    
    CREATE_FUNC(MCMainMenu);
    
    static CCScene *scene() {
        CCScene *scene = CCScene::create();
        
        scene->addChild(MCMainMenu::create());
        
        return scene;
    }
    
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    
protected:
    void continue_clicked(CCObject *aSender);    /* 继续游戏 */
    void playGame_clicked(CCObject *aSender);    /* 单人游戏 */
#if MC_MULTIPLAYER_SUPPORT == 1
    void multiplayer_clicked(CCObject *aSender); /* 多人对战 */
#endif
    void about_clicked(CCObject *aSender);       /* 关于 */
    void quit_clicked(CCObject *aSender);        /* 退出 */
    
private:
    MCMainMenuConfirmOperation confirmOperation_;
};

#endif /* defined(__Military_Confrontation__MCMainMenuScene__) */
