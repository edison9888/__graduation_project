//
//  MCActionBar.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCActionBar__
#define __Military_Confrontation__MCActionBar__

#include "MCBackpack.h"
#include <cocos2d.h>
USING_NS_CC;

extern const GLubyte kMCInvalidActionBarItemOpacity;
extern const GLubyte kMCDraggingActionBarItemOpacity;
extern const GLubyte kMCNormalActionBarItemOpacity;

class MCActionBarItem : public CCSprite {
public:
    bool init(MCBackpackItem *aBackpackItem);
    
    static MCActionBarItem *create(MCBackpackItem *aBackpackItem);
    
    void resetPosition();
    
    CCPoint touchedPoint;
    struct cc_timeval timestamp;
    
    void updateCount();
    
    inline bool isTrap() {
        return backpackItem_->item->getID().class_ == 'T';
    }
    
private:
    CC_SYNTHESIZE_READONLY(CCLabelTTF *, count_, Count);
    
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, backpackItem_, BackpackItem);
    
    CC_PROPERTY(CCPoint, itemPosition_, ItemPosition);
};

class MCActionBar : public CCLayer {
public:
    ~MCActionBar();
    
    bool init();
    
    CREATE_FUNC(MCActionBar);
    
    void toggle();
    
    MCActionBarItem *itemForTouch(CCTouch *pTouch);
    
private:
    void align();
    
    CCMenu *toggleButton_;
    
    CC_SYNTHESIZE_READONLY(MCActionBarItem *, trapWide_, TrapWide);
    CC_SYNTHESIZE_READONLY(MCActionBarItem *, trapDamage_, TrapDamage);
    CC_SYNTHESIZE_READONLY(MCActionBarItem *, healthPotion_, HealthPotion);
    CC_SYNTHESIZE_READONLY(MCActionBarItem *, physicalPotion_, PhysicalPotion);
};

#endif /* defined(__Military_Confrontation__MCActionBar__) */
