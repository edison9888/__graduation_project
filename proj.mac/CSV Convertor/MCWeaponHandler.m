//
//  MCWeaponHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCWeaponHandler.h"
#import "MCCoreMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);
MCDefineIndexAndKey(Icon, 2, icon);
MCDefineIndexAndKey(Price, 3, price);
MCDefineIndexAndKey(Damage, 4, damage);
MCDefineIndexAndKey(CriticalHitVisible, 7, critical-hit-visible);
MCDefineIndexAndKey(CriticalHitInvisible, 8, critical-hit-invisible);
MCDefineIndexAndKey(CriticalHit, 9, critical-hit);
MCDefineIndexAndKey(Distance, 12, distance);
MCDefineIndexAndKey(Effect, 13, effect);
MCDefineIndexAndKey(EffectCheck, 14, effect-check);
MCDefineIndexAndKey(Dexterity, 15, dexterity);

@implementation MCWeaponHandler

+ (NSString *)filename
{
    return @"E001.jpkg";
}

+ (NSString *)sourceFilename
{
    return @"游戏道具-武器系统.csv";
}

/*
 字段                    |  字段说明         | 解释
 ------------------------|------------------|---------
 ID                     | 武器ID            | 武器ID为道具字典中武器的key
 name                   | 武器名称           | 武器的名称
 icon                   | 武器图标           | 武器图标的路径，需要后缀名
 price                  | 升级基础价格        | 升级一次所需的基础金钱
 damage                 | 伤害值             | 骰子对象，有count和size两个属性
 critical-hit-visible   | 可视区域内重击范围   | D20的骰子的范围，有min、max，dice三个属性，dice为对象有count和size两个属性
 critical-hit-invisible | 非可视区域内重击范围 | D20的骰子的范围，有min、max，dice三个属性，dice为对象有count和size两个属性
 critical-hit           | 重击倍数           | 重击时伤害值乘以这个值得出重击伤害值
 distance               | 攻击距离           | 只能在此距离内攻击
 effect                 | 武器附带效果        | 只有锤子有击晕效果，其他武器为null值
 effect-check           | 武器附带效果判定范围 | 只有锤子有击晕效果判定，有min、max，dice三个属性，dice为对象有count和size两个属性，其他武器为null值
 dexterity              | 敏捷调整值         | 参与命中判定的值
 */
- (void)handleLine:(NSString *)aLine
{
    if ([aLine rangeOfString:@"W"].location != 0
        || [aLine rangeOfString:@"00"].location == 2) {
        return;
    }
    NSArray *data = [aLine componentsSeparatedByString:@","];
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:17];
    id object;
    NSArray *tempComponents;
    
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
    /* damage */
    tempComponents = [[data objectAtIndex:kMCDamageIndex] componentsSeparatedByString:@"d"];
    NSDictionary *damage = @{
                             @"count": @([[tempComponents objectAtIndex:0] integerValue]),
                             @"size": @([[tempComponents objectAtIndex:1] integerValue])};
    [content setObject:damage forKey:kMCDamageKey];
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
    /* dexterity */
    [content setObject:@([[data objectAtIndex:kMCDexterityIndex] integerValue]) forKey:kMCDexterityKey];
    
    [self setObject:content forKey:ID];
}

@end
