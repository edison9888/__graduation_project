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

/* 基本物件 */
class MCControllerDelegate {
public:
    virtual void controllerMoveUp(MCControllerDelegate *sender) = 0;
    virtual void controllerMoveDown(MCControllerDelegate *sender) = 0;
    virtual void controllerMoveLeft(MCControllerDelegate *sender) = 0;
    virtual void controllerMoveRight(MCControllerDelegate *sender) = 0;
    
    virtual void controllerMoveUpLeft(MCControllerDelegate *sender);
    virtual void controllerMoveUpRight(MCControllerDelegate *sender);
    virtual void controllerMoveDownLeft(MCControllerDelegate *sender);
    virtual void controllerMoveDownRight(MCControllerDelegate *sender);
    
    virtual void controllerMove(MCControllerDelegate *sender, const CCPoint &delta) = 0;
    
    virtual void controllerDidPress(MCControllerDelegate *sender);
    virtual void controllerDidRelease(MCControllerDelegate *sender);
    
    virtual void controllerDidClickEsc();
    virtual void controllerDidClickEnter();
    virtual void controllerDidClickMenu();
    virtual void controllerDidClickBack();
};

#endif /* defined(__Military_Confrontation__MCControllerDelegate__) */
