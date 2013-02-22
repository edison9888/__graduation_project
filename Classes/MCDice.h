//
//  MCDice.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-20.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCDice__
#define __Military_Confrontation__MCDice__

#include <cocos2d.h>
#include "MCType.h"

USING_NS_CC;

#define MCMakeDiceType(count, size) (0x100 * (count) + (size))
#define MCDiceSize(type)  ((mc_dice_unit_t) ((type) & 0x00FF))
#define MCDiceCount(type) ((mc_dice_unit_t) ((type) / 0x100))

enum {
    MC1D4Dice   = MCMakeDiceType(1, 4),  /* 1个4面骰子 */
    MC1D6Dice   = MCMakeDiceType(1, 6),  /* 1个6面骰子 */
    MC1D8Dice   = MCMakeDiceType(1, 8),  /* 1个8面骰子 */
    MC1D10Dice  = MCMakeDiceType(1, 10), /* 1个10面骰子 */
    MC1D12Dice  = MCMakeDiceType(1, 12), /* 1个12面骰子 */
    MC1D20Dice  = MCMakeDiceType(1, 20), /* 1个20面骰子 */
    MC2D4Dice   = MCMakeDiceType(2, 4),  /* 2个4面骰子 */
    MC2D6Dice   = MCMakeDiceType(2, 6),  /* 2个6面骰子 */
    MC2D8Dice   = MCMakeDiceType(2, 8),  /* 2个8面骰子 */
    MC2D20Dice  = MCMakeDiceType(2, 20), /* 2个20面骰子 */
    
    /* 特殊骰子 */
    MC3D4Dice   = MCMakeDiceType(2, 4)   /* 3个4面骰子 */
};
typedef mc_ushort_t MCDiceType;
typedef mc_ushort_t mc_dice_unit_t;

class MCDice : public CCObject {
public:
    /* 初始化 */
    bool init();
    
    /* 掷骰子 */
    mc_dice_unit_t roll();
    
    /* 返回骰子的面数 */
    inline mc_dice_unit_t size()
    {
        return MCDiceSize(_type);
    }
    
    /* 返回骰子的个数 */
    inline mc_dice_unit_t count()
    {
        return MCDiceCount(_type);
    }
    
    CREATE_FUNC(MCDice);
    
private:
    MCDiceType _type; /* 骰子类型 */
    
    friend class MCDiceMaker;
};

class MCDiceMaker : public CCObject {
    
public:
    /* 初始化 */
    bool init();
    
    /* 返回单实例 */
    static MCDiceMaker *sharedDiceMaker();
    
    /* 根据骰子类型获取骰子 */
    MCDice *diceWithType(MCDiceType type);
    
private:
    CCDictionary *dices_;
};

#endif /* defined(__Military_Confrontation__MCDice__) */
