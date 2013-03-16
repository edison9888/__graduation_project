//
//  MCControllerDelegate.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCControllerDelegate__
#define __Military_Confrontation__MCControllerDelegate__

#include <cocos2d.h>
#include "MCType.h"

USING_NS_CC;

enum {
    MCControlButtonUp           = MCMakeEnum(0),
    MCControlButtonDown         = MCMakeEnum(1),
    MCControlButtonLeft         = MCMakeEnum(2),
    MCControlButtonRight        = MCMakeEnum(3),
    
    MCControlButtonUpLeft       = MCMakeEnum(4),
    MCControlButtonUpRight      = MCMakeEnum(5),
    MCControlButtonDownLeft     = MCMakeEnum(6),
    MCControlButtonDownRight    = MCMakeEnum(7),
    
    MCControlButtonEsc          = MCMakeEnum(8),
    MCControlButtonEnter        = MCMakeEnum(9),
    MCControlButtonMenu         = MCMakeEnum(10),
    MCControlButtonBack         = MCMakeEnum(11)
};
typedef mc_enum_t MCControlButtonCode;

class MCRole;

/* 基本物件 */
class MCControllerDelegate {
public:
    /* joypad */
    virtual void controllerMove(MCControllerDelegate *sender, const CCPoint &delta) {}
    
    /* free */
    virtual void controllerDidSelectRole(MCRole *aRole) {}
};

#endif /* defined(__Military_Confrontation__MCControllerDelegate__) */
