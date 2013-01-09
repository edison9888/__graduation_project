#include "SplashScene.h"

USING_NS_CC;

const int BackgroundLayer_Tag = 0x24;

class BackgroundLayer : public CCLayer
{
public:
    BackgroundLayer() { }

    virtual bool initWithColor (const ccColor4B &color)
    {
        CCLabelTTF *title;
        CCSize winSize;
        CCSize titleSize;

        if (CCLayer::init())
        {
            winSize = CCDirector::sharedDirector ()->getWinSize ();

            title = CCLabelTTF::create ("Adventure", "Arial", 96);
            titleSize = title->getContentSize();
            title->setPosition (ccp (winSize.width / 2,
                                     winSize.height / 2 + 32));
            this->addChild (title);

            title = CCLabelTTF::create ("Created with Cocos2d-X", "Arial", 32);
            titleSize = title->getContentSize();
            title->setPosition (ccp (winSize.width / 2,
                                     winSize.height / 2 - titleSize.height));
            this->addChild (title);

            return true;
        }

        return false;
    }
};

CCScene*
Splash::scene()
{
    CCScene *scene = CCScene::create ();

    Splash *layer = Splash::create ();

    scene->addChild(layer);

    return scene;
}

bool
Splash::init()
{
    BackgroundLayer *backgroundLayer;

    if (CCLayer::init())
    {
        //add background
        backgroundLayer = new BackgroundLayer;
        backgroundLayer->initWithColor (ccc4 (64, 128, 240, 255));
        backgroundLayer->autorelease ();

        this->addChild (backgroundLayer, 0, BackgroundLayer_Tag);

        return true;
    }

    return false;
}

void Splash::onEnter ()
{
    CCLayer::onEnter ();
    CCNode *backgroundLayer;

    backgroundLayer = getChildByTag (BackgroundLayer_Tag);
    backgroundLayer->runAction (CCSequence::create (CCDelayTime::create (1),
                                                    CCCallFuncN::create (CCDirector::sharedDirector (),
                                                                         callfuncN_selector(CCDirector::popScene)),
                                                    NULL));
}
