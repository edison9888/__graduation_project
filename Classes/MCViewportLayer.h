//
//  MCViewportLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#if MC_DEBUG_NON_VISUAL_OBJECTS == 1
#ifndef __Military_Confrontation__MCViewportLayer__
#define __Military_Confrontation__MCViewportLayer__

#include <cocos2d.h>
USING_NS_CC;

class MCMezzanine;

class MCViewportLayer : public CCLayer {
    
public:
    bool init();
    
    CREATE_FUNC(MCViewportLayer);
    
    void draw(void);
    
    void loadObjects(CCArray *objects);
    void loadBarriers(CCArray *barriers);
    void loadSemis(CCArray *semis);
    void loadEntrances(CCArray *entrances);
    void loadMezzanine(MCMezzanine *mezzanine);
    
public:
    CCArray *objects;
    CCArray *barriers;
    CCArray *semis;
    CCArray *entrances;
    MCMezzanine *mezzanine;
    CCTMXTiledMap *map;
};

#endif /* defined(__Military_Confrontation__MCViewportLayer__) */
#endif /* MC_DEBUG_NON_VISUAL_OBJECTS */
