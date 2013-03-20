//
//  MCTaskTarget.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskTarget__
#define __Military_Confrontation__MCTaskTarget__

#include "MCObject.h"

class MCTaskTarget : public CCObject {
public:
    mc_object_id_t objectID;
    mc_size_t      count;
};

#endif /* defined(__Military_Confrontation__MCTaskTarget__) */
