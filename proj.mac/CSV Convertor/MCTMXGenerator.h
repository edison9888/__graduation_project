//
//  MCTMXGenerator.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-31.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MCScenePackage;

enum {
    MCUnknownPackage            = 0,    /* 未知场景包类型，直接终结游戏 */
    MCGameScenePackage          = 1,    /* 对应MCGameScene */
    MCBattleFieldScenePackage   = 2     /* 对应MCBattleFieldScene */
};
typedef NSUInteger MCScenePackageType;

@interface MCTMXGenerator : NSObject

- (MCScenePackage *)generate:(NSString *)aTMXMapFilepath;

@end
