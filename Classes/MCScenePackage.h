//
//  MCScenePackage.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCScenePackage__
#define __Military_Confrontation__MCScenePackage__

#include "MCObject.h"

class MCScenePackage : public MCObject {
public:
    CC_SYNTHESIZE(CCString *, backgroundMusicFilename_, BackgroundMusicFilename); /* 背景音乐文件名，NULL为无音乐 */
    
};

#endif /* defined(__Military_Confrontation__MCScenePackage__) */
