#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

/* For demonstrating using one design resolution to match different resources,
   or one resource to match different design resolutions.

   [Situation 1] Using one design resolution to match different resources.
     Please look into Appdelegate::applicationDidFinishLaunching.
     We check current device frame size to decide which resource need to be selected.
     So if you want to test this situation which said in title '[Situation 1]',
     you should change ios simulator to different device(e.g. iphone, iphone-retina3.5, iphone-retina4.0, ipad, ipad-retina),
     or change the window size in "proj.XXX/main.cpp" by "CCEGLView::setFrameSize" if you are using win32 or linux plaform
     and modify "proj.mac/AppController.mm" by changing the window rectangle.

   [Situation 2] Using one resource to match different design resolutions.
     The coordinates in your codes is based on your current design resolution rather than resource size.
     Therefore, your design resolution could be very large and your resource size could be small.
     To test this, just define the marco 'TARGET_DESIGN_RESOLUTION_SIZE' to 'DESIGN_RESOLUTION_2048X1536'
     and open iphone simulator or create a window of 480x320 size.

   [Note] Normally, developer just need to define one design resolution(e.g. 960x640) with one or more resources.
 */

#define CCDirectorGetWindowsSize() CCDirector::sharedDirector()->getWinSize()
#define CCDirectorGetContentScaleFactor() CCDirector::sharedDirector()->getContentScaleFactor()

#define DESIGN_RESOLUTION_480X320    0
#define DESIGN_RESOLUTION_800X480    1
#define DESIGN_RESOLUTION_960X640    2
#define DESIGN_RESOLUTION_1024X768   3
#define DESIGN_RESOLUTION_2048X1536  4

/* If you want to switch design resolution, change next line */

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
static Resource smallResource  =  { cocos2d::CCSizeMake(960, 640),   "iphone" };
//static Resource mediumResource =  { cocos2d::CCSizeMake(1024, 768),  "ipad"   };
//static Resource largeResource  =  { cocos2d::CCSizeMake(2048, 1536), "ipadhd" };
static Resource mediumResource =  { cocos2d::CCSizeMake(1024, 768),  "iphone"   };
static Resource largeResource  =  { cocos2d::CCSizeMake(2048, 1536), "iphone" };
#endif // IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static Resource resources[] = {
    { cocos2d::CCSizeMake(480, 320),   "iphone" },
    { cocos2d::CCSizeMake(800, 480),   "iphone" },
    { cocos2d::CCSizeMake(854, 320),   "iphone" },
    { cocos2d::CCSizeMake(960, 580),   "iphone" },
    { cocos2d::CCSizeMake(960, 640),   "iphone" }
};
#endif // ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//static Resource PCResource  =  { cocos2d::CCSizeMake(800, 480),   "iphone" };
static Resource PCResource  =  { cocos2d::CCSizeMake(960, 640),   "iphone" };
#endif // WIN32、MAC、LINUX

// The font size 24 is designed for small resolution, so we should change it to fit for current design resolution
#define TITLE_FONT_SIZE  (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / smallResource.size.width * 24)

#endif /* __APPMACROS_H__ */
