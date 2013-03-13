//
//  MCHero.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCHero__
#define __Military_Confrontation__MCHero__

#include "MCRole.h"

/* 主角 */
class MCHero : public MCRole {
public:
    ~MCHero();
    bool init();
    
    static MCHero *sharedHero();
};

#endif /* defined(__Military_Confrontation__MCHero__) */
