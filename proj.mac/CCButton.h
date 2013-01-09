#ifndef CCJOYPADBUTTON_H
#define CCJOYPADBUTTON_H

#include <cocos2d.h>
#include <cocos-ext.h>
USING_NS_CC;
USING_NS_CC_EXT;

NS_CC_BEGIN
namespace joypad {
    class CCButton : public CCControlButton
    {
    public:
        virtual bool initWithSprites (CCScale9Sprite *sprite, CCScale9Sprite *pressedSprite);

        static CCButton *create (CCScale9Sprite *sprite, CCScale9Sprite *pressedSprite);
    };
}

NS_CC_END

#endif // CCJOYPADBUTTON_H
