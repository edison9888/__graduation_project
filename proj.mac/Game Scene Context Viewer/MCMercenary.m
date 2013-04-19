//
//  MCMercenary.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCMercenary.h"

@implementation MCMercenary
@synthesize dying; /* 生命垂危线 */
@synthesize cost; /* 雇佣所需费用 */

+ (NSArray *)keys
{
    return @[
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
             @[@"effectCheck",@"附带效果判定"],
             
             @[@"dying",@"生命垂危阀值"],
             @[@"cost",@"雇佣所需费用"]
             ];
}

- (NSArray *)keys
{
    return [MCMercenary keys];
}

@end
