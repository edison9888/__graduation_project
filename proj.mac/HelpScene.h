#ifndef HELPSCENE_H
#define HELPSCENE_H

#include <cocos2d.h>

USING_NS_CC;

class Help : public CCLayer
{
public:
    virtual bool init();

    static CCScene* scene();

    virtual void menu_backCallback(CCObject *pSender);

    void onEnter ();
    void onExit ();

    CREATE_FUNC(Help);
};

#endif // HELPSCENE_H
