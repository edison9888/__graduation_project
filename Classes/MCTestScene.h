//
//  TestScene.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__TestScene__
#define __Military_Confrontation__TestScene__

#include <cocos2d.h>

USING_NS_CC;

class MCTestLayer : public CCLayerColor {
    
public:
    bool init();
    
    CREATE_FUNC(MCTestLayer);
    
protected:
	void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
private:
    CCSprite *_crow;
    CCMoveTo *_moveAction;
    CCPoint _prevousLocation;
};

#endif /* defined(__Military_Confrontation__TestScene__) */
