#ifndef CCSPRITECOLOR_H
#define CCSPRITECOLOR_H

#include <cocos2d.h>
USING_NS_CC;

class CCSpriteColor : public CCSprite
{
public:
    virtual bool initWithSize(const CCSize& size);
    virtual bool initWithColor(const ccColor4B& color, const CCSize& size);

    static CCSpriteColor *spriteWithSize(const CCSize& size);
    static CCSpriteColor *spriteWithColor(const ccColor4B& color, const CCSize& size);
};

#endif // CCSPRITECOLOR_H
