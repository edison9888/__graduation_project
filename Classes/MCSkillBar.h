//
//  MCSkillBar.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSkillBar__
#define __Military_Confrontation__MCSkillBar__

#include "MCSkill.h"
#include <cocos2d.h>
USING_NS_CC;

extern const GLubyte kMCSkillBarItemColdTimeOpacity;
extern const GLubyte kMCSkillBarItemPassiveSkillOpacity;
extern const GLubyte kMCSkillBarItemActiveSkillOpacity;

extern const char *kMCSkillBarVisibleDidChangeNotification;

class MCSkillBarItem : public CCSprite {
public:
    ~MCSkillBarItem();
    
    bool init(MCSkill *aSkill);
    
    static MCSkillBarItem *create(MCSkill *aSkill);
    
    /* cold time */
    void intoColdTime();
    bool inColdTime();
    
    bool selectable();
    
    inline bool isSelected() {
        return isSelected_;
    }
    
    inline void selected() {
        isSelected_ = true;
    }
    
    inline void unselected() {
        isSelected_ = false;
    }
    
private:
    void coldTimeDidFinish(CCObject *anObject);
    
    CC_SYNTHESIZE_READONLY(CCLabelTTF *, pp_, PP);
    CC_SYNTHESIZE_READONLY(MCSkill *, skill_, Skill);
    
    bool isSelected_;
};

class MCSkillBarItemGroup : public CCObject {
    friend class MCSkillBar;
public:
    ~MCSkillBarItemGroup();
    
    void init(MCRole *aRole);
    
    static MCSkillBarItemGroup *create(MCRole *aRole);
    
    /* 技能最多就4个 */
    CC_SYNTHESIZE_READONLY(MCSkillBarItem *, skillBarItemA_, SkillBarItemA);
    CC_SYNTHESIZE_READONLY(MCSkillBarItem *, skillBarItemB_, SkillBarItemB);
    CC_SYNTHESIZE_READONLY(MCSkillBarItem *, skillBarItemC_, SkillBarItemC);
    CC_SYNTHESIZE_READONLY(MCSkillBarItem *, skillBarItemD_, SkillBarItemD);
};

class MCSkillBar : public CCLayer {
public:
    ~MCSkillBar();
    
    bool init();
    
    CREATE_FUNC(MCSkillBar);
    
    void toggle();
    
    bool isHidden();
    
    void showSkillsForRole(MCRole *aRole);
    
    MCSkillBarItem *itemForTouch(CCTouch *pTouch);
    
private:
    void cleanOld();
    void align();
    
    CCMenu *toggleButton_;
    
    CC_SYNTHESIZE_READONLY(CCDictionary *, skillBarItemGroups_, MCSkillBarItemGroups);
    CC_SYNTHESIZE_READONLY(MCSkillBarItemGroup *, currentSkillBarItemGroup_, CurrentSkillBarItemGroup);
};


#endif /* defined(__Military_Confrontation__MCSkillBar__) */
