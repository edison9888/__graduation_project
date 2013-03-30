//
//  MCStateLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCStatusLayer__
#define __Military_Confrontation__MCStatusLayer__

#include "MCDetailLayer.h"

class MCStateLayer : public MCDetailLayer {
public:
    bool init();
    
    CREATE_FUNC(MCStateLayer);
    
protected:
    void loadData();
    
private:
    CCLabelTTF *hp_;
    CCLabelTTF *pp_;
    CCLabelTTF *state_;
    
    CCLabelTTF *damage_;
    CCLabelTTF *ac_;
    
    CCLabelTTF *money_;
    
    CCLabelTTF *name_;
    CCSprite *icon_;
};
#endif /* defined(__Military_Confrontation__MCStatusLayer__) */
