//
//  MCJSONModifier.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCJSONModifier__
#define __Military_Confrontation__MCJSONModifier__

#include "MCType.h"

enum {
    MCJSONVisible,
    MCJSONInvisible
};
typedef mc_enum_t MCJSONVisibleState;

class MCJSONModifier {
public:
    static void setJSONFileVisible(const char         *aFilepath,
                                   MCJSONVisibleState aState);
};

#endif /* defined(__Military_Confrontation__MCJSONModifier__) */
