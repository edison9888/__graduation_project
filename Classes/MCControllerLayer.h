//
//  MCControllerLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCControllerLayer__
#define __Military_Confrontation__MCControllerLayer__

#include <cocos2d.h>
USING_NS_CC;

class MCControllerLayer : public CCLayer {
public:
    virtual bool isEnable() { return true; }
    virtual void setEnable(bool var) {}
};

#endif /* defined(__Military_Confrontation__MCControllerLayer__) */
