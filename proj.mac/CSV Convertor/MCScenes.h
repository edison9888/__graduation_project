//
//  MCScenes.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-31.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MCScenePackage;

@interface MCScenes : NSObject
@property (nonatomic, copy) NSString *outputFilepath;

- (void)writeScenePackage:(MCScenePackage *)aScenePackage;

- (void)flush;

@end
