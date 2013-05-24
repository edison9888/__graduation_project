//
//  MCRoleProperty.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-9.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef Military_Confrontation_MCRoleProperty_h
#define Military_Confrontation_MCRoleProperty_h

#include <cocos2d.h>
#include "MCType.h"

USING_NS_CC;

/**
 * MCRole 角色属性类型
 */
typedef mc_short_t mc_hp_t;   /* 生命值类型 */
typedef float mc_pp_t;   /* 体力值类型 */
typedef mc_ushort_t mc_proficiency_t; /* 熟练度类型 */

/**
 * 角色种族
 */
enum {
    MCTerrans       = MCMakeEnum(0), /* 人类 */
    MCTerrestrial   = MCMakeEnum(0), /* 陆行种 */
    MCFlying        = MCMakeEnum(1), /* 飞行种 */
    MCProtoss       = MCMakeEnum(2), /* 神 */
};
typedef mc_enum_t MCRoleRace;

/**
 * 角色状态
 */
enum {
    MCNormalState    = 0,             /* 正常 */ /* 通常状态 */
    MCCurseState     = MCMakeEnum(0), /* 诅咒 */ /* 移动减少的体力值增加(double)，发动攻击会扣血(一半) */
    MCParalysisState = MCMakeEnum(1), /* 麻痹 */ /* 不动几秒钟 */
    MCVertigoState   = MCMakeEnum(2), /* 眩晕 */ /* 不动几秒钟 */
    MCPoisonedState  = MCMakeEnum(3), /* 中毒 */ /* 持续扣血，每秒3点血 */
    MCBlindingState  = MCMakeEnum(4), /* 盲 */   /* 命中下降，-3点敏捷 */
    MCChaosState     = MCMakeEnum(5)  /* 混乱 */ /* 角色不受玩家控制，把所有人当做敌人 */
};
typedef mc_enum_t MCRoleState;

extern const char *kMCNormalState;
extern const char *kMCCurseState;
extern const char *kMCParalysisState;
extern const char *kMCVertigoState;
extern const char *kMCPoisonedState;
extern const char *kMCBlindingState;
extern const char *kMCChaosState;

const char *MCRoleStateGetName(MCRoleState aRoleState);

enum {
    MCSpriteFrameSmallSize = 1,
    MCSpriteFrameMediumSize = 2,
    MCSpriteFrameLargeSize = 3
};
typedef mc_enum_t MCSpriteFrameType;

/**
 * 人物碰撞矩形(正方形)
 * 直接设置为三种规格的大小
 */
const CCSize kMCSpriteFrameSmallSize = CCSizeMake(24, 24);
const CCSize kMCSpriteFrameMediumSize = CCSizeMake(44, 44);
const CCSize kMCSpriteFrameLargeSize = CCSizeMake(72, 72);

#endif
