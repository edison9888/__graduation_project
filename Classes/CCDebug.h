#ifndef CCDEBUG_H
#define CCDEBUG_H

#include <cocos2d.h>
USING_NS_CC;

#include <stdarg.h>

#include <cstring>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <android/log.h>

#define mark()  __android_log_print(ANDROID_LOG_DEBUG, "CCDebug::mark", "file: %s, line: %d\n", __FILE__, __LINE__)
#define Log_d(s)  __android_log_print(ANDROID_LOG_DEBUG, "CCDebug::Log_d", "%s\n", (s))

#else

#define mark() printf ("file: %s, line: %d\n", __FILE__, __LINE__)

#endif  // CC_PLATFORM_ANDROID

#define qlog(s)             \
    do {                    \
    __qlog_buf.reset();     \
    __qlog_buf.append((s)); \
    __qlog_buf.log();       \
    } while (0)

const unsigned int CCBuffer_MaxSize = 4096;

class CCBuffer
{
public:
    CCBuffer()
    {
        buf = new char[CCBuffer_MaxSize];
        pointer = buf;
    }

    virtual ~CCBuffer()
    {
        delete buf;
    }

    static CCBuffer *create()
    {
        CCBuffer *buf = new CCBuffer;
        return buf;
    }

    void reset()
    {
        pointer = buf;
    }

    void seek(unsigned int offset)
    {
        pointer += offset;
    }

    void append(const char *fmt, ...)
    {
        va_list ap;
        va_start (ap, fmt);
        pointer += vsprintf (pointer, fmt, ap);
        va_end (ap);
    }

    void append(const CCPoint &point)
    {
        pointer += sprintf (pointer, "<CCPoint (%.0f, %0.f)>", point.x, point.y);
    }

    void append(const CCSize &size)
    {
        pointer += sprintf (pointer, "<CCSize (%.0f, %0.f)>", size.width, size.height);
    }

    void append(const CCRect &rect)
    {
        pointer += sprintf (pointer, "<CCRect x=%.0f y=%0.f w=%.0f h=%.0f>",
                            rect.origin.x, rect.origin.y,
                            rect.size.width, rect.size.height);
    }

    const char *buffer()
    {
        return buf;
    }

    const unsigned int size()
    {
        return strlen (buf);
    }

    void log()
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print (ANDROID_LOG_DEBUG, "CCBuffer", "%s", buf);
#else
        printf ("CCBuffer::%s\n", buf);
#endif  // CC_PLATFORM_ANDROID
    }

private:
    char *buf;
    char *pointer;
};

static CCBuffer __qlog_buf;

#endif // CCDEBUG_H
