//
//  MCJoystick.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright(c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCJoystick.h"

#ifndef absf
#define absf(x)((x) > 0 ?(x) : -(x))
#endif

const float PI = 3.1415926f;

/**
  * a: point.y
  * b: point.x
  * tan(angle) = a / b
  * angle = atan(a / b)
  */
inline static void
getAngle(float a, float b, float& angle)
{
    float theta;

    a = absf(a);
    b = absf(b);
    theta = atan(a / b);

    if ((int) b == 0) { //b不能等于0，否则atan不能计算
        angle = 90.0f;
    } else {
        angle = theta * 180 / PI;
    }
}

bool
MCJoystick::initWithResources(CCSprite *background,
                              CCSprite *control)
{
    CCSize size;

    if (background && control && CCSprite::init()) {
        background_ = background;
        control_ = control;

        this->addChild(background);
        this->addChild(control);
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
    if (joystick && joystick->initWithResources(background, control)) {
        joystick->autorelease();
    }

    return joystick;
}

void
MCJoystick::moveControl(CCPoint offsetPosition)
{
    control_->setPosition(offsetPosition);
}
