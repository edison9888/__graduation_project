#ifndef TOUCH_H
#define TOUCH_H

#include <cocos2d.h>

USING_NS_CC;

class Touch : public CCLayer
{
public:
    Touch() {}

    virtual bool init();

    static CCScene *scene()
    {
        CCScene *scene = new CCScene;

        scene->addChild (Touch::create ());

        return scene;
    }

    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

    void onEnter ();
    void onExit ();

    CREATE_FUNC(Touch);
};

#endif // TOUCH_H
