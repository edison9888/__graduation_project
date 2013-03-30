//
//  MCFlag.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-10.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCFlag.h"
#include "MCFlagManager.h"

bool
MCFlag::init(const mc_object_id_t &anObjectId, JsonBox::Object &aRoot)
{
    JsonBox::Value v;
    CCString *ccstring;
    
    setID(anObjectId);
    
    /* name String */
    ccstring = CCString::create(aRoot["name"].getString());
    setName(ccstring);
    ccstring->retain();
    /* state Integer */
    setState(aRoot["state"].getInt());
    /* description String */
    ccstring = CCString::create(aRoot["description"].getString());
    setName(ccstring);
    ccstring->retain();
    
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
    MCFlagManager::sharedFlagManager()->updateFlag(this);
}