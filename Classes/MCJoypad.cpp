//
//  MCJoypad.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCJoypad.h"

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

static CCPoint s_pJoystickTrailer = CCPointZero;
static CCPoint *__cc_point_zero = new CCPoint(0.f, 0.f);
static CCPoint *__mc_joystick_delta = new CCPoint(0.f, 0.f);

#define POINT_EQUAL(p1, p2) (((p1).x == (p2).x) && ((p1).y == (p2).y))

static void
getIntersectionPoint(CCPoint linePoint, float circleRadius,
                     float& intersectionPointX, float& intersectionPointY)
{
    float k = linePoint.x / linePoint.y;
    float squareOfRadius = circleRadius * circleRadius;
    float squareOfY = squareOfRadius / (k * k + 1);
    float squareOfX = squareOfRadius  - squareOfY;

    intersectionPointX = sqrtf(squareOfX);
    intersectionPointY = sqrtf(squareOfY);
}

MCJoypad::~MCJoypad()
{
}

bool
MCJoypad::init()
{
    if (CCLayer::init())
    {
        joystickDelta_ = __cc_point_zero;
        setTouchEnabled(true);
        setTouchMode(kCCTouchesAllAtOnce);
        schedule(schedule_selector(MCJoystick::update));

        return true;
    }

    return false;
}

MCJoystick *
MCJoypad::getJoystick()
{
    return joystick_;
}

void
MCJoypad::setJoystick(MCJoystick *joystick)
{
    this->removeChild (joystick_, true);
    joystick_ = joystick;
    joystick->setVisible (false);
    this->addChild (joystick, 1);
}

void
MCJoypad::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CC_UNUSED_PARAM (pEvent);

    CCTouch *pTouch = (CCTouch *) pTouches->anyObject();
    CCPoint location;
    
    CCLayer::ccTouchesBegan(pTouches, pEvent);
    
    controlTouch_ = pTouch;

    location = CCDirector::sharedDirector()->convertToGL (pTouch->getLocationInView());

    if (! joystick_->isVisible())
    {
        joystick_->showAtLocation (location);
        joystickPosition_ = location;
        s_pJoystickTrailer.x = location.x;
        s_pJoystickTrailer.y = location.y;

        joystick_->setPosition (location);
        joystickDelta_ = __mc_joystick_delta;
    }
}

void
MCJoypad::ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent)
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
    
    CCLayer::ccTouchesMoved(pTouches, pEvent);

    for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
    {
        pTouch = (CCTouch *) *iter;
        location = CCDirector::sharedDirector()->convertToGL (pTouch->getLocationInView());
        prevousLocation = CCDirector::sharedDirector()->convertToGL (pTouch->getPreviousLocationInView());

        if (s_pJoystickTrailer.equals(prevousLocation)) //移动joystick的点
        {
            s_pJoystickTrailer.x = location.x;
            s_pJoystickTrailer.y = location.y;
//            prevousLocation.x = location.x;
//            prevousLocation.y = location.y;
            offsetX = location.x -= joystickPosition_.x;
            offsetY = location.y -= joystickPosition_.y;
            offset = sqrtf (offsetX * offsetX + offsetY * offsetY);

            if (offset > joystick_->getThreshold()) //有效偏移
            {
                radius = joystick_->getRadius();
                if (absf (offsetX) > radius || absf (offsetY) > radius) //越界
                {
                    getIntersectionPoint (location, radius,
                                          intersectionPointX, intersectionPointY);
                    location.x = offsetX > 0 ? intersectionPointX : -intersectionPointX;
                    location.y = offsetY > 0 ? intersectionPointY : -intersectionPointY;
                }

                /* do handle */
                if (joystick_->isVisible()) { /* 响应了按钮后不响应joystick */
                    isValidControl_ = true;
                    joystickDelta_->x = location.x;
                    joystickDelta_->y = location.y;
                } else {
                    isValidControl_ = false;
                }
            }
            joystick_->moveControl (location);
            break;
        }
    }
}

void
MCJoypad::ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent)
{
    CC_UNUSED_PARAM (pTouches);
    CC_UNUSED_PARAM (pEvent);

    CCTouch *pTouch = (CCTouch *) pTouches->anyObject();
    CCPoint location;
    CCPoint prevousLocation;

    location = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    prevousLocation = CCDirector::sharedDirector()->convertToGL(pTouch->getPreviousLocationInView());

    if (s_pJoystickTrailer.equals(prevousLocation)) //移动joystick的点
    {
        joystick_->hide();
        s_pJoystickTrailer.x = 0;
        s_pJoystickTrailer.y = 0;
        joystickDelta_ = __cc_point_zero;
        __mc_joystick_delta->x = 0;
        __mc_joystick_delta->y = 0;
    }
    isValidControl_ = false;
}

void
MCJoypad::update(float dt)
{
    float angle;

    if (! delegate_ || !isValidControl_) {
        return;
    }
    CCPoint point = *joystickDelta_;
    
    //每个方向分配60度角的空间
    getAngle (point.y, point.x, angle);
    if (angle < 30.f) {
        if (point.x > 0) {
            delegate_->controllerMoveRight(delegate_);
        } else {
            delegate_->controllerMoveLeft(delegate_);
        }
    } else if (angle > 60.f) {
        if (point.y > 0) {
            delegate_->controllerMoveUp(delegate_);
        } else {
            delegate_->controllerMoveDown(delegate_);
        }
    }

}

