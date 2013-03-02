//
//  MCTestLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-28.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef Military_Confrontation_MCTestLayer_h
#define Military_Confrontation_MCTestLayer_h

#include <cocos2d.h>
USING_NS_CC;

class MCTestLayer : public CCLayerColor {
public:
    bool init() {
        if (CCLayerColor::init()) {
            
            return _init();
        }
        
        return false;
    }
    
    bool initWithColor(const ccColor4B& color) {
        if (CCLayerColor::initWithColor(color)) {
            return _init();
        }
        
        return false;
    }
    
    bool _init() {
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCMenu *menu = CCMenu::create();
        
        CCLabelTTF *label = CCLabelTTF::create("返回", "arial", 24);
        label->setColor(ccc3(0, 0, 0));
        CCMenuItemLabel *item = CCMenuItemLabel::create(label, this, menu_selector(MCTestLayer::backward));
        menu->addChild(item);
        menu->setPosition(ccp(winSize.width - item->getContentSize().width, item->getContentSize().height));
        
        menu->alignItemsHorizontally();
        
        addChild(menu, 0x7FFFFFFF);
        
        return true;
    }
    
    virtual void backward(CCObject *pSender) {
        CCDirector::sharedDirector()->popScene();
    }
};

#endif
