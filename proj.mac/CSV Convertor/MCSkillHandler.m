//
//  MCSkillHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-29.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCSkillHandler.h"
#import "MCCoreMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);
MCDefineIndexAndKey(Icon, 2, icon);
MCDefineIndexAndKey(EffectID, 3, effect-id);
MCDefineIndexAndKey(ReleaseTime, 4, release-time);
MCDefineIndexAndKey(ActiveTime, 5, active-time);
MCDefineIndexAndKey(ColdTime, 6, cold-time);
MCDefineIndexAndKey(Distance, 7, distance);
MCDefineIndexAndKey(Length, 8, length);
MCDefineIndexAndKey(Breadth, 9, breadth);
MCDefineIndexAndKey(Consume, 10, consume);
MCDefineIndexAndKey(RequiredProficiency, 11, required-proficiency);
MCDefineIndexAndKey(Revise, 12, revise);
MCDefineIndexAndKey(Count, 13, count);
MCDefineIndexAndKey(HP, 14, hp);
MCDefineIndexAndKey(PP, 15, pp);
MCDefineIndexAndKey(DamageBonus, 16, damage-bonus);
MCDefineIndexAndKey(ArmorCheckPenalty, 17, armor-check-penalty);
MCDefineIndexAndKey(Special, 18, special);
MCDefineIndexAndKey(Effect, 20, effect);
MCDefineIndexAndKey(EffectCheck, 21, effect-check);
MCDefineIndexAndKey(TriggerType, 22, trigger-type);

@implementation MCSkillHandler

+ (NSString *)filename
{
    return @"SXXX.jpkg";
}

+ (NSString *)sourceFilename
{
    return @"技能系统-技能系统.csv";
}

/*
 字段                  |  字段说明       | 解释
 ---------------------|----------------|---------
 ID                   | 技能ID         | 技能ID为技能字典中技能的key
 name                 | 技能名称        | 技能的名称
 icon                 | 技能图标        | 技能的图标，敌方技能没有图标
 effect-id            | 效果ID         | 效果ID为效果字典中效果的key
 release-time         | 释放所需时间     | 从启动释放到释放完成所需要的时间(此时间完成后才开始技能判定)
 active-time          | 完成所需时间     | 从释放到消失所经过的时间(此时间完成后效果消失)
 cold-time            | 冷却时间        | 到下一次释放技能所需要的等待时间
 distance             | 攻击距离        | 释放中心点到消失位置的距离
 length               | 攻击范围长      | OBB的高
 breadth              | 攻击范围宽      | OBB的宽
 consume              | 体力消耗        | 释放所需要消耗的体力
 required-proficiency | 熟练度          | 使用该技能所需要的熟练度
 revise               | 效果(伤害修正)   | 对对方产生的伤害的倍数
 count                | 连续释放次数     | 技能释放次数
 hp                   | HP影响         | 佣兵被动技能所有，对整个队伍的人物的HP的加成
 pp                   | PP影响         | 佣兵被动技能所有，对整个队伍的人物的PP的加成
 damage-bonus         | 伤害值加成      | 佣兵被动技能所有，对整个队伍的人物的伤害值的加成
 armor-check-penalty  | 防具检定减值加成 | 佣兵被动技能所有，对整个队伍的人物的防具检定减值的加成
 special              | 特殊效果        | 佣兵被动技能所有，具体效果查看特殊效果字典
 effect               | 附带效果        | 状态攻击
 effect-check         | 附带效果判定     | 状态攻击的判断范围
 trigger-type         | 效果触发类型     | 值为0或者1，0表示被动技能，1表示主动技能
 */
- (void)handleLine:(NSString *)aLine
{
    NSArray *data = [aLine componentsSeparatedByString:@","];
    if ([data count] != 23 || [[data objectAtIndex:2] length] < 1) {
        return;
    }
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:7];
    id object;
    
    /* ID */
    NSString *ID = [data objectAtIndex:0];
    
    /* name */
    [content setObject:[data objectAtIndex:kMCNameIndex] forKey:kMCNameKey];
    /* icon */
    [content setObject:[data objectAtIndex:kMCIconIndex] forKey:kMCIconKey];
    /* effect-id */
    object = [data objectAtIndex:kMCEffectIDIndex];
    if ([(NSString *) object compare:@"-"] != NSOrderedSame) {
        [content setObject:object forKey:kMCEffectIDKey];
    } else {
        [content setObject:[NSNull null] forKey:kMCEffectIDKey];
    }
    /* release-time */
    [content setObject:@([[data objectAtIndex:kMCReleaseTimeIndex] floatValue]) forKey:kMCReleaseTimeKey];
    /* active-time */
    [content setObject:@([[data objectAtIndex:kMCActiveTimeIndex] floatValue]) forKey:kMCActiveTimeKey];
    /* cold-time */
    [content setObject:@([[data objectAtIndex:kMCColdTimeIndex] floatValue]) forKey:kMCColdTimeKey];
    /* distance */
    [content setObject:@([[data objectAtIndex:kMCDistanceIndex] integerValue]) forKey:kMCDistanceKey];
    /* length */
    [content setObject:@([[data objectAtIndex:kMCLengthIndex] integerValue]) forKey:kMCLengthKey];
    /* breadth */
    [content setObject:@([[data objectAtIndex:kMCBreadthIndex] integerValue]) forKey:kMCBreadthKey];
    /* consume */
    [content setObject:@([[data objectAtIndex:kMCConsumeIndex] integerValue]) forKey:kMCConsumeKey];
    /* required-proficiency */
    [content setObject:@([[data objectAtIndex:kMCRequiredProficiencyIndex] integerValue]) forKey:kMCRequiredProficiencyKey];
    /* revise */
    [content setObject:@([[data objectAtIndex:kMCReviseIndex] floatValue]) forKey:kMCReviseKey];
    /* count */
    [content setObject:@([[data objectAtIndex:kMCCountIndex] integerValue]) forKey:kMCCountKey];
    /* hp */
    [content setObject:@([[data objectAtIndex:kMCHPIndex] integerValue]) forKey:kMCHPKey];
    /* pp */
    [content setObject:@([[data objectAtIndex:kMCPPIndex] integerValue]) forKey:kMCPPKey];
    /* damage-bonus */
    [content setObject:@([[data objectAtIndex:kMCDamageBonusIndex] integerValue]) forKey:kMCDamageBonusKey];
    /* armor-check-penalty */
    [content setObject:@([[data objectAtIndex:kMCArmorCheckPenaltyIndex] integerValue]) forKey:kMCArmorCheckPenaltyKey];
    /* special */
    [content setObject:@([[data objectAtIndex:kMCSpecialIndex] integerValue]) forKey:kMCSpecialKey];
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
            NSArray *tempComponents = [object componentsSeparatedByString:@"-"];
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
    /* trigger-type */
    [content setObject:@([[data objectAtIndex:kMCTriggerTypeIndex] integerValue]) forKey:kMCTriggerTypeKey];
    
    [self setObject:content forKey:ID];
}

@end
