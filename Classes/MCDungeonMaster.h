//
//  MCDungeonMaster.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCDungeonMaster__
#define __Military_Confrontation__MCDungeonMaster__

#include <cocos2d.h>

USING_NS_CC;

class MCDungeonMaster : public CCObject {
private:
    MCDungeonMaster() { };
    
    bool _init();
    
public:
    static MCDungeonMaster *sharedDungeonMaster();
};

#endif /* defined(__Military_Confrontation__MCDungeonMaster__) */
