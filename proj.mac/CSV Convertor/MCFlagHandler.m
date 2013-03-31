//
//  MCFlagHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCFlagHandler.h"
#import "MCCoreMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);
MCDefineIndexAndKey(State, 2, state);
MCDefineIndexAndKey(Description, 3, description);

@implementation MCFlagHandler

+ (NSString *)filename
{
    return @"F000.jpkg";
}

+ (NSString *)sourceFilename
{
    return @"标志系统-标志系统.csv";
}

/*
 字段            |  字段说明  | 解释
 ---------------|-----------|---------
 ID             | 标志ID       | 标志ID为标志字典中标志的key
 name           | 标志名称      | 标志的名称
 state          | 标志状态      | 标志的状态
 description    | 标志描述      | 标志的描述
 */
- (void)handleLine:(NSString *)aLine
{
    if ([aLine rangeOfString:@"F"].location != 0
        || [aLine rangeOfString:@"00"].location == 2) {
        return;
    }
    NSArray *data = [aLine componentsSeparatedByString:@","];
    if ([[data objectAtIndex:2] length] == 0) {
        return;
    }
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:4];
    
    /* ID */
    NSString *ID = [data objectAtIndex:kMCIDIndex];
    /* name */
    [content setObject:[data objectAtIndex:kMCNameIndex] forKey:kMCNameKey];
    /* state */
    [content setObject:@([(NSString *)[data objectAtIndex:kMCStateIndex] compare:@"on"] == NSOrderedSame ? 0 : 1) forKey:kMCStateKey];
    /* description */
    [content setObject:[data objectAtIndex:kMCDescriptionIndex] forKey:kMCDescriptionKey];
    
    [self setObject:content forKey:ID];
}

@end
