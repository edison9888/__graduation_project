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

extern const char *kMCShadow;
extern const char *kMCSelectedShadow;

class MCNPC;
class MCEnemy;

class MCRoleManager {
private:
    MCRoleManager();
    
public:
    ~MCRoleManager();
    
    static MCRoleManager *sharedRoleManager();
    
    /* copied data */
    MCRole *roleForObjectId(mc_object_id_t anObjectId);
    MCNPC *NPCForObjectId(mc_object_id_t anObjectId);
    MCEnemy *enemyForObjectId(mc_object_id_t anObjectId);
    
    /* 以下三个方法尽量不要使用！ */
    /* metadata */
    MCRole *metaRoleForObjectId(mc_object_id_t anObjectId);
    MCNPC *metaNPCForObjectId(mc_object_id_t anObjectId);
    MCEnemy *metaEnemyForObjectId(mc_object_id_t anObjectId);
    
private:
    void loadData();
    
    void loadNPCData();
    void loadEnemyData();
    
private:
    CCDictionary *npcs_; /* NPC */
    CCDictionary *enemies_; /* 怪物 */
};


#endif /* defined(__Military_Confrontation__MCRoleManager__) */
