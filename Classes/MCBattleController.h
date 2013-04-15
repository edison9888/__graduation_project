//
//  MCBattleController.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBattleController__
#define __Military_Confrontation__MCBattleController__

#include "MCTeamLayer.h"
#include "MCControllerDelegate.h"
#include "MCActionBar.h"

class MCBattleController : public CCLayer {
    
public:
    bool init();
    
    CREATE_FUNC(MCBattleController);
    
    CCArray *getSelectedRoles();
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    
protected:
    void didSelectAll(CCObject *aSender);
    
private:
    MCTeamLayer *teamLayer_; /* 队伍层 */
    MCActionBar *actionBar_; /* 道具层 */
    
    MCActionBarItem *selectedItem_;
    
    /* 选择按钮 */
    CCMenu *selectAllMenu_;
    CCSprite *multiSelection_;
    CCTouch *multiSelectionTouch_;
    
    CC_SYNTHESIZE(MCBattleControllerDelegate *, delegate_, Delegate);
};

#endif /* defined(__Military_Confrontation__MCBattleController__) */
