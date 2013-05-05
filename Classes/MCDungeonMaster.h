//
//  MCDungeonMaster.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCDungeonMaster__
#define __Military_Confrontation__MCDungeonMaster__

#include <cocos2d.h>

USING_NS_CC;

#include "MCTeam.h"
#include "MCDialog.h"

/**
 * 城主
 */
class MCDungeonMaster : public CCObject {
private:
    MCDungeonMaster() { };
    
public:
    static MCDungeonMaster *sharedDungeonMaster();
    
    void speak(const char *aMessage);
    
    /**
     * aRole将要攻击aTarget
     * 由AI调用
     */
    void roleWillAttack(MCRole *aRole, MCRole *aTarget);
    
    /**
     * aRole攻击aTarget
     */
    void roleAttackTarget(MCRole *aRole, MCRole *aTarget);
    
    void destroyGameWorld();
    
    void saveSpawnPoint();
    void loadSpawnPoint();
    
private:
    void dismiss(void *anUserdata);
    
private:
    MCTeam *team_; /* 队伍 */

    CC_SYNTHESIZE(mc_object_id_t, spawnPointID_, SpawnPointID); /* 重生点场景的ID */
};

#endif /* defined(__Military_Confrontation__MCDungeonMaster__) */
