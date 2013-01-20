#include "SimpleAudioEngine.h"

#include "MenuScene.h"
#include "HelpScene.h"
#include "ExitDialog.h"

#include "TestScene.h"
#include "CCSpriteColor.h"

#include "MenuItemLabelColor.h"

#include "CCButton.h"
using namespace joypad;

#include "CCDebug.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

const int PlayMenuItem_Tag = 0x01;
const int LoadMenuItem_Tag = 0x02;
const int HelpMenuItem_Tag = 0x03;
const int ExitMenuItem_Tag = 0x04;
const int SoundOnMenuItem_Tag = 0x05;
const int SoundOffMenuItem_Tag = 0x06;
const int BGMToggleMenuItem_Tag = 0x07;
const int SecondaryMenu_Tag = 0x20;
const int ExitDialog_Tag = 0x40;

CCScene*
MenuLayer::scene()
{
    CCScene *scene = CCScene::create ();

    MenuLayer *layer = MenuLayer::create ();

    scene->addChild(layer);

    return scene;
}

MenuLayer::~MenuLayer()
{
}

bool
MenuLayer::init()
{
    CCMenu *menu;
    CCLabelTTF *label;
    CCMenuItem *testMenuItem;
    CCMenuItem *playMenuItem;
    CCMenuItem *loadMenuItem;
    CCMenuItem *helpMenuItem;
    CCMenuItem *exitMenuItem;
    CCSize winSize;

    if (CCLayer::init())
    {
        winSize = CCDirector::sharedDirector ()->getWinSize ();

        label = CCLabelTTF::create ("Test Joypad", "Arial", 48);
        testMenuItem = MenuItemLabelColor::create (label, this,
                                                          menu_selector(MenuLayer::menuItem_testCallback));

        label = CCLabelTTF::create ("PLAY", "Arial", 48);
        playMenuItem = MenuItemLabelColor::create (label, this,
                                                          menu_selector(MenuLayer::menuItem_playCallback));
        playMenuItem->setTag(PlayMenuItem_Tag);

        label = CCLabelTTF::create ("LOAD", "Arial", 48);
        loadMenuItem = MenuItemLabelColor::create (label, this,
                                                          menu_selector(MenuLayer::menuItem_loadCallback));
        loadMenuItem->setTag(LoadMenuItem_Tag);

        label = CCLabelTTF::create ("HELP", "Arial", 48);
        helpMenuItem = MenuItemLabelColor::create (label, this,
                                                          menu_selector(MenuLayer::menuItem_helpCallback));
        helpMenuItem->setTag(HelpMenuItem_Tag);

        label = CCLabelTTF::create ("EXIT", "Arial", 48);
        exitMenuItem = MenuItemLabelColor::create (label, this,
                                                           menu_selector(MenuLayer::menuItem_quitCallback));
        exitMenuItem->setTag(ExitMenuItem_Tag);

        menu = CCMenu::create (testMenuItem,
                               playMenuItem,
                               loadMenuItem,
                               helpMenuItem,
                               exitMenuItem, NULL);
        menu->alignItemsVertically ();

        this->addChild (menu);
        this->menu = menu;

        label = CCLabelTTF::create ("Sound: ON", "Arial", 32);
        MenuItemLabelColor *menuItem = MenuItemLabelColor::create (label);
        menuItem->setTag (SoundOnMenuItem_Tag);
        CCMenuItemToggle *toggleBGM = CCMenuItemToggle::create (menuItem);
        toggleBGM->setPosition (ccp (winSize.width - toggleBGM->getContentSize().width * 2 / 3,
                                    toggleBGM->getContentSize().height));
        label = CCLabelTTF::create ("Sound: OFF", "Arial", 32);
        menuItem = MenuItemLabelColor::create (label);
        menuItem->setTag (SoundOffMenuItem_Tag);
        toggleBGM->addSubItem (menuItem);
        toggleBGM->setTarget (this, menu_selector(MenuLayer::menuItem_toggleMusic));
        toggleBGM->setTag (BGMToggleMenuItem_Tag);

        CCMenu *secondaryMenu = CCMenu::createWithItem (toggleBGM);
        secondaryMenu->setTag (SecondaryMenu_Tag);
        secondaryMenu->setPosition (CCPointZero);
        this->addChild (secondaryMenu);

        toggleBGM->activate ();

        return true;
    }

    return false;
}

void MenuLayer::menuItem_testCallback(CCObject *pSender)
{
    CC_UNUSED_PARAM (pSender);

    CCDirector::sharedDirector ()->pushScene (TestLayer::scene ());
}

void
MenuLayer::menuItem_playCallback(CCObject* pSender)
{
    CC_UNUSED_PARAM (pSender);
}

void
MenuLayer::menuItem_loadCallback(CCObject* pSender)
{
    CC_UNUSED_PARAM (pSender);
}

void
MenuLayer::menuItem_helpCallback(CCObject* pSender)
{
    CC_UNUSED_PARAM (pSender);

    CCDirector::sharedDirector ()->pushScene (Help::scene ());
}

void
MenuLayer::menuItem_quitCallback(CCObject* pSender)
{
    CC_UNUSED_PARAM (pSender);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize ();
    ExitDialog *exitDialog = new ExitDialog;

    exitDialog->init ();
    exitDialog->autorelease ();
    exitDialog->setTag (ExitDialog_Tag);
    exitDialog->getYes ()->setTarget (this, menu_selector (MenuLayer::menuItem_exitYesCallback));
    exitDialog->getNo ()->setTarget (this, menu_selector (MenuLayer::menuItem_exitNoCallback));
    this->addChild (exitDialog);

    menu->setTouchEnabled (false);

    exitDialog->showDialog ();
}

void
MenuLayer::menuItem_exitYesCallback(CCObject* pSender)
{
    CC_UNUSED_PARAM (pSender);

    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    exit(0);
#endif
}

void
MenuLayer::menuItem_exitNoCallback(CCObject* pSender)
{
    CC_UNUSED_PARAM (pSender);

    ExitDialog *exitDialog = (ExitDialog *) this->getChildByTag (ExitDialog_Tag);

    exitDialog->runAction (CCSequence
                           ::create (CCCallFunc::create (exitDialog,
                                                         callfunc_selector(ExitDialog::hideDialog)),
                                     CCDelayTime::create (0.5),
                                     CCCallFuncN::create (this,
                                                          callfuncN_selector(MenuLayer::exitDialog_close)),
                                     NULL));
}
void
MenuLayer::exitDialog_close(CCNode* pSender)
{
    CC_UNUSED_PARAM (pSender);

    menu->setTouchEnabled (true);
    this->removeChildByTag (ExitDialog_Tag, false);
}

void
MenuLayer::menuItem_toggleMusic(CCObject *pSender)
{
    CC_UNUSED_PARAM (pSender);

    CCMenu *secondaryMenu = (CCMenu *) getChildByTag (SecondaryMenu_Tag);
    CCMenuItemToggle *toggle = (CCMenuItemToggle *) secondaryMenu->getChildByTag (BGMToggleMenuItem_Tag);

    if (toggle->getSelectedIndex () == 0)
    {
        menuItem_playMusic (pSender);
    }
    else
    {
        menuItem_stopMusic (pSender);
    }
}

void
MenuLayer::menuItem_playMusic(CCObject *pSender)
{
    CC_UNUSED_PARAM (pSender);

    SimpleAudioEngine *engine = SimpleAudioEngine::sharedEngine();

    if (! engine->isBackgroundMusicPlaying ())
    {
        engine->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("Sounds/background.mp3"), 
									true);
    }
}

void
MenuLayer::menuItem_stopMusic(CCObject *pSender)
{
    CC_UNUSED_PARAM (pSender);

    SimpleAudioEngine *engine = SimpleAudioEngine::sharedEngine();
    
    if (engine->isBackgroundMusicPlaying ())
    {
        engine->stopBackgroundMusic ();
    }
}

void
MenuLayer::onEnter ()
{
    CCLayer::onEnter ();
    CCMenu *secondaryMenu = (CCMenu *) getChildByTag (SecondaryMenu_Tag);
    CCMenuItemToggle *toggle = (CCMenuItemToggle *) secondaryMenu->getChildByTag (BGMToggleMenuItem_Tag);

    if (toggle->getSelectedIndex () == 0)
    {
        menuItem_playMusic (this);
    }
}

void
MenuLayer::onExit ()
{
    CCLayer::onExit ();
}
