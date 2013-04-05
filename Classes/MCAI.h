//
//  MCAI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCAI__
#define __Military_Confrontation__MCAI__

#include "MCObject.h"
#include "MCVision.h"

class MCAI : public MCObject {
public:
    MCAI();
    ~MCAI();
    
    bool init();
    
    CREATE_FUNC(MCAI);
    
    /* 绑定到人物 */
    void bind(MCRole *aRole);
    MCRole *unbind(MCRole *aRole);
    
    void update(float dt); /* 大脑在转动 */
    
protected:
    MCRole *role_;
    
    CC_SYNTHESIZE_READONLY(MCVision *, vision_, Vision); /* 视野 */
};

#endif /* defined(__Military_Confrontation__MCAI__) */
