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

typedef mc_ushort_t mc_cose_t;

class MCMercenaryManager;

/* 佣兵 */
class MCMercenary : public MCNPC {
    friend class MCMercenaryManager;
public:
    ~MCMercenary();
    
    bool init();
    
    static MCMercenary *create(mc_object_id_t anObjectId);
    
    CCObject *copy();
    
    CC_SYNTHESIZE_READONLY(mc_cose_t, cost_, Cost); /* 雇佣所需费用 */
};

#endif /* defined(__Military_Confrontation__MCMercenary__) */
