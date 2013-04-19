//
//  MCDice.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#define MCMakeDiceType(count, size) (0x100 * (count) + (size))
#define MCDiceSize(type)  ((mc_dice_unit_t) ((type) & 0xFF))
#define MCDiceCount(type) ((mc_dice_unit_t) ((type) / 0x100))

typedef struct {
    NSUInteger count;
    NSUInteger size;
} MCDice;

typedef struct {
    NSUInteger min;
    NSUInteger max;
    MCDice     dice;
} MCDiceRange;
