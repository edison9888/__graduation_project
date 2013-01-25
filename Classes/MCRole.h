//
//  MCRole.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRole__
#define __Military_Confrontation__MCRole__

#include "MCObject.h"

/*
 * 角色种族
 * 暂时只有两个
 */
typedef mc_enum_t MCRoleRace;
enum {
    MCTerran    = MCMakeEnum(0), /* 人类 */
    MCMonster   = MCMakeEnum(1)  /* 怪物 */
};

/* 角色类型 */
typedef mc_enum_t MCRoleType;
enum {
    MCNPCType       = MCMakeEnum(0), /* NPC */
    MCHeroType      = MCMakeEnum(1), /* 主角 */
    MCMercenaryType = MCMakeEnum(2), /* 佣兵 */
    MCEnemy         = MCMakeEnum(3)  /* 敌人 */
};

/* 角色状态 */
typedef mc_enum_t MCRoleState;
enum {
    MCNormalState    = MCMakeEnum(0), /* 正常 */
    MCPoisonedState  = MCMakeEnum(1), /* 中毒 */
    MCChaosState     = MCMakeEnum(2), /* 混乱 */
    MCBlindingState  = MCMakeEnum(3), /* 盲 */
    MCVertigoState   = MCMakeEnum(4), /* 眩晕 */
    MCBurningState   = MCMakeEnum(5), /* 燃烧 */
    MCSlowState      = MCMakeEnum(6), /* 迟缓 */
    MCParalysisState = MCMakeEnum(7), /* 麻痹 */ /* 待定 */
    MCInjuredState   = MCMakeEnum(8)  /* 受伤 */ /* 待定 */
};

/* 基础角色 */
class MCRole : public MCObject {
public:
    virtual bool init()=0;
    
    CC_SYNTHESIZE(MCRoleRace, race_, RoleRace); /* 角色种族 */
    CC_SYNTHESIZE(MCRoleType, type_, RoleType); /* 角色类型 */
    CC_SYNTHESIZE(mc_hp_t, hp_, HP); /* 角色生命值 */
    CC_SYNTHESIZE(mc_pp_t, pp_, PP); /* 角色体力值 */
    CC_SYNTHESIZE(mc_load_t, load_, Load); /* 角色负重 */
    CC_SYNTHESIZE(MCRoleState, state_, RoleState); /* 角色状态 */
};

#endif /* defined(__Military_Confrontation__MCRole__) */
