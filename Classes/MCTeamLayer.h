//
//  MCTeamLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTeamLayer__
#define __Military_Confrontation__MCTeamLayer__

#include "MCRoleBaseInfo.h"

class MCTeamLayer : public CCLayer {
public:
    ~MCTeamLayer();
    bool init();
    
    CREATE_FUNC(MCTeamLayer);
    
    MCRoleBaseInfo *infoForRole(MCRole *aRole);
    bool hasRole(MCRole *aRole);
    void addRole(MCRole *aRole);
    void removeRole(MCRole *aRole);
    mc_size_t size();
    
    /* 选择控制 */
    void selectAll();
    void unselectAll();
    void selectRole(MCRole *aRole);
    void unselectRole(MCRole *aRole);
    
    inline bool isMultiSeletionMode() {
        return isMultiSeletionMode_;
    }
    
    inline void setMultiSeletionMode(bool var) {
        isMultiSeletionMode_ = var;
    }
    
    inline MCTeam *getTeam() {
        return group_->team_;
    }
    
private:
    MCRoleBaseInfoGroup *group_;
    bool isMultiSeletionMode_;
    
    CC_SYNTHESIZE_READONLY(CCArray *, selecredRoles_, SelecredRoles);
};

#endif /* defined(__Military_Confrontation__MCTeamLayer__) */
