//
//  MCTeam.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTeam__
#define __Military_Confrontation__MCTeam__

#include "MCRoleBaseInfo.h"

class MCTeam : public CCLayer {
public:
    ~MCTeam();
    bool init();
    
    CREATE_FUNC(MCTeam);
    
    void selectAll();
    void unselectAll();
    void selectRole(MCRole *aRole);
    void unselectRole(MCRole *aRole);
    
    MCRoleBaseInfo *infoForRole(MCRole *aRole);
    bool hasRole(MCRole *aRole);
    void addRole(MCRole *aRole);
    void removeRole(MCRole *aRole);
    mc_size_t size();
    
    inline bool isMultiSeletionMode() {
        return isMultiSeletionMode_;
    }
    
    inline void setMultiSeletionMode(bool var) {
        isMultiSeletionMode_ = var;
    }
    
private:
    MCRoleBaseInfoGroup *group_;
    bool isMultiSeletionMode_;
    
    CC_SYNTHESIZE_READONLY(CCArray *, selecredRoles_, SelecredRoles);
};

#endif /* defined(__Military_Confrontation__MCTeam__) */
