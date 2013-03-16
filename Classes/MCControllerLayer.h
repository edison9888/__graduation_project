//
//  MCControllerLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCControllerLayer__
#define __Military_Confrontation__MCControllerLayer__

#include "MCControllerDelegate.h"

#include "MCJoypad.h"

#include "MCActionMenu.h"

class MCObjectDataSource;

class MCControllerLayer : public CCLayer, public MCActionMenuDelegate {
    
public:
    bool init();
    
    CREATE_FUNC(MCControllerLayer);
    
    MCActionMenu *actionMenu();
    
    void actionMenuDidOpen(MCActionMenu *anActionMenu);
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    
private:
    MCJoypad *joypad_;
    
    CC_PROPERTY(MCControllerDelegate *, delegate_, Delegate);
    CC_SYNTHESIZE(MCObjectDataSource *, objectDataSource_, ObjectDataSource);
    CC_SYNTHESIZE(MCActionMenu *, actionMenu_, ActionMenu);
};

#endif /* defined(__Military_Confrontation__MCControllerLayer__) */
