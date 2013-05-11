//
//  MCMercenaryManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCMercenaryManager__
#define __Military_Confrontation__MCMercenaryManager__

#include "MCMercenary.h"
#include "MCTeam.h"
#include "MCInterfaceMacros.h"

const char *MCMercenaryManagerChoiceMercenaryName();

class MCMercenaryManager {
private:
    MCMercenaryManager();
    
public:
    ~MCMercenaryManager();
    
    static MCMercenaryManager *sharedMercenaryManager();
    
    void loadMercenaries();
    
    MCMercenary *mercenaryForObjectId(mc_object_id_t anObjectId);
    MCMercenary *protoMercenaryForObjectId(mc_object_id_t anObjectId);
    
    /**
     * 已雇佣将返回kMCHired
     */
    mc_ssize_t hire(mc_object_id_t anObjectId);
    
    /**
     * 解雇
     */
    void dismiss(MCMercenary *aMercenary);
    
    void saveData();
    void loadData();
private:
    CCDictionary *mercenaries_; /* 原始数据 */
    MCTeam       *hired_;     /* 现在雇佣的 */
};


#endif /* defined(__Military_Confrontation__MCMercenaryManager__) */
