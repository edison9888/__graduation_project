//
//  MCJoypad.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCJoypad.h"

static CCPoint s_pJoystickTrailer = CCPointZero;
static CCPoint *__cc_point_zero = new CCPoint(0.f, 0.f);
static CCPoint *__mc_joystick_delta = new CCPoint(0.f, 0.f);

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
        isValidControl_ = false;
        setTouchEnabled(true);
//        setTouchMode(kCCTouchesAllAtOnce);

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
                if (fabsf (offsetX) > radius || fabsf (offsetY) > radius) //越界
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
    CCLayer::ccTouchesEnded(pTouches, pEvent);
}

void
MCJoypad::onEnter()
{
    CCLayer::onEnter();
    schedule(schedule_selector(MCJoystick::update));
}

void
MCJoypad::onExit()
{
    unschedule(schedule_selector(MCJoystick::update));
    CCLayer::onExit();
}

void
MCJoypad::update(float dt)
{
    if (! delegate_ || !isValidControl_) {
        return;
    }
    delegate_->controllerMove(delegate_, *joystickDelta_);
}

