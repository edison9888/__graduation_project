//
//  MCJoystick.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCJoystick.h"

#ifndef absf
#define absf(x) ((x) > 0 ? (x) : -(x))
#endif

const float PI = 3.1415926f;

/**
  * a: point.y
  * b: point.x
  * tan(angle) = a / b
  * angle = atan (a / b)
  */
inline static void
getAngle(float a, float b, float& angle)
{
    float theta;

    a = absf (a);
    b = absf (b);
    theta = atan (a / b);

    if ((int) b == 0) //b不能等于0，否则atan不能计算
    {
        angle = 90.0f;
    }
    else
    {
        angle = theta * 180 / PI;
    }
}

bool
MCJoystick::initWithResources(CCSprite *background,
                              CCSprite *control)
{
    CCSize size;

    if (background && control && CCSprite::init())
    {
        background_ = background;
        control_ = control;

        this->addChild (background);
        this->addChild (control);
        radius_ = background->getContentSize().width / 2;

        return true;
    }

    return false;
}

MCJoystick *
MCJoystick::create(CCSprite *background,
                   CCSprite *control)
{
    MCJoystick *joystick;

    joystick = new MCJoystick;
    if (joystick && joystick->initWithResources (background, control))
    {
        joystick->autorelease();
    }

    return joystick;
}

void
MCJoystick::moveControl(CCPoint offsetPosition)
{
    control_->setPosition (offsetPosition);
}

//void
//MCJoystick::handleMoving(CCPoint *point)
//{
//    float angle;
//    
//    if (! delegate_) {
//        return;
//    }
//    
//    if (! delegate_) {
//        return;
//    }
//
//    //每个方向分配45度角的空间
//    getAngle (point.y, point.x, angle);
//    if (angle < 22.5f) {
//        if (point.x > 0) {
//            delegate_->controllerMoveRight(delegate_);
//        } else {
//            delegate_->controllerMoveLeft(delegate_);
//        }
//    }
//#if (0) /* 不开启8-Pad */
//    else if (angle < 67.5f) {
//        if (point.x > 0)
//            if (point.y > 0)
//                direction = JoystickDirectionUpRight;
//            else
//                direction = JoystickDirectionDownRight;
//        else
//            if (point.y > 0)
//                direction = JoystickDirectionUpLeft;
//            else
//                direction = JoystickDirectionDownLeft;
//    }
//#endif /*  */
//    else {
//        if (point.y > 0) {
//            delegate_->controllerMoveUp(delegate_);
//        } else {
//            delegate_->controllerMoveDown(delegate_);
//        }
//    }
//}
