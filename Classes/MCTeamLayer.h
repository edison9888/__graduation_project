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

class MCActionBarItem;

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
    void selectFirstRole();
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
    
    MCRoleBaseInfo *roleBaseInfoForTouch(CCTouch *aTouch);
    
    /**
     * 确实发生碰撞返回碰撞的人物，否则返回NULL
     */
    MCRoleBaseInfo *collidesWithActionBarItem(MCActionBarItem *anActionBarItem);
    
    /**
     * 名字有点蛋疼，其实意思是检测item和人物头像的碰撞，若有碰撞则半透明化
     */
    void acceptActionBarItem(MCActionBarItem *anActionBarItem);
    
    
    void selectedRolesUseActionBarItem(MCActionBarItem *anActionBarItem);
    
private:
    MCRoleBaseInfoGroup *group_;
    bool isMultiSeletionMode_;
    
    CC_SYNTHESIZE_READONLY(CCArray *, selectedRoles_, SelectedRoles);
};

#endif /* defined(__Military_Confrontation__MCTeamLayer__) */
