//
//  MCEnemyAI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-16.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEnemyAI__
#define __Military_Confrontation__MCEnemyAI__

#include "MCAI.h"

class MCEnemyAI : public MCAI {
public:
    void changingState();
    
    CREATE_FUNC(MCEnemyAI);
};

#endif /* defined(__Military_Confrontation__MCEnemyAI__) */
