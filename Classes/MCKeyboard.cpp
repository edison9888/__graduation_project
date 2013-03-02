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
    
    delegate_->controllerDidPress(delegate_);
    switch (keycode) {
        case MCKeyUp:
            delegate_->controllerMoveUp(delegate_);
            break;
        case MCKeyDown:
            delegate_->controllerMoveDown(delegate_);
            break;
        case MCKeyLeft:
            delegate_->controllerMoveLeft(delegate_);
            break;
        case MCKeyRight:
            delegate_->controllerMoveRight(delegate_);
            break;
            
        default:
            break;
    }
}

void
MCKeyboard::keyReleased(unsigned short keycode)
{
    delegate_->controllerDidRelease(delegate_);
}
