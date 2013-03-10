//
//  MCTestLua.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTestLua.h"
#include "MCScript.h"

const int LINE_SPACE = 40;
const CCPoint curPos = ccp(0, 0);

void base_test(CCObject *anObject)
{
    MCScriptRun("test-lua.lua");
}

void test2(CCObject *anObject)
{
    CCLog("test 2");
}

typedef void (*testFunc)(CCObject *);

struct __mc_menu_item {
    const char *label;
    testFunc func;
};

struct __mc_menu_item __menu_items[] = {
    {"base test", base_test},
    {"test2", test2}
};

bool
MCTestLua::init()
{
    if (MCTestLayer::initWithColor(ccc4(64, 64, 64, 255)))
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCMenu *menu = CCMenu::create();
        size_t count = sizeof(__menu_items) / sizeof(struct __mc_menu_item);
        CCMenuItem *item;
        CCLabelTTF *label;
        
        for (int i = 0; i < count; ++i) {
            struct __mc_menu_item __menu_item = __menu_items[i];
            label = CCLabelTTF::create(__menu_item.label, "Arial", 24);
            item = CCMenuItemLabel::create(label, this, menu_selector(MCTestLua::runScript));
            item->setUserData((void *) __menu_item.func);
            item->setPosition(winSize.width / 2, (winSize.height - (i + 1) * LINE_SPACE));
            menu->addChild(item);
        }
        
        menu->setContentSize(CCSizeMake(winSize.width, (count + 1) * LINE_SPACE));
        menu->setPosition(curPos);
        addChild(menu);
        menu_ = menu;
        
        return true;
    }
    
    return false;
}

void
MCTestLua::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    CCPoint delta = touch->getDelta();
    
    moveMenu(delta);
}

void
MCTestLua::moveMenu(const CCPoint &delta) {
    CCPoint current = menu_->getPosition();
    float newY = current.y + delta.y;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    size_t count = sizeof(__menu_items) / sizeof(struct __mc_menu_item);
    
    if (newY < 0)
        newY = 0;
    
    if( newY > ((count + 1) * LINE_SPACE - winSize.height))
        newY = ((count + 1) * LINE_SPACE - winSize.height);
    
    menu_->setPosition(current.x, newY);
}


void
MCTestLua::onEnter()
{
    CCLayerColor::onEnter();
}

void
MCTestLua::runScript(CCObject *anObject)
{
    CCMenuItem *item = (CCMenuItem *) anObject;
    testFunc func = (testFunc) item->getUserData();
    func(anObject);
}
