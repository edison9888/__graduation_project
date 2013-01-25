#include "AppMacros.h"

#include "AppDelegate.h"

#include "MCTestScene.h"
#include "MCSplashScene.h"
#include "MCMainMenuScene.h"

#include "MCSceneList.h"
#include "MCTestControllerScene.h"

USING_NS_CC;

class TL: public CCLayer {
public:
    bool init()
    {
        setTouchEnabled(true);;
        CCSprite *s=CCSprite::create("tank.png");
        addChild(s);
        s->setPosition(ccp(10,10));
        
        return true;
    }
    CREATE_FUNC(TL);
    static CCScene *scene()
    {
        CCScene *scene = CCScene::create();
        scene->addChild(TL::create());
        return scene;
    }
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
    {
//        stopAllActions();
        runAction(CCMoveTo::create(4, ((CCTouch *)pTouches->anyObject())->getLocationInView()));
    }
};

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	CCSize frameSize = pEGLView->getFrameSize();

    pDirector->setOpenGLView(pEGLView);
    
    CCLog("%s(%d): %.0f %.0f", __FILE__, __LINE__, frameSize.width, frameSize.height);

    // Set the design resolution
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    pEGLView->setDesignResolutionSize(PCResource.size.width, PCResource.size.height, kResolutionExactFit);
    CCFileUtils::sharedFileUtils()->setResourceDirectory(PCResource.directory);
    pDirector->setContentScaleFactor(MIN(32 * 25 / PCResource.size.width, 32 * 15 / PCResource.size.height));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height == smallResource.size.height)
	{
        pEGLView->setDesignResolutionSize(smallResource.size.width, smallResource.size.height, kResolutionNoBorder);
		CCFileUtils::sharedFileUtils()->setResourceDirectory(smallResource.directory);
	}
        // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height == mediumResource.size.height)
    {
        pEGLView->setDesignResolutionSize(mediumResource.size.width, mediumResource.size.height, kResolutionNoBorder);
        CCFileUtils::sharedFileUtils()->setResourceDirectory(mediumResource.directory);
    }
        // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        pEGLView->setDesignResolutionSize(largeResource.size.width, largeResource.size.height, kResolutionNoBorder);
		CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
    }
    pDirector->setContentScaleFactor(MIN(32 * 25 / frameSize.width, 32 * 15 / frameSize.height));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int i = 0;
    int size = sizeof(resources);
    int found = 0;
    for (; i < size; ++i) {
        if ((frameSize.width == resources[i].size.width && frameSize.height == resources[i].size.height)
            || (frameSize.width == resources[i].size.height && frameSize.height == resources[i].size.width)) {
            CCFileUtils::sharedFileUtils()->setResourceDirectory(resources[i].directory);
            pEGLView->setDesignResolutionSize(resources[i].size.width, resources[i].size.height, kResolutionNoBorder);
            found = 1;
        }
    }
    if (found == 0) {
		CCFileUtils::sharedFileUtils()->setResourceDirectory(resources[0].directory);
        pEGLView->setDesignResolutionSize(resources[0].size.width, resources[0].size.height, kResolutionNoBorder);
    }
#else
    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
		CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
        pDirector->setContentScaleFactor(MIN(largeResource.size.height/smallResource.size.height, largeResource.size.width/smallResource.size.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(mediumResource.directory);
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/smallResource.size.height, mediumResource.size.width/smallResource.size.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
		CCFileUtils::sharedFileUtils()->setResourceDirectory(smallResource.directory);
        pDirector->setContentScaleFactor(MIN(smallResource.size.height, smallResource.size.width));
    }
#endif
	
    // turn on display FPS
//    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
//    MCSceneList *slist = MCSceneList::sharedSceneList();
//    slist->loadSceneListFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("test.slist"));
    
    // run
//    pDirector->pushScene(MCMainMenuLayer::scene());
//    pDirector->runWithScene(MCSplashLayer::scene());
    pDirector->runWithScene(MCTestControllerLayer::scene());
//    pDirector->runWithScene(TL::scene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
