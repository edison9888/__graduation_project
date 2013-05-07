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
#if MC_SELECT_ALL_SUPPORT == 1
    void selectAll();
#endif
    void unselectAll();
    void selectRole(MCRole *aRole);
    void unselectRole(MCRole *aRole);
    
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
    void toggle();
    
    MCRoleBaseInfoGroup *group_;
    
    CCMenu *toggleButton_;
    
#if MC_SELECT_ALL_SUPPORT == 1
    CC_SYNTHESIZE_READONLY(CCArray *, selectedRoles_, SelectedRoles);
#else
    CC_SYNTHESIZE_READONLY(MCRole *, selectedRole_, SelectedRole);
#endif
    
    CC_PROPERTY_READONLY_PASS_BY_REF(CCSize, frameSize_, FrameSize);
};

#endif /* defined(__Military_Confrontation__MCTeamLayer__) */
