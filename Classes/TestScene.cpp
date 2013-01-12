#include "TestScene.h"

#include "MenuItemLabelColor.h"
#include "CCButton.h"
#include "CCJoypad.h"
using namespace joypad;

#include "CCDebug.h"
#include "CCSpriteColor.h"
static CCBuffer *buf = CCBuffer::create ();
static CCLabelTTF *output = NULL;
static unsigned int c = 0;
static CCButton *buttonB;

class MySneakyJoystickCallback : public CCSpiritedJoystickHandler
{

    void onMoveUp(CCSpiritedJoystick *joystick)
    {
        output->setString ("up");
//        qlog("up");
    }

    virtual void onMoveDown(CCSpiritedJoystick *joystick)
    {
        output->setString ("down");
//        qlog("down");
    }
    virtual void onMoveLeft(CCSpiritedJoystick *joystick)
    {
        output->setString ("left");
//        qlog("left");
    }
    virtual void onMoveRight(CCSpiritedJoystick *joystick)
    {
        output->setString ("right");
//        qlog("right");
    }
    virtual void onMoveUpLeft(CCSpiritedJoystick *joystick)
    {
        output->setString ("up left");
//        qlog("up left");
    }
    virtual void onMoveUpRight(CCSpiritedJoystick *joystick)
    {
        output->setString ("up right");
//        qlog("up right");
    }
    virtual void onMoveDownLeft(CCSpiritedJoystick *joystick)
    {
        output->setString ("down left");
//        qlog("down left");
    }
    virtual void onMoveDownRight(CCSpiritedJoystick *joystick)
    {
        output->setString ("down right");
//        qlog("down right");
    }
};

bool
TestLayer::init()
{
    if (CCLayerColor::initWithColor (ccc4 (32, 32, 32, 128)))
    {
        CCSize ws = CCDirector::sharedDirector ()->getWinSize ();

        CCLabelTTF *label = CCLabelTTF::create ("Test AnywhereJoypad", "arial", 32);
        label->setColor (ccc3 (64, 128, 240));
        this->addChild (label);
        label->setPosition (ccp (ws.width / 2, label->getContentSize ().height));

        output = CCLabelTTF::create ("Touch the screen plz", "arial", 32);
        this->addChild (output);
        output->setPosition (ccp (ws.width / 2, ws.height - output->getContentSize ().height));

        CCSpiritedJoystick *joystick;
        CCSprite *bg = CCSprite::create ("joystick-background.png");
        CCSprite *control = CCSprite::create ("joystick-control.png");
        CCJoypad *controller;
        MySneakyJoystickCallback *sneakyjoystickCallback = new MySneakyJoystickCallback;

        joystick = CCSpiritedJoystick::joystickWithResources (bg, control);

        controller = CCJoypad::create ();
        controller->setJoystick (joystick);
        controller->setSneakyJoystickHandler (sneakyjoystickCallback);

        CCScale9Sprite *normal = CCScale9Sprite::create ("button-a.png");
        CCScale9Sprite *pressed = CCScale9Sprite::create ("button-a-pressed.png");
        CCButton *button = CCButton::create (normal, pressed);
        button->addTargetWithActionForControlEvents ((CCLayerColor *) this,
                                                     cccontrol_selector(TestLayer::buttonA_clicked),
                                                     CCControlEventTouchUpInside);
        controller->addChild (button, 1);
        button->setPosition (ccp (ws.width - normal->getContentSize ().width * 2.5,
                                   normal->getContentSize ().height * 1.5));

        normal = CCScale9Sprite::create ("button-b.png");
        pressed = CCScale9Sprite::create ("button-b-pressed.png");
        button = CCButton::create (normal, pressed);
        button->addTargetWithActionForControlEvents ((CCLayerColor *) this,
                                                     cccontrol_selector(TestLayer::buttonB_clicked),
                                                     CCControlEventTouchUpInside);
        controller->addChild (button, 1);
        button->setPosition (ccp (ws.width - normal->getContentSize ().width * 1.5,
                                   normal->getContentSize ().height * 2.5));
        buttonB = button;

        this->addChild (controller);

        label = CCLabelTTF::create ("Back", "Arial", 32);
        MenuItemLabelColor *menuItem = MenuItemLabelColor::create (label,
                                                      this,
													  menu_selector(TestLayer::popScene));
        menuItem->setPosition (ccp (ws.width - menuItem->getContentSize().width * 2 / 3,
                                    menuItem->getContentSize().height));

        CCMenu *menu = CCMenu::createWithItem (menuItem);
        menu->setPosition (CCPointZero);
        this->addChild (menu);

//        schedule (schedule_selector(TestLayer::update), 0.3);

        return true;
    }

    return false;
}

void TestLayer::buttonA_clicked(CCObject* object, CCControlEvent event)
{
    qlog("Button A was clicked");
    output->setString ("Button A was clicked");
}

void TestLayer::buttonB_clicked(CCObject* object, CCControlEvent event)
{
    qlog("Button B was clicked");
    output->setString ("Button B was clicked");
}

void TestLayer::popScene(CCObject* object)
{
	CCDirector::sharedDirector ()->popScene();
}

void TestLayer::update (float dt)
{
    CCTouch *touch;

    if (c++ % 2 == 0)
    {
        touch = new CCTouch;
        touch->setTouchInfo (0, 710, 310);
        buttonB->ccTouchBegan (touch, NULL);
    }
    else
    {
        touch = new CCTouch;
        touch->setTouchInfo (0, 710, 310);
        buttonB->ccTouchEnded (touch, NULL);
    }
}