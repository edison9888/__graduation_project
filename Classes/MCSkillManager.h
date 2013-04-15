//
//  MCSkillManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSkillManager__
#define __Military_Confrontation__MCSkillManager__

#include "MCSkill.h"

class MCSkillManager {
private:
    MCSkillManager();
    
public:
    ~MCSkillManager();
    static MCSkillManager *sharedSkillManager();
    
    MCSkill *skillForObjectId(mc_object_id_t anObjectId);
    
    void saveData();
    void loadData();
    
private:
    
private:
    CCDictionary *skills_;
};

#endif /* defined(__Military_Confrontation__MCSkillManager__) */
