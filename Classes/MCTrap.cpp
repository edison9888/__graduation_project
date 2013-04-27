//
//  MCTrap.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTrap.h"

const char *kMCUnknownTrap = "未知陷阱";
const char *kMCFireballTrap = "火球陷阱";
const char *kMCCurseTrap = "诅咒陷阱";
const char *kMCParalysisTrap = "麻痹陷阱";
const char *kMCFogTrap = "迷雾陷阱";
const char *kMCFlashTrap = "闪光陷阱";

const char *
MCTrapGetNameWithTrapType(MCTrapType aTrapType)
{
    switch (aTrapType) {
        case MCFireballTrap:
            return kMCFireballTrap;
        case MCCurseTrap:
            return kMCCurseTrap;
        case MCParalysisTrap:
            return kMCParalysisTrap;
        case MCFogTrap:
            return kMCFogTrap;
        case MCFlashTrap:
            return kMCFlashTrap;
            
        default:
            return kMCUnknownTrap;
    }
}

bool
MCTrap::init(mc_object_id_t anObjectId)
{
    if (MCEffectiveItem::init()) {
        setID(anObjectId);
        return true;
    }
    
    return false;
}

MCTrap *
MCTrap::create(mc_object_id_t anObjectId)
{
    MCTrap *trap = new MCTrap;
    
    if (trap && trap->init(anObjectId)) {
        trap->autorelease();
    } else {
        CC_SAFE_DELETE(trap);
        trap = NULL;
    }
    
    return trap;
}

CCObject *
MCTrap::copy()
{
    MCTrap *trap = new MCTrap;
    
    trap->setID(id_);
    trap->setTag(tag_);
    trap->setName(name_);
    trap->setDescription(description_);
    trap->setItemType(itemType_);
#warning effect_
//    trap->effect_ = effect_;
    trap->trapType_ = trapType_;
    
    return trap;
}
