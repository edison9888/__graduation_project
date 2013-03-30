//
//  MCMercenaryHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCMercenaryHandler.h"
#import "MCCSVHandlerMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);
MCDefineIndexAndKey(Face, 2, face);
MCDefineIndexAndKey(SpriteSheet, 3, sprite-sheet);
MCDefineIndexAndKey(AI, 4, AI);
MCDefineIndexAndKey(Cost, 5, cost);
MCDefineIndexAndKey(HP, 6, HP);
MCDefineIndexAndKey(PP, 7, PP);
MCDefineIndexAndKey(AC, 8, AC);
MCDefineIndexAndKey(ArmorCheckPenalty, 9, armor-check-penalty);
MCDefineIndexAndKey(Damage, 10, damage);
MCDefineIndexAndKey(DamageBonus, 13, damage-bonus);
MCDefineIndexAndKey(CriticalHitVisible, 14, critical-hit-visible);
MCDefineIndexAndKey(CriticalHitInvisible, 15, critical-hit-invisible);
MCDefineIndexAndKey(CriticalHit, 16, critical-hit);
MCDefineIndexAndKey(Distance, 17, distance);

static const NSUInteger kMCSkillAIndex = 18;
static const NSUInteger kMCSkillBIndex = 19;
static const NSUInteger kMCSkillCIndex = 20;
static const NSUInteger kMCSkillDIndex = 21;
static const NSString   *kMCSkillsKey = @"skills";

MCDefineIndexAndKey(Effect, 22, effect);
MCDefineIndexAndKey(EffectCheck, 23, effect-check);

@implementation MCMercenaryHandler

+ (NSString *)filename
{
    return @"M000.jpkg";
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
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:24];
    id object;
    
    /* ID */
    NSString *ID = [data objectAtIndex:0];
    
    /* name */
    [content setObject:[data objectAtIndex:kMCNameIndex] forKey:kMCNameKey];
    /* face */
    [content setObject:[data objectAtIndex:kMCFaceIndex] forKey:kMCFaceKey];
    /* sprite-sheet */
    [content setObject:[data objectAtIndex:kMCSpriteSheetIndex] forKey:kMCSpriteSheetKey];
    /* AI */
    [content setObject:[data objectAtIndex:kMCAIIndex] forKey:kMCAIKey];
    /* cost */
    [content setObject:[data objectAtIndex:kMCCostIndex] forKey:kMCCostKey];
    /* HP */
    [content setObject:@([[data objectAtIndex:kMCHPIndex] integerValue]) forKey:kMCHPKey];
    /* PP */
    [content setObject:@([[data objectAtIndex:kMCPPIndex] integerValue]) forKey:kMCPPKey];
    /* AC */
    [content setObject:@([[data objectAtIndex:kMCACIndex] integerValue]) forKey:kMCACKey];
    /* armor-check-penalty */
    [content setObject:@([[data objectAtIndex:kMCArmorCheckPenaltyIndex] integerValue]) forKey:kMCArmorCheckPenaltyKey];
    /* damage */
    NSArray *tempComponents = [[data objectAtIndex:kMCDamageIndex] componentsSeparatedByString:@"d"];
    NSDictionary *damage = @{
                             @"count": @([[tempComponents objectAtIndex:0] integerValue]),
                             @"size": @([[tempComponents objectAtIndex:1] integerValue])};
    [content setObject:damage forKey:kMCDamageKey];
    /* damage-bonus */
    [content setObject:@([[data objectAtIndex:kMCDamageBonusIndex] integerValue]) forKey:kMCDamageBonusKey];
    /* critical-hit-visible */
    object = [data objectAtIndex:kMCCriticalHitVisibleIndex];
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
    [content setObject:criticalHitVisible forKey:kMCCriticalHitVisibleKey];
    /* critical-hit-invisible */
    object = [data objectAtIndex:kMCCriticalHitInvisibleIndex];
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
    [content setObject:criticalHitInvisible forKey:kMCCriticalHitInvisibleKey];
    /* critical-hit */
    [content setObject:@([[data objectAtIndex:kMCCriticalHitIndex] integerValue]) forKey:kMCCriticalHitKey];
    /* distance */
    [content setObject:@([[data objectAtIndex:kMCDistanceIndex] integerValue]) forKey:kMCDistanceKey];
    /* skills */
    NSString *skillA = [data objectAtIndex:kMCSkillAIndex];
    NSString *skillB = [data objectAtIndex:kMCSkillBIndex];
    NSString *skillC = [data objectAtIndex:kMCSkillCIndex];
    NSString *skillD = [data objectAtIndex:kMCSkillDIndex];
    NSDictionary *skills = @{@"A":[skillA compare:@"-"] == NSOrderedSame ? [NSNull null] : skillA,
                             @"B":[skillB compare:@"-"] == NSOrderedSame ? [NSNull null] : skillB,
                             @"C":[skillC compare:@"-"] == NSOrderedSame ? [NSNull null] : skillC,
                             @"D":[skillD compare:@"-"] == NSOrderedSame ? [NSNull null] : skillD};
    [content setObject:skills forKey:kMCSkillsKey];
    /* effect */
    id effect = [data objectAtIndex:kMCEffectIndex];
    if ([(NSString *) effect compare:@"-"] != NSOrderedSame) {
        [content setObject:effect forKey:kMCEffectKey];
        /* effect-check */
        object = [data objectAtIndex:kMCEffectCheckIndex];
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
        [content setObject:effectCheck forKey:kMCEffectCheckKey];
    } else {
        [content setObject:[NSNull null] forKey:kMCEffectKey];
        [content setObject:[NSNull null] forKey:kMCEffectCheckKey];
    }
    
    [self setObject:content forKey:ID];
}

@end
