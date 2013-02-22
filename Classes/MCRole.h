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
 */
enum {
    MCTerrans   = MCMakeEnum(0), /* 人类 */
    MCDemons    = MCMakeEnum(1)  /* 魔族 */
};
typedef mc_enum_t MCRoleRace;

/* 角色类型 */
enum {
    MCNPCType       = MCMakeEnum(0), /* NPC */
    MCHeroType      = MCMakeEnum(1), /* 主角 */
    MCMercenaryType = MCMakeEnum(2), /* 佣兵 */
    MCEnemy         = MCMakeEnum(3)  /* 敌人 */
};
typedef mc_enum_t MCRoleType;

/* 角色状态 */
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
typedef mc_enum_t MCRoleState;

/* 基础角色 */
class MCRole : public MCObject {
public:
    virtual bool init() = 0;
    virtual CCSprite *entity() = 0;
    
    /* 下一句话。如果没有则返回NULL；如果本来就没话说则返回默认对白 */
    const char *nextSentence();
    
protected:
    CCSprite *entiry_;
    
    CC_SYNTHESIZE(MCRoleRace, roleRace_, RoleRace); /* 角色种族 */
    CC_SYNTHESIZE(MCRoleType, roleType_, RoleType); /* 角色类型 */
    CC_SYNTHESIZE(mc_hp_t, hp_, HP); /* 角色生命值 */
    CC_SYNTHESIZE(mc_pp_t, pp_, PP); /* 角色体力值 */
    CC_SYNTHESIZE(mc_load_t, load_, Load); /* 角色负重 */
    CC_SYNTHESIZE(MCRoleState, roleState_, RoleState); /* 角色状态 */
    CC_SYNTHESIZE(CCSprite *, face_, Face); /* 角色头像 */
    
    CC_SYNTHESIZE(CCString *, defaultDialogue_, DefaultDialogue); /* 默认对白 */
    CC_SYNTHESIZE(CCArray *, dialogues_, Dialogues); /* 当前场景可能的对白 */
    CC_SYNTHESIZE(mc_index_t, nextSentenceIndex_, NextSentenceIndex); /* 下一句话的索引号 */
};

#endif /* defined(__Military_Confrontation__MCRole__) */
