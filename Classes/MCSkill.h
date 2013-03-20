//
//  MCSkill.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSkill__
#define __Military_Confrontation__MCSkill__

#include "MCObject.h"

class MCSkillManager;

class MCSkill : public MCObject {
    friend class MCSkillManager;
    
public:
    void fire();
    
private:
    void skillDidHit();
    
    CC_SYNTHESIZE(CCAnimation *, effect_, Effect);
};

#endif /* defined(__Military_Confrontation__MCSkill__) */
