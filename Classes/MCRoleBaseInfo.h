//
//  MCRoleBaseInfo.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRoleBaseInfo__
#define __Military_Confrontation__MCRoleBaseInfo__

#include <cocos2d.h>
USING_NS_CC;
#include <cocos-ext.h>
USING_NS_CC_EXT;

#include "MCRole.h"
#include "MCTeam.h"

/**
 * BaseInfo指的是HP和PP值
 */
class MCRoleBaseInfo : public CCLayer {
public:
    bool init(MCRole *aRole);
    
    static MCRoleBaseInfo *create(MCRole *aRole);
    
    inline bool isSelected() {
        return faceBoxSelected_->isVisible();
    }
    
    inline void setSelected(bool var) {
        faceBoxSelected_->setVisible(var);
    }
    
    inline void selected() {
        faceBoxSelected_->setVisible(true);
    }
    
    inline void unselected() {
        faceBoxSelected_->setVisible(false);
    }
    
    inline CCScale9Sprite *getFaceBox() {
        return faceBox_;
    }
    
private:
    CCSprite *face_;
    CCScale9Sprite *faceBox_;
    CCScale9Sprite *faceBoxSelected_;
    CCLabelTTF *hpLabel_;
    CCLabelTTF *maxHPLabel_;
    CCLabelTTF *ppLabel_;
    CCLabelTTF *maxPPLabel_;
    
    CC_SYNTHESIZE_READONLY(MCRole *, role_, Role);
};

class MCTeamLayer;

class MCRoleBaseInfoGroup : public CCLayer {
    friend class MCTeamLayer;
public:
    MCRoleBaseInfoGroup();
    ~MCRoleBaseInfoGroup();
    
    CREATE_FUNC(MCRoleBaseInfoGroup);
    
    void addRoleBaseInfo(MCRoleBaseInfo *anInfo);
    void removeRoleBaseInfo(MCRoleBaseInfo *anInfo);
    mc_size_t size();
    
protected:
    void align();
    
private:
    MCTeam *team_;
    CCArray *infoList_;
};

#endif /* defined(__Military_Confrontation__MCRoleBaseInfo__) */
