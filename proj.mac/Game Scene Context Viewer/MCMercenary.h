//
//  MCMercenary.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCEnemy.h"

@interface MCMercenary : MCEnemy

@property (nonatomic) NSUInteger dying; /* 生命垂危线 */
@property (nonatomic) NSUInteger cost; /* 雇佣所需费用 */

@end
