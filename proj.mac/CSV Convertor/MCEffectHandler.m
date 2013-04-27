//
//  MCEffectHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCEffectHandler.h"
#import "MCCoreMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);
MCDefineIndexAndKey(Path, 2, path);
MCDefineIndexAndKey(Type, 3, type);

@implementation MCEffectHandler

+ (NSString *)filename
{
    return @"E700.jpkg";
}

+ (NSString *)sourceFilename
{
    return @"游戏机制-效果列表.csv";
}

/*
 字段                 |  字段说明  | 解释
 ---------------------|-----------|---------
 */
- (void)handleLine:(NSString *)aLine
{
    NSArray *data = [aLine componentsSeparatedByString:@","];
    if ([data count] != 4 || [[data objectAtIndex:2] length] < 1) {
        return;
    }
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:7];
    
    /* ID */
    NSString *ID = [data objectAtIndex:0];
    
    /* name */
    [content setObject:[data objectAtIndex:kMCNameIndex] forKey:kMCNameKey];
    /* path */
    [content setObject:[data objectAtIndex:kMCPathIndex] forKey:kMCPathKey];
    /* type */
    [content setObject:@([[data objectAtIndex:kMCTypeIndex] integerValue]) forKey:kMCTypeKey];
    
    [self setObject:content forKey:ID];
}

@end
