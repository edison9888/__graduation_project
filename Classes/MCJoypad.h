#ifndef MCJoypad_H
#define MCJoypad_H

#include <cocos2d.h>
USING_NS_CC;

#include "MCSpiritedJoystick.h"

NS_CC_BEGIN

namespace joypad {

    class MCJoypad : public CCLayer
    {
    public:
        MCJoypad():
            m_sjcJoystickHandler(NULL)
        {
        }

        virtual bool init();

        inline MCSpiritedJoystickHandler *
        setSneakyJoystickHandler(MCSpiritedJoystickHandler *sneakyjoystickHandler)
        {
            MCSpiritedJoystickHandler *oldCallback = m_sjcJoystickHandler;

            m_sjcJoystickHandler = sneakyjoystickHandler;

            return oldCallback;
        }

        CREATE_FUNC(MCJoypad);

        virtual void ccTouchesBegan (CCSet *pTouches, CCEvent *pEvent);
        virtual void ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent);
        virtual void ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent);

    private:
        MCSpiritedJoystickHandler *m_sjcJoystickHandler;

        CC_PROPERTY (MCSpiritedJoystick *, m_sjJoystick, Joystick)
        CC_SYNTHESIZE_PASS_BY_REF (CCPoint, m_pJoystickPosition, JoystickPosition)
    };
}

NS_CC_END

#endif // MCJoypad_H
