//
//  MCTestbed.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-28.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTestbed__
#define __Military_Confrontation__MCTestbed__

#include <cocos2d.h>
USING_NS_CC;

class MCTestbed : public CCLayer {
public:
    bool init();
    
    CREATE_FUNC(MCTestbed);
    
    static CCScene *scene()
    {
        CCScene *scene = CCScene::create();
        
        MCTestbed * l1 = new MCTestbed;
        
        l1->init();
        scene->addChild(l1);
        
        return scene;
    }
    
    void testbedDidSelectScene(CCObject *pSender);
    
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    
    void moveMenu(const CCPoint &delta);
    
protected:
    CCMenu *menu_;
};

#endif /* defined(__Military_Confrontation__MCTestbed__) */
