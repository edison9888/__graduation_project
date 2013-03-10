#include <windows.h>
#include <tchar.h>

// C RunTime Header Files
#include <CCStdC.h>

#include "../Classes/AppDelegate.h"
#include <CCEGLView.h>

USING_NS_CC;

#undef main
int main(int argc, char *argv[])
{

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setFrameSize(800, 480);
    // The resolution of ipad3 is very large. In general, PC's resolution is smaller than it.
    // So we need to invoke 'setFrameZoomFactor'(only valid on desktop(win32, mac, linux)) to make the window smaller.
    eglView->setFrameZoomFactor(1.0f);
    return CCApplication::sharedApplication()->run();
}
