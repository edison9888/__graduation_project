#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <cocos2d.h>
USING_NS_CC;

class MenuLayer : public CCLayer
{
public:
    ~MenuLayer();

    virtual bool init();

    static CCScene* scene();

    //menu item action
    void menuItem_playCallback(CCObject* pSender);
    void menuItem_loadCallback(CCObject* pSender);
    void menuItem_helpCallback(CCObject* pSender);
    void menuItem_quitCallback(CCObject* pSender);

    void menuItem_exitYesCallback(CCObject* pSender);
    void menuItem_exitNoCallback(CCObject* pSender);
    void exitDialog_close(CCObject* pSender);

    void menuItem_toggleMusic(CCObject *pSender);
    void menuItem_playMusic(CCObject *pSender);
    void menuItem_stopMusic(CCObject *pSender);

    void onEnter ();
    void onExit ();

    void menuItem_testCallback(CCObject *pSender);

    CREATE_FUNC(MenuLayer);

private:
    CCMenu *menu;
};

#endif // MENUSCENE_H
