//
//  MCArmorHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCArmorHandler.h"
#import "MCCSVHandlerMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);
MCDefineIndexAndKey(Icon, 2, icon);
MCDefineIndexAndKey(Price, 3, price);
MCDefineIndexAndKey(Defense, 4, defense);
MCDefineIndexAndKey(Dexterity, 5, dexterity);
MCDefineIndexAndKey(ArmorCheckPenalty, 7, armor-check-penalty);

@implementation MCArmorHandler

+ (NSString *)filename
{
    return @"E002.jpkg";
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
    [content setObject:[data objectAtIndex:kMCNameIndex] forKey:kMCNameKey];
    /* icon */
    object = [data objectAtIndex:kMCIconIndex];
    [content setObject:([(NSString *) object compare:@"-"] == NSOrderedSame
                        ? @""
                        : object)
                forKey:kMCIconKey];
    /* price */
    [content setObject:[data objectAtIndex:kMCPriceIndex] forKey:kMCPriceKey];
    /* defense */
    [content setObject:[data objectAtIndex:kMCDefenseIndex] forKey:kMCDefenseKey];
    /* dexterity */
    [content setObject:@([[data objectAtIndex:kMCDexterityIndex] integerValue]) forKey:kMCDexterityKey];
    /* armor-check-penalty */
    [content setObject:@([[data objectAtIndex:kMCArmorCheckPenaltyIndex] integerValue]) forKey:kMCArmorCheckPenaltyKey];
    
    [self setObject:content forKey:ID];
}

@end
