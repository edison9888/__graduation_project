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
    MCControlButtonUp     = MCMakeEnum(0),
    MCControlButtonDown   = MCMakeEnum(1),
    MCControlButtonLeft   = MCMakeEnum(2),
    MCControlButtonRight  = MCMakeEnum(3),
    
    MCControlButtonEsc    = MCMakeEnum(4),
    MCControlButtonEnter  = MCMakeEnum(5),
    MCControlButtonMenu   = MCMakeEnum(6),
    MCControlButtonBack   = MCMakeEnum(7)
};
typedef mc_enum_t MCControlButtonCode;

/* 基本物件 */
class MCControllerDelegate {
public:
    virtual void controllerMoveUp(MCControllerDelegate *sender) = 0;
    virtual void controllerMoveDown(MCControllerDelegate *sender) = 0;
    virtual void controllerMoveLeft(MCControllerDelegate *sender) = 0;
    virtual void controllerMoveRight(MCControllerDelegate *sender) = 0;
    
    virtual void controllerDidClickEsc();
    virtual void controllerDidClickEnter();
    virtual void controllerDidClickMenu();
    virtual void controllerDidClickBack();
};

#endif /* defined(__Military_Confrontation__MCControllerDelegate__) */
