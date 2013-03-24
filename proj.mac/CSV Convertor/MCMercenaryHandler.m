//
//  MCMercenaryHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCMercenaryHandler.h"

@implementation MCMercenaryHandler

+ (NSString *)filename
{
    return @"M000.mpkg";
}

+ (NSString *)sourceFilename
{
    return @"游戏角色-佣兵列表.csv";
}

- (void)handleLine:(NSString *)aLine
{
    if ([aLine rangeOfString:@"M"].location != 0
        || [aLine rangeOfString:@"00"].location == 2) {
        return;
    }
    NSArray *data = [aLine componentsSeparatedByString:@","];
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:16];
    id object;
    
    /* ID */
    NSString *ID = [data objectAtIndex:0];
    
    /* name */
    [content setObject:[data objectAtIndex:1] forKey:@"name"];
    /* AI */
    [content setObject:[data objectAtIndex:2] forKey:@"AI"];
    /* HP */
    [content setObject:@([[data objectAtIndex:3] integerValue]) forKey:@"HP"];
    /* PP */
    [content setObject:@([[data objectAtIndex:4] integerValue]) forKey:@"PP"];
    /* AC */
    [content setObject:@([[data objectAtIndex:5] integerValue]) forKey:@"AC"];
    /* armor-check-penalty */
    [content setObject:@([[data objectAtIndex:6] integerValue]) forKey:@"armor-check-penalty"];
    /* damage */
    NSArray *tempComponents = [[data objectAtIndex:7] componentsSeparatedByString:@"d"];
    NSDictionary *damage = @{
                             @"count": @([[tempComponents objectAtIndex:0] integerValue]),
                             @"size": @([[tempComponents objectAtIndex:1] integerValue])};
    [content setObject:damage forKey:@"damage"];
    /* damage-bonus */
    [content setObject:@([[data objectAtIndex:10] integerValue]) forKey:@"damage-bonus"];
    /* critical-hit-visible */
    object = [data objectAtIndex:11];
    NSDictionary *criticalHitVisible;
    if ([object rangeOfString:@"-"].length == 0) {
        criticalHitVisible = @{
                               @"min": @([object integerValue]),
                               @"max": @(20),
                               @"dice":@{@"count":@(1),
                                         @"size":@(20)}};
    } else {
        tempComponents = [object componentsSeparatedByString:@"-"];
        criticalHitVisible = @{
                               @"min": @([[tempComponents objectAtIndex:0] integerValue]),
                               @"max": @([[tempComponents objectAtIndex:1] integerValue]),
                               @"dice":@{@"count":@(1),
                                         @"size":@(20)}};
    }
    [content setObject:criticalHitVisible forKey:@"critical-hit-visible"];
    /* critical-hit-invisible */
    object = [data objectAtIndex:12];
    NSDictionary *criticalHitInvisible;
    if ([object rangeOfString:@"-"].length == 0) {
        criticalHitInvisible = @{
                                 @"min": @([object integerValue]),
                                 @"max": @(20),
                                 @"dice":@{@"count":@(1),
                                           @"size":@(20)}};
    } else {
        tempComponents = [object componentsSeparatedByString:@"-"];
        criticalHitInvisible = @{
                                 @"min": @([[tempComponents objectAtIndex:0] integerValue]),
                                 @"max": @([[tempComponents objectAtIndex:1] integerValue]),
                                 @"dice":@{@"count":@(1),
                                           @"size":@(20)}};
    }
    [content setObject:criticalHitInvisible forKey:@"critical-hit-invisible"];
    /* critical-hit */
    [content setObject:@([[data objectAtIndex:13] integerValue]) forKey:@"critical-hit"];
    /* distance */
    [content setObject:@([[data objectAtIndex:14] integerValue]) forKey:@"distance"];
    /* skills */
    NSString *skillA = [data objectAtIndex:15];
    NSString *skillB = [data objectAtIndex:16];
    NSString *skillC = [data objectAtIndex:17];
    NSString *skillD = [data objectAtIndex:18];
    NSDictionary *skills = @{@"A":[skillA compare:@"-"] == NSOrderedSame ? [NSNull null] : skillA,
                             @"B":[skillB compare:@"-"] == NSOrderedSame ? [NSNull null] : skillB,
                             @"C":[skillC compare:@"-"] == NSOrderedSame ? [NSNull null] : skillC,
                             @"D":[skillD compare:@"-"] == NSOrderedSame ? [NSNull null] : skillD};
    [content setObject:skills forKey:@"skills"];
    /* effect */
    id effect = [data objectAtIndex:19];
    if ([(NSString *) effect compare:@"-"] != NSOrderedSame) {
        [content setObject:effect forKey:@"effect"];
        /* effect-check */
        object = [data objectAtIndex:20];
        NSDictionary *effectCheck;
        if ([object rangeOfString:@"-"].length == 0) {
            effectCheck = @{
                            @"min": @([object integerValue]),
                            @"max": @(20),
                            @"dice":@{@"count":@(1),
                                      @"size":@(20)}};
        } else {
            tempComponents = [object componentsSeparatedByString:@"-"];
            effectCheck = @{
                            @"min": @([[tempComponents objectAtIndex:0] integerValue]),
                            @"max": @([[tempComponents objectAtIndex:1] integerValue]),
                            @"dice":@{@"count":@(1),
                                      @"size":@(20)}};
        }
        [content setObject:effectCheck forKey:@"effect-check"];
    } else {
        [content setObject:[NSNull null] forKey:@"effect"];
        [content setObject:[NSNull null] forKey:@"effect-check"];
    }
    
    [self setObject:content forKey:ID];
}

@end
