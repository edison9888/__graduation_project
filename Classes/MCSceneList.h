//
//  MCSceneList.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSceneList__
#define __Military_Confrontation__MCSceneList__

#include "MCScenePackage.h"

class MCSceneList : public CCObject {
private:
    MCSceneList();
    
public:
    ~MCSceneList();
    
    static MCSceneList *sharedSceneList();
    
    void loadSceneListFile(const char *aFilepath);
    
    MCScenePackage *packageWithNamed(const char *aName);
    MCScenePackage *packageAtIndex(mc_index_t anIndex);
    
private:
    CCArray *scenes_;
};

#endif /* defined(__Military_Confrontation__MCSceneList__) */
