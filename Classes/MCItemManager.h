//
//  MCItemManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCItemManager__
#define __Military_Confrontation__MCItemManager__

#include "MCItem.h"

class MCEquipmentItem;
class MCEffectiveItem;

class MCItemManager {
private:
    MCItemManager();
    
public:
    ~MCItemManager();
    
    static MCItemManager *sharedItemManager();
    
    /* copied data */
    MCItem *itemForObjectId(mc_object_id_t anObjectId);
    MCEquipmentItem *equipmentItemForObjectId(mc_object_id_t anObjectId);
    MCEffectiveItem *effectiveItemForObjectId(mc_object_id_t anObjectId);
    
    /* metadata */
    MCItem *metaItemForObjectId(mc_object_id_t anObjectId);
    MCEquipmentItem *metaEquipmentItemForObjectId(mc_object_id_t anObjectId);
    MCEffectiveItem *metaEffectiveItemForObjectId(mc_object_id_t anObjectId);
    
private:
    /**
     * load all flags
     */
    void loadAllItems();
    
protected:
    void loadEquipmentItems();
    void loadEffectiveItems();
    
private:
    CCDictionary *equipmentItems_; /* 装备类物品 */
    CCDictionary *effectiveItems_; /* 效果类物品 */
};

#endif /* defined(__Military_Confrontation__MCItemManager__) */
