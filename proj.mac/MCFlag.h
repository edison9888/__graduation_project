//
//  MCFlag.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-10.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCFlag__
#define __Military_Confrontation__MCFlag__

#include "MCObject.h"
#include "JsonBox.h"

enum {
    MCInvalidState = -1,    /* 无效标志，不存在此标志 */
    MCOnState = 0,          /* 标志打开状态 */
    MCOffState = 1          /* 标志关闭状态 */
};
typedef mc_enum_t MCFlagState;

class MCFlagManager;

class MCFlag : public MCObject {
    friend class MCFlagManager;
private:
    bool init(const mc_object_id_t &anObjectId, JsonBox::Object &aRoot);
    
    static MCFlag *create(const mc_object_id_t &anObjectId, JsonBox::Object &aRoot);

    CC_PROPERTY(MCFlagState, state_, State);
};

#endif /* defined(__Military_Confrontation__MCFlag__) */
