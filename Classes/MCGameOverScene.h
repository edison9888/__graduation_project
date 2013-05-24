//
//  MCGameOverScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCGameOverScene__
#define __Military_Confrontation__MCGameOverScene__

#include "MCConfirm.h"

class MCGameOver : public CCLayerColor, public MCConfirmDelegate {
    
public:
    bool init();
    
    CREATE_FUNC(MCGameOver);
    
    static CCScene *scene() {
        CCScene *scene = CCScene::create();
        
        scene->addChild(MCGameOver::create());
        
        return scene;
    }
    
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    
protected:
    void continueGame(CCObject *anObject);
    void exit(CCObject *anObject);
    
private:
};

#endif /* defined(__Military_Confrontation__MCGameOverScene__) */
