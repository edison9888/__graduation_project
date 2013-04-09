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
    
    inline bool atEntrance() {
        return atEntrance_;
    }
    
    inline void setAtEntrance(bool var) {
        atEntrance_ = var;
    }
    
    /**
     * 在主角面前的人物
     * 判断当前视野中的人物，然后返回最近的那个
     */
    MCRole *roleOfFront();
    
    CCObject *copy() { return copyWithZone(0); }
private:
    bool atEntrance_;
};

#endif /* defined(__Military_Confrontation__MCHero__) */
