//
//  MCDice.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-20.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCDice.h"

static MCDiceMaker *__shared_dice_maker = NULL;

/* 初始化 */
bool
MCDice::init()
{
    return true;
}

/* 掷骰子 */
mc_dice_unit_t
MCDice::roll()
{
    mc_dice_unit_t size = MCDiceSize(_type);
    mc_dice_unit_t count = MCDiceCount(_type);
    mc_dice_unit_t sum = 0;
    mc_dice_unit_t i;
    for (i = 0; i < count; ++i) {
        sum += rand() % size + 1;
    }
    
    return sum;
}

MCDiceMaker::~MCDiceMaker()
{
    CC_SAFE_RELEASE(dices_);
}

/* 初始化 */
bool
MCDiceMaker::init()
{
    dices_ = CCDictionary::create();
    dices_->retain();
    
    return true;
}

/* 返回单实例 */
MCDiceMaker *
MCDiceMaker::sharedDiceMaker()
{
    if (__shared_dice_maker == NULL) {
        __shared_dice_maker = new MCDiceMaker;
        if (__shared_dice_maker && __shared_dice_maker->init()) {
            struct timeval tv;
            
            gettimeofday(&tv, NULL);
            srand(tv.tv_usec);
            __shared_dice_maker->autorelease();
        } else {
            delete __shared_dice_maker;
            __shared_dice_maker = NULL;
        }
    }
    
    return __shared_dice_maker;
}

/* 根据骰子类型获取骰子 */
MCDice *
MCDiceMaker::diceWithType(MCDiceType type)
{
    MCDice *dice = (MCDice *) dices_->objectForKey(type);
    if (dice == NULL) {
        dice = MCDice::create();
        dice->_type = type;
        dices_->setObject(dice, type);
    }
    
    return dice;
}

