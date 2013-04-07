//
//  MCScenePackage.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-31.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString *kMCScenePackageID;
extern NSString *kMCScenePackageInternal;
extern NSString *kMCScenePackageSceneType;
extern NSString *kMCScenePackageBackground;
extern NSString *kMCScenePackageTMXMap;
extern NSString *kMCScenePackageSound;
extern NSString *kMCScenePackageObjects;
extern NSString *kMCScenePackageObjectsX;
extern NSString *kMCScenePackageObjectsY;
extern NSString *kMCScenePackageObjectsFlags;
extern NSString *kMCScenePackageScenes;
extern NSString *kMCScenePackageScenesID;
extern NSString *kMCScenePackageScenesDestination;
extern NSString *kMCScenePackageName;
extern NSString *kMCScenePackageDescription;


@interface MCScenePackage : NSObject<NSXMLParserDelegate>

@property (nonatomic, copy) NSString *ID;
@property (nonatomic, copy) NSString *outputFilepath;

- (id)objectForKey:(id)aKey;
- (void)setObject:(id)anObject forKey:(id)aKey;

- (NSDictionary *)data;

@end
