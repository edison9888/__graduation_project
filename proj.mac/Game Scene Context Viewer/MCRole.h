//
//  MCRole.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MCRole : NSObject

@property (nonatomic, copy) NSString *ID;
@property (nonatomic, copy) NSString *name;

@property (nonatomic, copy) NSString *roleType; /* 角色类型 */
@property (nonatomic, copy) NSString *roleRace; /* 角色种族 */
@property (nonatomic) NSUInteger hp; /* 角色生命值 */
@property (nonatomic) NSUInteger pp; /* 角色体力值 */
@property (nonatomic) BOOL exhausted; /* 体力透支导致快挂了 */
@property (nonatomic) NSUInteger exhaustion; /* 体力透支线 */
@property (nonatomic) NSUInteger tired; /* 体力疲惫线 */
@property (nonatomic) NSUInteger maxHP; /* 角色满生命值 */
@property (nonatomic) NSUInteger maxPP; /* 角色满体力值 */
@property (nonatomic, copy) NSString *roleState; /* 角色状态 */
@property (nonatomic, copy) NSString *face; /* 角色头像 */
@property (nonatomic, copy) NSString *spriteSheet; /* 角色精灵表 */
@property (nonatomic, copy) NSString *defaultDialogue; /* 默认对白 */

@property (nonatomic, copy) NSString *AIState; /* AI状态 */

@property (nonatomic, copy) NSString *trigger; /* 点击活动键触发 */

+ (NSArray *)keys;
- (NSArray *)keys;

@end
