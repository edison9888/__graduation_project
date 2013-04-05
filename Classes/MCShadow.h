//
//  MCShadow.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-1.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCShadow__
#define __Military_Confrontation__MCShadow__

#include "MCRole.h"

class MCShadow : public CCSprite {
public:
    MCShadow();
    ~MCShadow();
    
    bool init();
    
    CREATE_FUNC(MCShadow);
    
    void bind(MCRole *aRole);
    
    MCRole *unbind();
    
    void selected();
    void unselected();
    
    void updatePosition();
    
    inline MCRole *getRole() {
        return role_;
    }
    
private:
    MCRole *role_;
    
    CCSpriteFrame *shadow_;
    CCSpriteFrame *selectedShadow_;
};

#endif /* defined(__Military_Confrontation__MCShadow__) */
