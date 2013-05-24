//
//  MCEnemyHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCEnemyHandler.h"
#import "MCCoreMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);
MCDefineIndexAndKey(SpriteSheet, 2, sprite-sheet);
MCDefineIndexAndKey(EffectID, 3, effect-id);
MCDefineIndexAndKey(RoleRace, 4, role-race);
MCDefineIndexAndKey(HP, 5, HP);
MCDefineIndexAndKey(PP, 6, PP);
MCDefineIndexAndKey(Consume, 7, consume);
MCDefineIndexAndKey(Exhaustion, 8, exhaustion);
MCDefineIndexAndKey(Tired, 9, tired);
MCDefineIndexAndKey(Dexterity, 10, dexterity);
MCDefineIndexAndKey(AC, 11, AC);
MCDefineIndexAndKey(ArmorCheckPenalty, 12, armor-check-penalty);
MCDefineIndexAndKey(Damage, 13, damage);
MCDefineIndexAndKey(DamageBonus, 16, damage-bonus);
MCDefineIndexAndKey(CriticalHitVisible, 17, critical-hit-visible);
MCDefineIndexAndKey(CriticalHitInvisible, 18, critical-hit-invisible);
MCDefineIndexAndKey(CriticalHit, 19, critical-hit);
MCDefineIndexAndKey(Distance, 20, distance);

static const NSUInteger kMCSkillAIndex = 21;
static const NSUInteger kMCSkillBIndex = 22;
static const NSUInteger kMCSkillCIndex = 23;
static const NSUInteger kMCSkillDIndex = 24;
static const NSString   *kMCSkillsKey = @"skills";

MCDefineIndexAndKey(Effect, 25, effect);
MCDefineIndexAndKey(EffectCheck, 26, effect-check);
MCDefineIndexAndKey(ActionEffect, 27, action-effect);

@implementation MCEnemyHandler

+ (NSString *)filename
{
    return @"E400.jpkg";
}

+ (NSString *)sourceFilename
{
    return @"游戏角色-敌人列表.csv";
}

- (void)handleLine:(NSString *)aLine
{
    if ([aLine rangeOfString:@"E"].location != 0
        || [aLine rangeOfString:@"00"].location == 2) {
        return;
    }
    NSArray *data = [aLine componentsSeparatedByString:@","];
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:16];
    id object;
    
    /* ID */
    NSString *ID = [data objectAtIndex:kMCIDIndex];
    
    /* name */
    [content setObject:[data objectAtIndex:kMCNameIndex] forKey:kMCNameKey];
    /* sprite-sheet */
    [content setObject:[data objectAtIndex:kMCSpriteSheetIndex] forKey:kMCSpriteSheetKey];
    /* effect-id */
    [content setObject:[data objectAtIndex:kMCEffectIDIndex] forKey:kMCEffectIDKey];
    /* AI */
    [content setObject:@([[data objectAtIndex:kMCRoleRaceIndex] integerValue]) forKey:kMCRoleRaceKey];
    /* HP */
    [content setObject:@([[data objectAtIndex:kMCHPIndex] integerValue]) forKey:kMCHPKey];
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
    NSString *effect = [data objectAtIndex:kMCEffectIndex];
    if ([effect compare:@"-"] != NSOrderedSame) {
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
    /* action-effect */
    [content setObject:[data objectAtIndex:kMCActionEffectIndex] forKey:kMCActionEffectKey];
    
    [self setObject:content forKey:ID];
}

@end
