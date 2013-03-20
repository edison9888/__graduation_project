//
//  MCEffectiveItem.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEffectiveItem.h"

bool
MCEffectiveItem::init()
{
    if (MCItem::init()) {
        effect_.hp = 0;
        effect_.pp = 0;
        effect_.positive_state = 0;
        effect_.negative_state = 0;
        
        return true;
    }
    
    return false;
}

CCObject *
MCEffectiveItem::copy()
{
    MCEffectiveItem *effectiveItem = new MCEffectiveItem;
    
    effectiveItem->setID(id_);
    effectiveItem->setTag(tag_);
    effectiveItem->setName(name_);
    effectiveItem->setDescription(description_);
    effectiveItem->setItemType(itemType_);
    effectiveItem->setIcon(icon_);
    effectiveItem->effect_ = effect_;
    effectiveItem->setArea(area_);
    effectiveItem->setPath(path_);
    
    return effectiveItem;
}
