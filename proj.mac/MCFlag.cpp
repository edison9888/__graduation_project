//
//  MCFlag.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-10.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCFlag.h"

bool
MCFlag::init(const mc_object_id_t &anObjectId, JsonBox::Object &aRoot)
{
    JsonBox::Value v;
    
    setID(anObjectId);
    v = aRoot["name"];
    if (v.isString()) {
        setName(CCString::create(v.getString()));
    }
    v = aRoot["description"];
    if (v.isString()) {
        setDescription(CCString::create(v.getString()));
    }
    return true;
}

MCFlag *
MCFlag::create(const mc_object_id_t &anObjectId, JsonBox::Object &aRoot)
{
    MCFlag *flag = new MCFlag;
    
    if (flag && flag->init(anObjectId, aRoot)) {
        flag->autorelease();
    } else {
        CC_SAFE_DELETE(flag);
        flag = NULL;
    }
    
    return flag;
}

MCFlagState
MCFlag::getState()
{
    return state_;
}

void
MCFlag::setState(MCFlagState aFlagState)
{
    state_ = aFlagState;
#warning todo: 更新标志管理器里的信息
}