#include "AppDelegate.h"
#include "AppMacros.h"

#include "AppDelegate.h"

//#include "SplashScene.h"
//#include "MenuScene.h"

//#include "Touch.h"
//#include "CCDebug.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);

	CCSize frameSize = pEGLView->getFrameSize();

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

#ifdef CC_TARGET_OS_MAC
    CCFileUtils::sharedFileUtils()->setResourceDirectory(macResource.directory);
    pDirector->setContentScaleFactor(MIN(macResource.size.height/designResolutionSize.height, macResource.size.width/designResolutionSize.width));
#else
    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{ 
		CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    { 
        CCFileUtils::sharedFileUtils()->setResourceDirectory(mediumResource.directory);
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    { 
		CCFileUtils::sharedFileUtils()->setResourceDirectory(smallResource.directory);
        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
#endif
	
    // turn on display FPS
//    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    
    // run
//    pDirector->pushScene (MenuLayer::scene());
//    pDirector->runWithScene(Splash::scene());
    
    //debug
    // create a scene. it's an autorelease object
//    pDirector->runWithScene (MenuLayer::scene ());

    CCSize winSize = CCDirector::sharedDirector ()->getWinSize ();
	CCScene *scene = new CCScene;
	CCLayerColor *layer = CCLayerColor::create(ccc4(255, 0, 255, 255));
	CCLabelTTF *label = CCLabelTTF::create("hello world", "Arial", 36);
	label->setColor(ccc3(64, 128, 240));
	label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	layer->addChild(label);
	scene->addChild(layer);
	pDirector->runWithScene (scene);

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
