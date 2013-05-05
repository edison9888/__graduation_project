//
//  MCPropsLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCPropsLayer__
#define __Military_Confrontation__MCPropsLayer__

#include "MCBaseInfoLayer.h"

class MCPropsLayer : public MCBaseInfoLayer {
public:
    bool init();
    
    void clickFirstItem();
        
    CREATE_FUNC(MCPropsLayer);
    
protected:
    void loadData();
    void itemDidClick(CCObject *obj);
    
private:
    CCMenu *items_;
    CCMenuItemLabel *lastClickedItem_;
    CCMenuItem *firstItem_;
    
    CCLabelTTF *name_;
    CCLabelTTF *description_;
    CCLabelTTF *count_;
    CCSprite *icon_;
};

#endif /* defined(__Military_Confrontation__MCPropsLayer__) */
