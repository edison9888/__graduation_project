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

class MCMezzanine : public CCLayer {
    friend class MCScene;
public:
    ~MCMezzanine();
    
    bool init();
    
    void onEnter();
    void onExit();
    
    void flow(float dt);
    
    void attach(MCSkill *aSkill);
    
    CREATE_FUNC(MCMezzanine);
    
private:
    void detectsSkills(MCRole *aRole);
    
    void skillDidFinish(CCObject *anObject);
    
    CC_SYNTHESIZE(CCArray *, skills_, Skills);
};

#endif /* defined(__Military_Confrontation__MCMezzanine__) */
