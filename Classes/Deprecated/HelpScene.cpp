#include "HelpScene.h"

#include "MenuItemLabelColor.h"

USING_NS_CC;

CCScene* Help::scene()
{
    CCScene *scene = CCScene::create ();

    Help *layer = Help::create ();

    scene->addChild(layer);

    return scene;
}

bool Help::init()
{
    CCLabelTTF *label;
    CCMenu *menu;
    CCMenuItem *menuItem;
    CCSize winSize;
    float lastLabelPositionX, lastLabelPositionY;

    if (CCLayer::init ())
    {
        winSize = CCDirector::sharedDirector ()->getWinSize ();

        //Game's name
        label = CCLabelTTF::create ("Adventure", "Arial", 64);
        label->setPosition (ccp (winSize.width / 2,
                                 winSize.height * 3 / 4));
        label->getPosition (&lastLabelPositionX, &lastLabelPositionY);
        this->addChild (label);

        //Version
        label = CCLabelTTF::create (" Version: 0.1 ", "Arial", 32);
        label->setPosition (ccp (winSize.width / 2,
                                 lastLabelPositionY - label->getContentSize().height * 2));
        label->getPosition (&lastLabelPositionX, &lastLabelPositionY);
        this->addChild (label);

        //Author
        label = CCLabelTTF::create ("Author: DF", "Arial", 32);
        label->setPosition (ccp (winSize.width / 2,
                                 lastLabelPositionY - label->getContentSize().height));
        label->getPosition (&lastLabelPositionX, &lastLabelPositionY);
        this->addChild (label);

        //Email
        label = CCLabelTTF::create ("EMail: lapazzavita@gmail.com", "Arial", 32);
        label->setPosition (ccp (winSize.width / 2,
                                 lastLabelPositionY - label->getContentSize().height));
        this->addChild (label);


        //add back menu item
        label = CCLabelTTF::create ("Back", "Arial", 48);
        menuItem = MenuItemLabelColor::create (label,
                                                      this,
                                                      menu_selector(Help::menu_backCallback));
        menuItem->setPosition (ccp (winSize.width - menuItem->getContentSize().width,
                                    menuItem->getContentSize().height));

        menu = CCMenu::create (menuItem);
        menu->setPosition (CCPointZero);
        this->addChild (menu);

        return true;
    }

    return false;
}

void Help::menu_backCallback(CCObject *pSender)
{
    CCDirector::sharedDirector ()->popScene ();
}

void
Help::onEnter ()
{
    CCLayer::onEnter ();
    schedule (schedule_selector(Help::update));
}

void
Help::onExit ()
{
    CCLayer::onExit ();
    unschedule (schedule_selector(Help::update));
}
