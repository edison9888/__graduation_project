#ifndef CCJOYPAD_H
#define CCJOYPAD_H

#include <cocos2d.h>
USING_NS_CC;

#include "CCSpiritedJoystick.h"

NS_CC_BEGIN

namespace joypad {

    class CCJoypad : public CCLayer
    {
    public:
        CCJoypad():
            m_sjcJoystickHandler(NULL)
        {
        }

        virtual bool init();

        inline CCSpiritedJoystickHandler *
        setSneakyJoystickHandler(CCSpiritedJoystickHandler *sneakyjoystickHandler)
        {
            CCSpiritedJoystickHandler *oldCallback = m_sjcJoystickHandler;

            m_sjcJoystickHandler = sneakyjoystickHandler;

            return oldCallback;
        }

        CREATE_FUNC(CCJoypad);

        virtual void ccTouchesBegan (CCSet *pTouches, CCEvent *pEvent);
        virtual void ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent);
        virtual void ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent);

    private:
        CCSpiritedJoystickHandler *m_sjcJoystickHandler;

        CC_PROPERTY (CCSpiritedJoystick *, m_sjJoystick, Joystick)
        CC_SYNTHESIZE_PASS_BY_REF (CCPoint, m_pJoystickPosition, JoystickPosition)
    };
}

NS_CC_END

#endif // CCJOYPAD_H
