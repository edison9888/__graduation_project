//
//  MCMercenary.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMercenary__
#define __Military_Confrontation__MCMercenary__

#include "MCNPC.h"

/* NPC */
class MCMercenary : public MCNPC {
public:
    ~MCMercenary();
    
    bool init();
    
    static MCMercenary *create(mc_object_id_t anObjectId);
    
    CCObject *copy();
};

#endif /* defined(__Military_Confrontation__MCMercenary__) */
