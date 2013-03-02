//
//  MCNPC.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCNPC__
#define __Military_Confrontation__MCNPC__

#include "MCRole.h"

/* NPC */
class MCNPC : public MCRole {
public:
    bool init();
    
    static MCNPC *create(mc_object_id_t anObjectId);
};

#endif /* defined(__Military_Confrontation__MCNPC__) */
