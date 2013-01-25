//
//  MCJoystick.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCJoystick__
#define __Military_Confrontation__MCJoystick__

#include <cocos2d.h>
USING_NS_CC;

/**
 * Joystick示例图：
 *  .--|++++++++)
 *  .原点 -无效区域 |生效阀值 +有效区域 )边界
 */
class MCJoystick : public CCSprite
{
public:
    MCJoystick():threshold_(13) {}
    
    virtual bool initWithResources(CCSprite *background, CCSprite *control);
    
    static MCJoystick *create(CCSprite *background, CCSprite *control);
    
    void moveControl(CCPoint offsetPosition);
    
    inline void showAtLocation(CCPoint location)
    {
        this->setVisible (true);
        this->setCenter (location);
    }
    
    inline void hide()
    {
        CCSize size = getContentSize ();
        this->setVisible (false);
        control_->setPosition (ccp (size.width / 2, size.height));
    }
    
private:
    CCSprite *background_;
    CCSprite *control_;
    
    CC_SYNTHESIZE_READONLY (float, radius_, Radius)
    CC_SYNTHESIZE (float, threshold_, Threshold)
    CC_SYNTHESIZE_PASS_BY_REF( CCPoint, center_, Center)
};

#endif /* defined(__Military_Confrontation__MCJoystick__) */
