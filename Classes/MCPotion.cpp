//
//  MCPotion.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCPotion.h"

bool
MCPotion::init(mc_object_id_t anObjectId)
{
    if (MCEffectiveItem::init()) {
        setID(anObjectId);
        return true;
    }
    
    return false;
}

MCPotion *
MCPotion::create(mc_object_id_t anObjectId)
{
    MCPotion *potion = new MCPotion;
    
    if (potion && potion->init(anObjectId)) {
        potion->autorelease();
    } else {
        CC_SAFE_DELETE(potion);
        potion = NULL;
    }
    
    return potion;
}

CCObject *
MCPotion::copy()
{
    MCPotion *potion = new MCPotion;
    
    potion->setID(id_);
    potion->setTag(tag_);
    potion->setName(name_);
    potion->setDescription(description_);
    potion->setItemType(itemType_);
    
#warning effect_
//    potion->effect_ = effect_;
    
    return potion;
}
