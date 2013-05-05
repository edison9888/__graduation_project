//
//  MCMercenaryHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCMercenaryHandler.h"
#import "MCCoreMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);
MCDefineIndexAndKey(Face, 2, face);
MCDefineIndexAndKey(SpriteSheet, 3, sprite-sheet);
MCDefineIndexAndKey(EffectID, 4, effect-id);
MCDefineIndexAndKey(AI, 5, AI);
MCDefineIndexAndKey(Cost, 6, cost);
MCDefineIndexAndKey(HP, 7, HP);
MCDefineIndexAndKey(Dying, 8, dying);
MCDefineIndexAndKey(PP, 9, PP);
MCDefineIndexAndKey(Consume, 10, consume);
MCDefineIndexAndKey(Exhaustion, 11, exhaustion);
MCDefineIndexAndKey(Tired, 12, tired);
MCDefineIndexAndKey(Dexterity, 13, dexterity);
MCDefineIndexAndKey(AC, 14, AC);
MCDefineIndexAndKey(ArmorCheckPenalty, 15, armor-check-penalty);
MCDefineIndexAndKey(Damage, 16, damage);
MCDefineIndexAndKey(DamageBonus, 19, damage-bonus);
MCDefineIndexAndKey(CriticalHitVisible, 20, critical-hit-visible);
MCDefineIndexAndKey(CriticalHitInvisible, 21, critical-hit-invisible);
MCDefineIndexAndKey(CriticalHit, 22, critical-hit);
MCDefineIndexAndKey(Distance, 23, distance);

static const NSUInteger kMCSkillAIndex = 24;
static const NSUInteger kMCSkillBIndex = 25;
static const NSUInteger kMCSkillCIndex = 26;
static const NSUInteger kMCSkillDIndex = 27;
static const NSString   *kMCSkillsKey = @"skills";

MCDefineIndexAndKey(Effect, 28, effect);
MCDefineIndexAndKey(EffectCheck, 29, effect-check);

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
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:28];
    id object;
    
    /* ID */
    NSString *ID = [data objectAtIndex:0];
    
    /* name */
    [content setObject:[data objectAtIndex:kMCNameIndex] forKey:kMCNameKey];
    /* face */
    [content setObject:[data objectAtIndex:kMCFaceIndex] forKey:kMCFaceKey];
    /* sprite-sheet */
    [content setObject:[data objectAtIndex:kMCSpriteSheetIndex] forKey:kMCSpriteSheetKey];
    /* effect-id */
    [content setObject:[data objectAtIndex:kMCEffectIDIndex] forKey:kMCEffectIDKey];
    /* AI */
    [content setObject:[data objectAtIndex:kMCAIIndex] forKey:kMCAIKey];
    /* cost */
    [content setObject:@([[data objectAtIndex:kMCCostIndex] integerValue]) forKey:kMCCostKey];
    /* HP */
    [content setObject:@([[data objectAtIndex:kMCHPIndex] integerValue]) forKey:kMCHPKey];
    /* dying */
    [content setObject:@([[data objectAtIndex:kMCDyingIndex] integerValue]) forKey:kMCDyingKey];
    /* PP */
    [content setObject:@([[data objectAtIndex:kMCPPIndex] integerValue]) forKey:kMCPPKey];
    /* consume */
    [content setObject:@([[data objectAtIndex:kMCConsumeIndex] floatValue]) forKey:kMCConsumeKey];
    /* exhaustion */
    [content setObject:@([[data objectAtIndex:kMCExhaustionIndex] integerValue]) forKey:kMCExhaustionKey];
    /* tired */
    [content setObject:@([[data objectAtIndex:kMCTiredIndex] integerValue]) forKey:kMCTiredKey];
    /* dexterity */
    [content setObject:@([[data objectAtIndex:kMCDexterityIndex] integerValue]) forKey:kMCDexterityKey];
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
