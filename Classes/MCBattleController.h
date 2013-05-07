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
#include "MCSkillBar.h"

class MCBattleController : public CCLayer {
    void pointToEnded(CCObject *obj);
public:
    ~MCBattleController();
    bool init();
    
    CREATE_FUNC(MCBattleController);
    
#if MC_SELECT_ALL_SUPPORT == 1
    CCArray *getSelectedRoles();
#else
    MCRole *getSelectedRole();
#endif
    
    /* 选中敌人 */
    void selectTarget(MCRole *aRole);
    
    inline bool isJoypadEnable() {
        return isJoypadEnabled_;
    }
    
    inline void setJoypadEnable(bool var) {
        isJoypadEnabled_ = var;
    }
    
    bool isItemTouchedForTouch(CCTouch *aTouch);
    
    void onEnter();
    void onExit();
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    
protected:
#if MC_SELECT_ALL_SUPPORT == 1
    void didSelectAll(CCObject *aSender);
    
    /* 选择按钮 */
    CCMenu *selectAllMenu_;
#endif
    void skillBarVisibleDidChange(CCObject *anObject);
    void installTrap(CCObject *anObject);
    
    MCTeamLayer *teamLayer_; /* 队伍层 */
    MCActionBar *actionBar_; /* 道具栏 */
    MCSkillBar *skillBar_; /* 道具栏 */
    
    MCActionBarItem *selectedActionBarItem_;
    
    MCSkillBarItem *selectedSkillBarItem_;
    CCParticleSystemQuad *skillBarItemSelectedEffect_;
    MCSkillBarItem *lastSelectedSkillBarItem_;
    
    bool isJoypadEnabled_;
    bool isDragging_;
    
    struct cc_timeval lastTouchedTime_;
    
    CC_SYNTHESIZE(MCBattleControllerDelegate *, delegate_, Delegate);
};

#endif /* defined(__Military_Confrontation__MCBattleController__) */
