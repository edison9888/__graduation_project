#include "CCButton.h"

#include "CCDebug.h"

using namespace joypad;

bool
CCButton::initWithSprites (CCScale9Sprite *sprite, CCScale9Sprite *pressedSprite)
{
    if (CCControlButton::init ())
    {
        this->setPreferredSize (sprite->getContentSize ());
        this->setBackgroundSpriteForState (sprite, CCControlStateNormal);
        this->setBackgroundSpriteForState (pressedSprite, CCControlStateHighlighted);

        return true;
    }

    return false;
}

CCButton *
CCButton::create (CCScale9Sprite *sprite, CCScale9Sprite *pressedSprite)
{
    CCButton *button;

    button = new CCButton;
    if (button && button->initWithSprites (sprite, pressedSprite))
    {
        button->autorelease ();
    }

    return button;
}
