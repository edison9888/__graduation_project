//
//  MCTaskHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCTaskHandler.h"
#import "MCCoreMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);

static const NSString   *kMCTargetsKey = @"targets";

static const NSUInteger kMCTargetAIndex = 2;
static const NSUInteger kMCTargetBIndex = 4;
static const NSUInteger kMCTargetCIndex = 6;
static const NSUInteger kMCTargetDIndex = 8;

static const NSUInteger kMCTargetACountIndex = 3;
static const NSUInteger kMCTargetBCountIndex = 5;
static const NSUInteger kMCTargetCCountIndex = 7;
static const NSUInteger kMCTargetDCountIndex = 9;

MCDefineIndexAndKey(TrapType, 11, trap-type);

static const NSUInteger kMCTrapLimitedAIndex = 12;
static const NSUInteger kMCTrapLimitedBIndex = 13;
static const NSString   *kMCTrapLimitedKey = @"trap-limited";

static const NSUInteger kMCPotionLimitedAIndex = 14;
static const NSUInteger kMCPotionLimitedBIndex = 15;
static const NSString   *kMCPotionLimitedKey = @"potion-limited";

static const NSUInteger kMCTrapBonusAIndex = 16;
static const NSUInteger kMCTrapBonusBIndex = 17;
static const NSString   *kMCTrapBonusdKey = @"trap-bonus";

static const NSUInteger kMCPotionBonusAIndex = 18;
static const NSUInteger kMCPotionBonusBIndex = 19;
static const NSString   *kMCPotionBonusKey = @"potion-bonus";

MCDefineIndexAndKey(CashPledge, 20, cash-pledge);
MCDefineIndexAndKey(Bonus, 21, bonus);
MCDefineIndexAndKey(Flag, 22, flag);
MCDefineIndexAndKey(Region, 23, region);
MCDefineIndexAndKey(Description, 24, description);

@implementation MCTaskHandler

+ (NSString *)filename
{
    return @"T000.jpkg";
}

+ (NSString *)sourceFilename
{
    return @"任务系统-任务系统.csv";
}

/*
 字段            |  字段说明     | 解释
 ---------------|--------------|------------------------------------------------------------
 ID             | 任务ID       | 任务ID为任务字典中任务的key
 name           | 任务名称      | 任务的名称
 targets        | 任务目标      | 任务的目标字典，key为目标的ID，value为目标的数量
 trap-type      | 陷阱类型      | 值为1-5，分别为火球、诅咒、麻痹、迷雾、闪光
 trap-limited   | 陷阱限制      | 二元组，第一个值为大范围陷阱的限制个数，第二个值为大伤害陷阱的限制个数
 potion-limited | 药水限制      | 二元组，第一个值为生命药水的限制个数，第二个值为体力药水的限制个数
 trap-bonus     | 赠送的陷阱数量 | 二元组，第一个值为大范围陷阱的赠送个数，第二个值为大伤害陷阱的赠送个数
 potion-bonus   | 赠送的药水数量 | 二元组，第一个值为生命药水的赠送个数，第二个值为体力药水的赠送个数
 cash-pledge    | 任务押金      | 任务的押金，完成任务后退回，任务失败后不退回
 bonus          | 任务奖励      | 顺利完成任务的奖励
 flag           | 开启的标志ID  | 接受任务后打开的标志的ID
 description    | 任务描述      | 任务的描述
 */
- (void)handleLine:(NSString *)aLine
{
    if ([aLine rangeOfString:@"T"].location != 0
         || [aLine rangeOfString:@"00"].location == 2) {
        return;
    }
    NSArray *data = [aLine componentsSeparatedByString:@","];
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:12];
    id object;
    
    /* ID */
    NSString *ID = [data objectAtIndex:kMCIDIndex];
    /* name */
    [content setObject:[data objectAtIndex:kMCNameIndex] forKey:kMCNameKey];
    /* targets */
    NSMutableDictionary *targets = [[NSMutableDictionary alloc] initWithCapacity:2];
    object = [data objectAtIndex:kMCTargetAIndex];
    [targets setObject:@([[data objectAtIndex:kMCTargetACountIndex] integerValue])
                forKey:[(NSString *)object stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]]];
    /* 测试多个目标 */
    object = [data objectAtIndex:kMCTargetBIndex];
    if ([(NSString *) object compare:@"-"] != NSOrderedSame) {
        [targets setObject:@([[data objectAtIndex:kMCTargetBCountIndex] integerValue])
                    forKey:[(NSString *)object stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]]];
    }
    object = [data objectAtIndex:kMCTargetCIndex];
    if ([(NSString *) object compare:@"-"] != NSOrderedSame) {
        [targets setObject:@([[data objectAtIndex:kMCTargetCCountIndex] integerValue])
                    forKey:[(NSString *)object stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]]];
    }
    object = [data objectAtIndex:kMCTargetDIndex];
    if ([(NSString *) object compare:@"-"] != NSOrderedSame) {
        [targets setObject:@([[data objectAtIndex:kMCTargetDCountIndex] integerValue])
                    forKey:[(NSString *)object stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]]];
    }
    [content setObject:targets forKey:kMCTargetsKey];
    /* trap-type */
    NSString *trapType = [data objectAtIndex:kMCTrapTypeIndex];
    if ([trapType compare:@"火球"] == NSOrderedSame) {
        [content setObject:@(1) forKey:kMCTrapTypeKey];
    } else if ([trapType compare:@"诅咒"] == NSOrderedSame) {
        [content setObject:@(2) forKey:kMCTrapTypeKey];
    } else if ([trapType compare:@"麻痹"] == NSOrderedSame) {
        [content setObject:@(4) forKey:kMCTrapTypeKey];
    } else if ([trapType compare:@"迷雾"] == NSOrderedSame) {
        [content setObject:@(8) forKey:kMCTrapTypeKey];
    } else if ([trapType compare:@"闪光"] == NSOrderedSame) {
        [content setObject:@(16) forKey:kMCTrapTypeKey];
    } else {
        NSLog(@"未知陷阱类型");
        exit(1);
    }
    /* trap-limited */
    [content setObject:@[@([[data objectAtIndex:kMCTrapLimitedAIndex] integerValue]),
                        @([[data objectAtIndex:kMCTrapLimitedBIndex] integerValue])]
                forKey:kMCTrapLimitedKey];
    /* potion-limited */
    [content setObject:@[@([[data objectAtIndex:kMCPotionLimitedAIndex] integerValue]),
                        @([[data objectAtIndex:kMCPotionLimitedBIndex] integerValue])]
                forKey:kMCPotionLimitedKey];
    /* trap-bonus */
    [content setObject:@[@([[data objectAtIndex:kMCTrapBonusAIndex] integerValue]),
                        @([[data objectAtIndex:kMCTrapBonusBIndex] integerValue])]
                forKey:kMCTrapBonusdKey];
    /* potion-bonus */
    [content setObject:@[@([[data objectAtIndex:kMCPotionBonusAIndex] integerValue]),
                        @([[data objectAtIndex:kMCPotionBonusBIndex] integerValue])]
                forKey:kMCPotionBonusKey];
    /* cash-pledge */
    [content setObject:@([[data objectAtIndex:kMCCashPledgeIndex] integerValue]) forKey:kMCCashPledgeKey];
    /* bonus */
    [content setObject:@([[data objectAtIndex:kMCBonusIndex] integerValue]) forKey:kMCBonusKey];
    /* flag */
    [content setObject:[data objectAtIndex:kMCFlagIndex] forKey:kMCFlagKey];
    /* region */
    [content setObject:[data objectAtIndex:kMCRegionIndex] forKey:kMCRegionKey];
    /* description */
    [content setObject:[data objectAtIndex:kMCDescriptionIndex] forKey:kMCDescriptionKey];
    
    [self setObject:content forKey:ID];
}

@end
