//
//  MCScenePackage.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-31.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCScenePackage.h"

NSString *kMCScenePackageID = @"id";
NSString *kMCScenePackageInternal = @"internal";
NSString *kMCScenePackageSceneType = @"scene-type";

NSString *kMCScenePackageBackground = @"background";
NSString *kMCScenePackageTMXMap = @"tmx";
NSString *kMCScenePackageSound = @"sound";

NSString *kMCScenePackageObjects = @"objects";
NSString *kMCScenePackageObjectsID = @"id";
NSString *kMCScenePackageObjectsX = @"x";
NSString *kMCScenePackageObjectsY = @"y";
NSString *kMCScenePackageObjectsFlags = @"flags";
NSString *kMCScenePackageScenes = @"scenes";
NSString *kMCScenePackageScenesID = @"id";
NSString *kMCScenePackageScenesDestination = @"destination";

NSString *kMCScenePackageName = @"name";
NSString *kMCScenePackageDescription = @"description";

@interface MCScenePackage ()
{
    NSMutableDictionary *scenePackage_;
}

@end

@implementation MCScenePackage
@synthesize ID;
@synthesize outputFilepath;

- (id)init
{
    self = [super init];
    if (self) {
        scenePackage_ = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (void)dealloc
{
    [scenePackage_ release];
    [super dealloc];
}

- (id)objectForKey:(id)aKey
{
    return [scenePackage_ objectForKey:aKey];
}

- (void)setObject:(id)anObject forKey:(id)aKey
{
    [scenePackage_ setObject:anObject forKey:aKey];
}

- (NSDictionary *)data
{
    return scenePackage_;
}


@end
