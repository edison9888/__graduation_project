#include "AppMacros.h"

#include "AppDelegate.h"

#include "MCTestScene.h"

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
	CCSize frameSize = pEGLView->getFrameSize();

    pDirector->setOpenGLView(pEGLView);
    
    CCLog("%.0f %.0f", frameSize.width, frameSize.height);

    // Set the design resolution
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    pEGLView->setDesignResolutionSize(PCResource.size.width, PCResource.size.height, kResolutionNoBorder);
    CCFileUtils::sharedFileUtils()->setResourceDirectory(PCResource.directory);
    pDirector->setContentScaleFactor(1.0f);
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
    pDirector->setContentScaleFactor(1.0f);
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

    CCString *str = CCString::createWithFormat("getContentScaleFactor: %f", pDirector->getContentScaleFactor());
    CCScene *s = MCTestLayer::scene ();
    
    CCLabelTTF *l = CCLabelTTF::create();
    l->setString(str->getCString());
    l->setColor(ccc3(64, 128, 240));
    l->setFontSize(64);
    s->addChild(l);
    l->setPosition(ccp(200, 200));
    
    // run
    pDirector->runWithScene (s);

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
