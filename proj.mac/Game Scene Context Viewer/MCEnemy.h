//
//  MCEnemy.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCRole.h"
#import "MCDice.h"

@interface MCEnemy : MCRole

@property (nonatomic) NSUInteger dexterity; /* 敏捷调整值 */
@property (nonatomic) NSUInteger ac; /* 防御等级 */
@property (nonatomic) NSUInteger armorCheckPenalty; /* 防具检定减值 */
@property (nonatomic) MCDice damage; /* 伤害值 */
@property (nonatomic) NSUInteger damageBonus; /* 伤害值加成 */
@property (nonatomic) MCDiceRange criticalHitVisible; /* 可视区域重击范围 */
@property (nonatomic) MCDiceRange criticalHitInvisible; /* 非可视区域重击范围 */
@property (nonatomic) CGFloat criticalHit; /* 重击倍数 */
@property (nonatomic) NSUInteger distance; /* 攻击距离 */

@property (nonatomic, copy) NSString *effect; /* 附带效果 */
@property (nonatomic) MCDiceRange effectCheck; /* 附带效果判定 */

@end
