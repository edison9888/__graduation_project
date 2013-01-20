#include "MCSpiritedJoystick.h"

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

using namespace joypad;

bool
MCSpiritedJoystick::initWithResources(CCSprite *background,
                                CCSprite *control)
{
    CCSize size;

    if (background && control && CCSprite::init ())
    {
        m_sBackground = background;
        m_sControl = control;

        this->addChild (background);
        this->addChild (control);
        m_fRadius = background->getContentSize ().width / 2;

        return true;
    }

    return false;
}

MCSpiritedJoystick *
MCSpiritedJoystick::joystickWithResources(CCSprite *background,
                                      CCSprite *control)
{
    MCSpiritedJoystick *joystick;

    joystick = new MCSpiritedJoystick;
    if (joystick && joystick->initWithResources (background, control))
    {
        joystick->autorelease ();
    }

    return joystick;
}

void
MCSpiritedJoystick::moveControl(CCPoint offsetPosition)
{
    m_sControl->setPosition (offsetPosition);
}

void MCSpiritedJoystick::handleMoving(CCPoint point, MCSpiritedJoystickHandler *handler)
{
    float angle;
    JoystickDirection direction;

    //每个方向分配45度角的空间
    getAngle (point.y, point.x, angle);
    if (angle < 22.5f)
    {
        if (point.x > 0)
            direction = JoystickDirectionRight;
        else
            direction = JoystickDirectionLeft;
    }
    else if (angle < 67.5f)
    {
        if (point.x > 0)
            if (point.y > 0)
                direction = JoystickDirectionUpRight;
            else
                direction = JoystickDirectionDownRight;
        else
            if (point.y > 0)
                direction = JoystickDirectionUpLeft;
            else
                direction = JoystickDirectionDownLeft;
    }
    else
    {
        if (point.y > 0)
            direction = JoystickDirectionUp;
        else
            direction = JoystickDirectionDown;
    }
    handler->onMove (this, direction);
}
