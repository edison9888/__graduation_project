//
//  MCRoleManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRoleManager__
#define __Military_Confrontation__MCRoleManager__

#include "MCRole.h"

class MCNPC;
class MCMonster;

class MCRoleManager {
private:
    MCRoleManager();
    
public:
    ~MCRoleManager();
    
    static MCRoleManager *sharedRoleManager();
    
    void loadData();
    
    MCRole *roleForObjectId(mc_object_id_t anObjectId);
    MCNPC *NPCForObjectId(mc_object_id_t anObjectId);
    MCMonster *monsterForObjectId(mc_object_id_t anObjectId);
    
protected:
    void loadNPCData();
    void loadMonsterData();
    
private:
    CCDictionary *npcs_; /* NPC */
    CCDictionary *monsters_; /* 怪物 */
};


#endif /* defined(__Military_Confrontation__MCRoleManager__) */
