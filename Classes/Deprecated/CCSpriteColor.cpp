#include <cstdlib>

#include "CCSpriteColor.h"

static void *texture2d_createData(size_t dataLength)
{
    return calloc (sizeof(unsigned char), dataLength);
}

const size_t bitsPerComponent = 8;

bool
CCSpriteColor::initWithSize(const CCSize& size)
{
    CCTexture2D *texture;
    size_t bytesPerPixel = (bitsPerComponent * 3) / 8;
    size_t bytesPerRow = bytesPerPixel * size.width;
    size_t dataLength = bytesPerRow * size.height;
    void *data = texture2d_createData (dataLength);

    texture = new CCTexture2D;
    texture->initWithData (data,
                           kCCTexture2DPixelFormat_RGB888,
                           size.width, size.height, size);

    if (CCSprite::initWithTexture (texture))
    {
        free(data);

        return true;
    }

    return false;
}

bool
CCSpriteColor::initWithColor(const ccColor4B& color, const CCSize& size)
{
    CCTexture2D *texture;
    size_t bytesPerPixel = (bitsPerComponent * 4) / 8;
    size_t bytesPerRow = bytesPerPixel * size.width;
    size_t dataLength = bytesPerRow * size.height;
    void *data = texture2d_createData (dataLength);
    unsigned char *bytesData = (unsigned char *) data;
    size_t i;

    if (! bytesData)
    {
        return false;
    }

    for (i = 0; i < dataLength;)
    {
        bytesData[i++] = color.r;
        bytesData[i++] = color.g;
        bytesData[i++] = color.b;
        bytesData[i++] = color.a;
    }

    texture = new CCTexture2D;
    texture->initWithData (data,
                           kCCTexture2DPixelFormat_RGBA8888,
                           size.width, size.height, size);

    if (CCSprite::initWithTexture (texture))
    {
        free(data);

        return true;
    }

    return false;
}

CCSpriteColor *
CCSpriteColor::spriteWithSize(const CCSize& size)
{
    CCSpriteColor *sprite = new CCSpriteColor;

    if (sprite && sprite->initWithSize (size))
    {
        sprite->autorelease ();
    }

    return sprite;
}

CCSpriteColor *
CCSpriteColor::spriteWithColor(const ccColor4B& color, const CCSize& size)
{
    CCSpriteColor *sprite = new CCSpriteColor;

    if (sprite && sprite->initWithColor (color, size))
    {
        sprite->autorelease ();
    }

    return sprite;
}
