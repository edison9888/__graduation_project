#include "CCJoypad.h"

using namespace joypad;

#include "CCDebug.h"
static CCBuffer *buf = CCBuffer::create ();

#ifndef absf
#define absf(x) ((x) > 0 ? (x) : -(x))
#endif

static CCPoint s_pJoystickTrailer = CCPointZero;

#define POINT_EQUAL(p1, p2) (((p1).x == (p2).x) && ((p1).y == (p2).y))

static void
getIntersectionPoint(CCPoint linePoint, float circleRadius,
                     float& intersectionPointX, float& intersectionPointY)
{
    float k = linePoint.x / linePoint.y;
    float squareOfRadius = circleRadius * circleRadius;
    float squareOfY = squareOfRadius / (k * k + 1);
    float squareOfX = squareOfRadius  - squareOfY;

    intersectionPointX = sqrtf (squareOfX);
    intersectionPointY = sqrtf (squareOfY);
}

bool
CCJoypad::init()
{
    if (CCLayer::init ())
    {
        this->setTouchEnabled (true);

        return true;
    }

    return false;
}

CCSpiritedJoystick *
CCJoypad::getJoystick ()
{
    return m_sjJoystick;
}

void
CCJoypad::setJoystick(CCSpiritedJoystick *joystick)
{
    this->removeChild (m_sjJoystick, true);
    m_sjJoystick = joystick;
    joystick->setVisible (false);
    this->addChild (joystick, 1);
}

void
CCJoypad::ccTouchesBegan (CCSet *pTouches, CCEvent *pEvent)
{
    CC_UNUSED_PARAM (pEvent);

    CCTouch *pTouch = (CCTouch *) pTouches->anyObject ();
    CCPoint location;

    location = CCDirector::sharedDirector ()->convertToGL (pTouch->getLocationInView ());

    if (! m_sjJoystick->isVisible ())
    {
        m_sjJoystick->showAtLocation (location);
        m_pJoystickPosition = location;
        s_pJoystickTrailer.x = location.x;
        s_pJoystickTrailer.y = location.y;

        m_sjJoystick->setPosition (location);
    }
}

void
CCJoypad::ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent)
{
    CC_UNUSED_PARAM (pEvent);

    CCTouch *pTouch;
    CCPoint location;
    CCPoint prevousLocation;
    float offsetX;
    float offsetY;
    float offset;
    float radius;
    float intersectionPointX;
    float intersectionPointY;
    CCSetIterator iter;

    for (iter = pTouches->begin (); iter != pTouches->end (); ++iter)
    {
        pTouch = (CCTouch *) *iter;
        location = CCDirector::sharedDirector ()->convertToGL (pTouch->getLocationInView ());
        prevousLocation = CCDirector::sharedDirector ()->convertToGL (pTouch->getPreviousLocationInView ());

        if (POINT_EQUAL (s_pJoystickTrailer, prevousLocation)) //移动joystick的点
        {
            s_pJoystickTrailer.x = location.x;
            s_pJoystickTrailer.y = location.y;
            offsetX = location.x -= m_pJoystickPosition.x;
            offsetY = location.y -= m_pJoystickPosition.y;
            offset = sqrtf (offsetX * offsetX + offsetY * offsetY);

            if (offset > m_sjJoystick->getThreshold ()) //有效偏移
            {
                radius = m_sjJoystick->getRadius ();
                if (absf (offsetX) > radius || absf (offsetY) > radius) //越界
                {
                    getIntersectionPoint (location, radius,
                                          intersectionPointX, intersectionPointY);
                    location.x = offsetX > 0 ? intersectionPointX : -intersectionPointX;
                    location.y = offsetY > 0 ? intersectionPointY : -intersectionPointY;
                }

                //do handle
                if (m_sjJoystick->isVisible ()  /* 响应了按钮后不响应joystick */
                        && m_sjcJoystickHandler != NULL)
                {
                    m_sjJoystick->handleMoving (location, m_sjcJoystickHandler);
                }
            }
            m_sjJoystick->moveControl (location);
            break;
        }
    }
}

void
CCJoypad::ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent)
{
    CC_UNUSED_PARAM (pTouches);
    CC_UNUSED_PARAM (pEvent);

    CCTouch *pTouch = (CCTouch *) pTouches->anyObject ();
    CCPoint location;
    CCPoint prevousLocation;

    location = CCDirector::sharedDirector ()->convertToGL (pTouch->getLocationInView ());
    prevousLocation = CCDirector::sharedDirector ()->convertToGL (pTouch->getPreviousLocationInView ());


    if (POINT_EQUAL (s_pJoystickTrailer, prevousLocation)) //移动joystick的点
    {
        s_pJoystickTrailer.x = 0;
        s_pJoystickTrailer.y = 0;
        m_sjJoystick->hide ();
    }
}
