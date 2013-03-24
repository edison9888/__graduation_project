//
//  MCItemHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCItemHandler.h"

@implementation MCItemHandler

+ (NSString *)filename
{
    return @"E100.epkg";
}

+ (NSString *)sourceFilename
{
    return @"游戏道具-游戏道具.csv";
}

/*
 字段          |  字段说明 | 解释
 --------------|----------|---------
 icon           | 道具图标      | 道具图标的路径，需要后缀名
 path           | 效果路径      | 效果精灵表的路径，不需要后缀名
 price          | 价格         | 购买所需的金钱
 radius         | 效果半径      | 效果行程的圆的半径
 active-time    | 完成所需时间   | 从使用到使用完毕所经过的时间
 CD             | 冷却时间      | 到下一使用道具能所需要的等待时间
 item-type      | 道具类型      | 值为1或者0，1表示药水，使用立即见效，不需要判定，0表示陷阱
 check          | 命中范围      | 值为null或者对象，item-type为1的时候为null，0的时候为表示一个D20的命中范围
 hp             | 生命值        | 补充或减少的生命值
 pp             | 体力值        | 补充或减少的体力值
 positive-state | 正状态        | 若判定成功将增加的状态
 negative-state | 负状态        | 若判定成功将减少的状态
 lasting-time   | 持续时间      | 若增加了状态，状态的持续时间
 */
- (void)handleLine:(NSString *)aLine
{
    if (([aLine rangeOfString:@"P"].location != 0
         && [aLine rangeOfString:@"T"].location != 0)
        || [aLine rangeOfString:@"000"].location == 1) {
        return;
    }
    NSArray *data = [aLine componentsSeparatedByString:@","];
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:12];
    id object;
    NSArray *tempComponents;
    
    /* ID */
    NSString *ID = [data objectAtIndex:0];
    
    /* name */
    [content setObject:[data objectAtIndex:1] forKey:@"name"];
    /* icon */
    object = [data objectAtIndex:2];
    [content setObject:([(NSString *) object compare:@"-"] == NSOrderedSame
                        ? @""
                        : object)
                forKey:@"icon"];
    /* path */
    object = [data objectAtIndex:3];
    [content setObject:([(NSString *) object compare:@"-"] == NSOrderedSame
                        ? @""
                        : object)
                forKey:@"path"];
    /* price */
    [content setObject:@([[data objectAtIndex:4] integerValue]) forKey:@"price"];
    /* radius */
    [content setObject:@([[data objectAtIndex:5] integerValue]) forKey:@"radius"];
    /* item-type && check */
    object = [data objectAtIndex:6];
    if ([(NSString *) object compare:@"-"] == NSOrderedSame) {
        [content setObject:@(1) forKey:@"item-type"];
        [content setObject:[NSNull null] forKey:@"check"];
    } else {
        [content setObject:@(0) forKey:@"item-type"];
        NSDictionary *check;
        if ([object rangeOfString:@"-"].length == 0) {
            check = @{
                            @"min": @([object integerValue]),
                            @"max": @(20),
                            @"dice":@{@"count":@(1),
                                      @"size":@(20)}};
        } else {
            tempComponents = [object componentsSeparatedByString:@"-"];
            check = @{
                            @"min": @([[tempComponents objectAtIndex:0] integerValue]),
                            @"max": @([[tempComponents objectAtIndex:1] integerValue]),
                            @"dice":@{@"count":@(1),
                                      @"size":@(20)}};
        }
        [content setObject:check forKey:@"check"];
    }
    /* HP */
    [content setObject:@([[data objectAtIndex:7] integerValue]) forKey:@"hp"];
    /* PP */
    [content setObject:@([[data objectAtIndex:8] integerValue]) forKey:@"pp"];
    /* positive-state */
    [content setObject:@([[data objectAtIndex:9] integerValue]) forKey:@"positive-state"];
    /* negative-state */
    [content setObject:@([[data objectAtIndex:10] integerValue]) forKey:@"negative-state"];
    /* lasting-time */
    [content setObject:@([[data objectAtIndex:11] integerValue]) forKey:@"lasting-time"];
    
    [self setObject:content forKey:ID];
}

@end
