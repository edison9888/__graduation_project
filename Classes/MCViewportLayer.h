//
//  MCViewportLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCViewportLayer__
#define __Military_Confrontation__MCViewportLayer__

#include <cocos2d.h>
USING_NS_CC;

class MCViewportLayer : public CCLayer {
    
public:
    bool init();
    
    CREATE_FUNC(MCViewportLayer);
    
    void draw(void);
    
public:
    CCArray *roles;
};

#endif /* defined(__Military_Confrontation__MCViewportLayer__) */
