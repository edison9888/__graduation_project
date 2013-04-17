//
//  MCNPCAI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-16.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCNPCAI__
#define __Military_Confrontation__MCNPCAI__

#include "MCAI.h"

class MCNPCAI : public MCAI {
public:
    void update(float dt);
    
    CREATE_FUNC(MCNPCAI);
};

#endif /* defined(__Military_Confrontation__MCNPCAI__) */
