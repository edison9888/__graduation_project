#include "Touch.h"

#include "CCDebug.h"

bool
Touch::init()
{
    if (CCLayer::init ())
    {
        this->setTouchEnabled (true);

        return true;
    }

    return false;
}

bool
Touch::ccTouchBegan(CCTouch *touch, CCEvent *event)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Log_d("touch began\n");
#endif
    printf("touch began\n");

    return true;
}

void
Touch::ccTouchMoved(CCTouch *touch, CCEvent *event)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Log_d("touch moved\n");
#endif
    printf("touch moved\n");
}

void
Touch::ccTouchEnded(CCTouch *touch, CCEvent *event)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Log_d("touch ended\n");
#endif
    printf("touch ended\n");
}

void
Touch::ccTouchCancelled (CCTouch *pTouch, CCEvent *pEvent)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Log_d("touch cancelled\n");
#endif
    printf("touch cancelled\n");
}

void
Touch::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Log_d("touches began\t");
    char buf[512];
    sprintf (buf, "tc: %d\n", pTouches->count ());
    Log_d (buf);
#endif
    printf("touches began\n");
}

void
Touch::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Log_d("touches moved\t");
    char buf[512];
    sprintf (buf, "tc: %d\n", pTouches->count ());
    Log_d (buf);
#endif
    printf("touches moved\n");
}

void
Touch::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Log_d("touches ended\t");
    char buf[512];
    sprintf (buf, "tc: %d\n", pTouches->count ());
    Log_d (buf);
#endif
    printf("touches ended\n");
}

void
Touch::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Log_d("touches cancelled\t");
    char buf[512];
    sprintf (buf, "tc: %d\n", pTouches->count ());
    Log_d (buf);
#endif
    printf("touches cancelled\n");
}


void
Touch::onEnter ()
{
    CCLayer::onEnter();
//    CCDirector::sharedDirector ()->getTouchDispatcher ()->addTargetedDelegate(this, 0, true);
}

void
Touch::onExit ()
{
//    CCDirector::sharedDirector ()->getTouchDispatcher ()->removeDelegate(this);
    CCLayer::onExit();
}
