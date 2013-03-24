//
//  MCArmorHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCArmorHandler.h"

@implementation MCArmorHandler

+ (NSString *)filename
{
    return @"E002.epkg";
}

+ (NSString *)sourceFilename
{
    return @"游戏道具-防具系统.csv";
}

/*
 字段                 |  字段说明  | 解释
 ---------------------|-----------|---------
 ID                  | 防具ID     | 防具ID为道具字典中防具的key
 name                | 防具名称    | 防具的名称
 icon                | 防具图标    | 防具图标的路径，需要后缀名
 price               | 升级基础价格 | 升级一次所需的基础金钱
 defense             | 防具加值    | 参与闪避判断的值
 dexterity           | 敏捷调整值  | 参与闪避判断的值
 armor-check-penalty | 敏捷调整值  | 抵消伤害的值
 */
- (void)handleLine:(NSString *)aLine
{
    if (([aLine rangeOfString:@"H"].location != 0
         && [aLine rangeOfString:@"A"].location != 0
         && [aLine rangeOfString:@"B"].location != 0)) {
        return;
    }
    NSArray *data = [aLine componentsSeparatedByString:@","];
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:7];
    id object;
    
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
    /* price */
    [content setObject:@([[data objectAtIndex:3] integerValue]) forKey:@"price"];
    /* defense */
    [content setObject:@([[data objectAtIndex:4] integerValue]) forKey:@"defense"];
    /* dexterity */
    [content setObject:@([[data objectAtIndex:5] integerValue]) forKey:@"dexterity"];
    /* armor-check-penalty */
    [content setObject:@([[data objectAtIndex:7] integerValue]) forKey:@"armor-check-penalty"];
    
    [self setObject:content forKey:ID];
}

@end
