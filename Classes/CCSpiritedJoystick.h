#ifndef SNEAKYJOYSTICK_H
#define SNEAKYJOYSTICK_H

#include <cocos2d.h>

USING_NS_CC;

NS_CC_BEGIN

namespace joypad {

    class CCSpiritedJoystickHandler;

    /**
 * Joystick示例图：
 *  .--|++++++++)
 *  .原点 -无效区域 |生效阀值 +有效区域 )边界
 */
    class CCSpiritedJoystick : public CCSprite
    {
    public:
        CCSpiritedJoystick():m_fThreshold(13) {}

        virtual bool initWithResources(CCSprite *background, CCSprite *control);

        static CCSpiritedJoystick *joystickWithResources(CCSprite *background, CCSprite *control);

        void moveControl(CCPoint offsetPosition);
        void handleMoving(CCPoint point, CCSpiritedJoystickHandler *handler);

        inline void showAtLocation(CCPoint location)
        {
            this->setVisible (true);
            this->setCenter (location);
        }

        inline void hide()
        {
            CCSize size = getContentSize ();
            this->setVisible (false);
            m_sControl->setPosition (ccp (size.width / 2, size.height));
        }

    private:
        CCSprite *m_sBackground;
        CCSprite *m_sControl;

        CC_SYNTHESIZE_READONLY (float, m_fRadius, Radius)
        CC_SYNTHESIZE (float, m_fThreshold, Threshold)
        CC_SYNTHESIZE_PASS_BY_REF( CCPoint, m_pCenter, Center)
    };

    typedef enum {
        JoystickDirectionUp = 1,
        JoystickDirectionDown,
        JoystickDirectionLeft,
        JoystickDirectionRight,
        JoystickDirectionUpLeft,
        JoystickDirectionUpRight,
        JoystickDirectionDownLeft,
        JoystickDirectionDownRight
    } JoystickDirection;

    class CCSpiritedJoystickHandler : public CCObject
    {
    public:
        void onMove(CCSpiritedJoystick *joystick, JoystickDirection direction)
        {
            switch (direction)
            {
            case JoystickDirectionUp:
                onMoveUp (joystick);
                break;
            case JoystickDirectionDown:
                onMoveDown (joystick);
                break;
            case JoystickDirectionLeft:
                onMoveLeft (joystick);
                break;
            case JoystickDirectionRight:
                onMoveRight (joystick);
                break;
            case JoystickDirectionUpLeft:
                onMoveUpLeft (joystick);
                break;
            case JoystickDirectionUpRight:
                onMoveUpRight (joystick);
                break;
            case JoystickDirectionDownLeft:
                onMoveDownLeft (joystick);
                break;
            case JoystickDirectionDownRight:
                onMoveDownRight (joystick);
                break;
            }
        }

        virtual void onMoveUp(CCSpiritedJoystick *joystick)=0;
        virtual void onMoveDown(CCSpiritedJoystick *joystick)=0;
        virtual void onMoveLeft(CCSpiritedJoystick *joystick)=0;
        virtual void onMoveRight(CCSpiritedJoystick *joystick)=0;
        virtual void onMoveUpLeft(CCSpiritedJoystick *joystick)=0;
        virtual void onMoveUpRight(CCSpiritedJoystick *joystick)=0;
        virtual void onMoveDownLeft(CCSpiritedJoystick *joystick)=0;
        virtual void onMoveDownRight(CCSpiritedJoystick *joystick)=0;
    };
}

NS_CC_END

#endif // SNEAKYJOYSTICK_H
