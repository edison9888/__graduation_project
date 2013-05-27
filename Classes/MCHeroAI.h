//
//  MCHeroAI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-5-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCHeroAI__
#define __Military_Confrontation__MCHeroAI__

#include "MCAI.h"

class MCHeroAI : public MCAI {
public:
    void changingState(); /* 状态切换 */
    
    CREATE_FUNC(MCHeroAI);
};

#endif /* defined(__Military_Confrontation__MCHeroAI__) */
