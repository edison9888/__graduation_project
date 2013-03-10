//
//  MCKeyboard.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCKeyboard.h"

bool
MCKeyboard::init()
{
    MCKeyboardDispatcher::sharedKeyboardDispatcher()->registerKayboardDelegate(this);
    
    return true;
}

void
MCKeyboard::keyPressed(unsigned short keycode)
{
    if (! delegate_) {
        return;
    }

//    CCLog("%d", keycode);
    CCPoint offset = CCPointZero;
    delegate_->controllerDidPress(delegate_);
    switch (keycode) {
        case MCKeyUp:
            offset.y = 4;
            break;
        case MCKeyDown:
            offset.y = -4;
            break;
        case MCKeyLeft:
            offset.x = -4;
            break;
        case MCKeyRight:
            offset.x = 4;
            break;
            
        default:
            break;
    }
    delegate_->controllerMove(delegate_, offset);
}

void
MCKeyboard::keyReleased(unsigned short keycode)
{
    delegate_->controllerDidRelease(delegate_);
}
