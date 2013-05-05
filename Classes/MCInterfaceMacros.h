 //
//  MCInterfaceMacros.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef Military_Confrontation_MCInterfaceMacros_h
#define Military_Confrontation_MCInterfaceMacros_h

#define kMCNotEnoughMoney -1
#define kMCHandleSucceed 0
#define kMCHandleFailured 1
#define kMCFullLevel 2

#define kMCHired 1
#define kMCFullTeam 2

/**
 * 购买接口，返回购买的个数。不够钱购买则返回-1
 */
#define MCDefineBuyInterface(var, Var)                        \
inline mc_ssize_t buy##Var(mc_size_t aValue) {                \
    var->count += aValue;                                     \
    mc_ssize_t difference = var->count - kMCItemMax;          \
    MCEffectiveItem *item = (MCEffectiveItem *) var->item;    \
    if (difference > 0) {                                     \
        var->count -= difference;                             \
        aValue -= difference;                                 \
    }                                                         \
    mc_price_t cost = item->getPrice() * aValue;              \
    if (cost > money_) {                                      \
        return kMCNotEnoughMoney;                             \
    }                                                         \
    money_ -= cost;                                           \
    return aValue;                                            \
}

/**
 * 卖出接口，返回卖出的个数。
 */
#define MCDefineSellInterface(var, Var)         \
inline mc_size_t sell##Var(mc_size_t aValue) {  \
    mc_size_t difference = var->count - aValue; \
    if (difference < 0) {                       \
        var->count += difference;               \
        aValue += difference;                   \
    }                                           \
    return aValue;                              \
}

/**
 * 定义操作接口
 */
#define MCDefineInterface(var, Var) \
    MCDefineBuyInterface(var, Var)  \

#endif
