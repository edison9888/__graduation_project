//
//  MCScenes.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-31.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCScenes.h"
#import "MCScenePackage.h"
#import "JSONKit.h"

@interface MCScenes ()
{
    NSMutableDictionary *scenePackages_; //场景包列表文件，是个字典。。。。
    NSMutableDictionary *scenes_; //场景
}

@end

@implementation MCScenes
@synthesize outputFilepath;

- (id)init
{
    self = [super init];
    if (self) {
        scenePackages_ = [[NSMutableDictionary alloc] init];
        scenes_ = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (void)dealloc
{
    [scenePackages_ release];
    [scenes_ release];
    [super dealloc];
}

- (void)writeScenePackage:(MCScenePackage *)aScenePackage
{
    [scenes_ setObject:aScenePackage forKey:[aScenePackage ID]];
    [scenePackages_ setObject:[[[[aScenePackage objectForKey:@"background"]
                               objectForKey:@"tmx"]
                               stringByDeletingPathExtension]
                               stringByAppendingPathExtension:@"jpkg"]
                       forKey:[aScenePackage ID]];
}

- (void)flush
{
    for (id key in scenes_) {
        MCScenePackage *scenePackage = [scenes_ objectForKey:key];
        NSString *data = [[scenePackage data] JSONString];
        [data writeToFile:[scenePackage outputFilepath]
               atomically:YES
                 encoding:NSUTF8StringEncoding
                    error:nil];
    }

    NSString *data = [scenePackages_ JSONString];
    [data writeToFile:self.outputFilepath
           atomically:YES
             encoding:NSUTF8StringEncoding
                error:nil];
}

@end
