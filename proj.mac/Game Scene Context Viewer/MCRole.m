//
//  MCRole.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCRole.h"

@implementation MCRole
@synthesize ID;
@synthesize name;

@synthesize roleType; /* 角色类型 */
@synthesize roleRace; /* 角色种族 */
@synthesize hp; /* 角色生命值 */
@synthesize pp; /* 角色体力值 */
@synthesize exhausted; /* 体力透支导致快挂了 */
@synthesize exhaustion; /* 体力透支线 */
@synthesize tired; /* 体力疲惫线 */
@synthesize maxHP; /* 角色满生命值 */
@synthesize maxPP; /* 角色满体力值 */
@synthesize roleState; /* 角色状态 */
@synthesize face; /* 角色头像 */
@synthesize spriteSheet; /* 角色精灵表 */
@synthesize defaultDialogue; /* 默认对白 */

@synthesize AIState; /* AI状态 */

@synthesize trigger; /* 点击活动键触发 */

- (id)init
{
    self = [super init];
    if (self) {
    }
    return self;
}

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
             @[@"trigger",@"触发器脚本"]
             ];
}

- (NSArray *)keys
{
    return [MCRole keys];
}

@end
