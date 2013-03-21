//
//  MCTeam.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTeam__
#define __Military_Confrontation__MCTeam__

#include "MCRole.h"

class MCTeam {
private:
    MCTeam();
public:
    ~MCTeam();
    
    static MCTeam *sharedTeam();
    
    CCArray *getRoles();
    MCRole *roleAtIndex(mc_index_t anIndex);
    
    bool hasRole(MCRole *aRole);
    void addRole(MCRole *aRole);
    void removeRole(MCRole *aRole);
    mc_size_t size();
    
private:
    CCArray *teams_;
    
    CC_SYNTHESIZE(mc_size_t, maxSize_, MaxSize);
};

#endif /* defined(__Military_Confrontation__MCTeam__) */
