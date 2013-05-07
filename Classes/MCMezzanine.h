//
//  MCMezzanine.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMezzanine__
#define __Military_Confrontation__MCMezzanine__

#include "MCScene.h"
#include "MCEffect.h"

class MCSkill;
class MCTrap;

class MCMezzanine : public CCLayer {
    friend class MCScene;
public:
    ~MCMezzanine();
    
    bool init();
    
    void draw();
    
    void drawDraggingTrap(MCTrap *aTrap);
    
    void installTrap(MCTrap *aTrap); /* 已设置好位置的陷阱 */
    
    /* aLocation为屏幕上位置 */
    void installTrap(MCTrap *aTrap, const CCPoint &aLocation);
    
    void onEnter();
    void onExit();
    
    void flow(float dt);
    
    void attach(MCSkill *aSkill);
    
    CREATE_FUNC(MCMezzanine);
    
private:
    void detectsSkills(MCRole *aRole);
    void skillDidFinish(CCObject *anObject);
    
    void detectsTraps(MCRole *aRole);
    void trapDidTriggerByRole(MCTrap *aTrap, MCRole *aRole);
    
    CCDrawNode *drawer_;
    
    CC_SYNTHESIZE(CCArray *, skills_, Skills);
    CC_SYNTHESIZE(CCArray *, traps_, Traps);
    
    CC_SYNTHESIZE(MCTrap *, draggingTrap_, DraggingTrap);
};

#endif /* defined(__Military_Confrontation__MCMezzanine__) */
