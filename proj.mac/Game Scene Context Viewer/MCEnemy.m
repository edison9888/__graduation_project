//
//  MCEnemy.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCEnemy.h"

@implementation MCEnemy

@synthesize dexterity; /* 敏捷调整值 */
@synthesize ac; /* 防御等级 */
@synthesize armorCheckPenalty; /* 防具检定减值 */
@synthesize damage; /* 伤害值 */
@synthesize damageBonus; /* 伤害值加成 */
@synthesize criticalHitVisible; /* 可视区域重击范围 */
@synthesize criticalHitInvisible; /* 非可视区域重击范围 */
@synthesize criticalHit; /* 重击倍数 */
@synthesize distance; /* 攻击距离 */

@synthesize effect; /* 附带效果 */
@synthesize effectCheck; /* 附带效果判定 */

- (id)valueForKey:(NSString *)key
{
    if ([key isEqualToString:@"damage"]) {
        return [NSString stringWithFormat:@"%luD%lu", self.damage.count, self.damage.size];
    } else if ([key isEqualToString:@"criticalHitVisible"]) {
        if (self.criticalHitVisible.min == self.criticalHitVisible.max) {
            return @(self.criticalHitVisible.max);
        } else {
            return [NSString stringWithFormat:@"%lu-%lu",
                    self.criticalHitVisible.min,
                    self.criticalHitVisible.max];
        }
    } else if ([key isEqualToString:@"criticalHitInvisible"]) {
        if (self.criticalHitInvisible.min == self.criticalHitInvisible.max) {
            return @(self.criticalHitInvisible.max);
        } else {
            return [NSString stringWithFormat:@"%lu-%lu",
                    self.criticalHitInvisible.min,
                    self.criticalHitInvisible.max];
        }
    } else if ([key isEqualToString:@"effectCheck"]) {
        if (self.effectCheck.min == self.effectCheck.max) {
            return @(self.effectCheck.max);
        } else {
            [NSString stringWithFormat:@"%lu-%lu",
             self.effectCheck.min,
             self.effectCheck.max];
        }
    }
    
    return [super valueForKey:key];
}

+ (NSArray *)keys
{
    return  @[
              @[@"ID",@"ID"],
              @[@"name",@"名称"],
              @[@"roleType",@"角色类型"],
              @[@"roleRace",@"角色种族"],
              @[@"hp",@"当前生命值"],
              @[@"pp",@"当前体力值"],
              @[@"exhausted",@"是否体力透支"],
              @[@"exhaustion",@"体力透支阀值"],
              @[@"tired",@"体力疲惫阀值"],
              @[@"maxHP",@"最大生命值"],
              @[@"maxPP",@"最大体力值"],
              @[@"roleState",@"角色状态"],
              @[@"face",@"头像文件"],
              @[@"spriteSheet",@"精灵表文件"],
              @[@"defaultDialogue",@"默认对话"],
              @[@"AIState",@"AI状态"],
              @[@"trigger",@"触发器脚本"],
              
              @[@"dexterity",@"敏捷调整值"],
              @[@"ac",@"防御等级"],
              @[@"armorCheckPenalty",@"防具检定减值"],
              @[@"damage",@"伤害值"],
              @[@"damageBonus",@"伤害值加成"],
              @[@"criticalHitVisible",@"可视区域重击范围"],
              @[@"criticalHitInvisible",@"非可视区域重击范围"],
              @[@"criticalHit",@"重击倍数"],
              @[@"distance",@"攻击距离"],
              @[@"effect",@"附带效果"],
              @[@"effectCheck",@"附带效果判定"]
              ];
}

- (NSArray *)keys
{
    return [MCEnemy keys];
}

@end
