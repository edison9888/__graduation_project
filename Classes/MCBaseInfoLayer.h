//
//  MCBaseInfoLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBaseInfoLayer__
#define __Military_Confrontation__MCBaseInfoLayer__

#include <cocos2d.h>
USING_NS_CC;

class MCBaseInfoLayer : public CCLayer {
public:
    virtual void show();
    virtual void hide();
    
    void onEnter();
    
protected:
    virtual void loadData() {}
};

#endif /* defined(__Military_Confrontation__MCBaseInfoLayer__) */
