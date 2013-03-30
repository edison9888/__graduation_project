//
//  MCTaskHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCTaskHandler.h"

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
    NSString *ID = [data objectAtIndex:0];
    /* name */
    [content setObject:[data objectAtIndex:1] forKey:@"name"];
    /* targets */
    NSMutableDictionary *targets = [[NSMutableDictionary alloc] initWithCapacity:2];
    [targets setObject:@([[data objectAtIndex:3] integerValue])
                forKey:[data objectAtIndex:2]];
    /* 测试多个目标 */
    object = [data objectAtIndex:4];
    if ([(NSString *) object compare:@"-"] != NSOrderedSame) {
        [targets setObject:@([[data objectAtIndex:5] integerValue])
                    forKey:object];
    }
    object = [data objectAtIndex:6];
    if ([(NSString *) object compare:@"-"] != NSOrderedSame) {
        [targets setObject:@([[data objectAtIndex:7] integerValue])
                    forKey:object];
    }
    object = [data objectAtIndex:8];
    if ([(NSString *) object compare:@"-"] != NSOrderedSame) {
        [targets setObject:@([[data objectAtIndex:9] integerValue])
                    forKey:object];
    }
    [content setObject:targets forKey:@"targets"];
    /* trap-type */
    NSString *trapType = [data objectAtIndex:11];
    if ([trapType compare:@"火球"] == NSOrderedSame) {
        [content setObject:@(1) forKey:@"trap-type"];
    } else if ([trapType compare:@"诅咒"] == NSOrderedSame) {
        [content setObject:@(2) forKey:@"trap-type"];
    } else if ([trapType compare:@"麻痹"] == NSOrderedSame) {
        [content setObject:@(4) forKey:@"trap-type"];
    } else if ([trapType compare:@"迷雾"] == NSOrderedSame) {
        [content setObject:@(8) forKey:@"trap-type"];
    } else if ([trapType compare:@"闪光"] == NSOrderedSame) {
        [content setObject:@(16) forKey:@"trap-type"];
    } else {
        NSLog(@"未知陷阱类型");
        exit(1);
    }
    /* trap-limited */
    [content setObject:@[@([[data objectAtIndex:12] integerValue]),
                        @([[data objectAtIndex:13] integerValue])]
                forKey:@"trap-limited"];
    /* potion-limited */
    [content setObject:@[@([[data objectAtIndex:14] integerValue]),
                        @([[data objectAtIndex:15] integerValue])]
                forKey:@"potion-limited"];
    /* trap-bonus */
    [content setObject:@[@([[data objectAtIndex:16] integerValue]),
                        @([[data objectAtIndex:17] integerValue])]
                forKey:@"trap-bonus"];
    /* potion-bonus */
    [content setObject:@[@([[data objectAtIndex:18] integerValue]),
                        @([[data objectAtIndex:19] integerValue])]
                forKey:@"potion-bonus"];
    /* cash-pledge */
    [content setObject:[data objectAtIndex:20] forKey:@"cash-pledge"];
    /* bonus */
    [content setObject:@([[data objectAtIndex:21] integerValue]) forKey:@"bonus"];
    /* flag */
    [content setObject:[data objectAtIndex:22] forKey:@"flag"];
    /* description */
    [content setObject:[data objectAtIndex:23] forKey:@"description"];
    
    [self setObject:content forKey:ID];
}

@end
