//
//  MCRoleProperty.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCRoleProperty.h"


const char *kMCNormalState    = "正常"; /* 通常状态 */
const char *kMCCurseState     = "诅咒"; /* 移动减少的体力值增加，发动攻击会扣血 */
const char *kMCParalysisState = "麻痹"; /* 不动几秒钟 */
const char *kMCVertigoState   = "眩晕"; /* 不动几秒钟 */
const char *kMCPoisonedState  = "中毒"; /* 持续扣血 */
const char *kMCBlindingState  = "盲";   /* 命中下降 */
const char *kMCChaosState     = "混乱"; /* 把所有人当做敌人 */

const char *
MCRoleStateGetName(MCRoleState aRoleState)
{
    if (aRoleState == MCNormalState) {
        return kMCNormalState;
    } else if (aRoleState == MCCurseState) {
        return kMCCurseState;
    } else if (aRoleState == MCParalysisState) {
        return kMCParalysisState;
    } else if (aRoleState == MCVertigoState) {
        return kMCVertigoState;
    } else if (aRoleState == MCPoisonedState) {
        return kMCPoisonedState;
    } else if (aRoleState == MCBlindingState) {
        return kMCBlindingState;
    } else if (aRoleState == MCChaosState) {
        return kMCChaosState;
    }
    
    return "未知状态";
}