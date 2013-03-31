//
//  MCScenePackageGenerator.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-31.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCScenePackageGenerator.h"
#import "MCScenes.h"
#import "MCTMXGenerator.h"
#import "MCScenePackage.h"

static NSString *kMCScenePackageFilename = @"S000.jpkg";

@interface NSString (MCStringCompare)

- (BOOL)startswith:(NSString *)aString;
- (BOOL)endswith:(NSString *)aString;

@end

@implementation NSString (MCStringCompare)

- (BOOL)startswith:(NSString *)aString
{
    NSRange range = [self rangeOfString:aString];
    
    return range.location == 0;
}

- (BOOL)endswith:(NSString *)aString
{
    NSRange range = [self rangeOfString:aString];
    
    return range.location == [self length] - [aString length];
}

@end

@implementation MCScenePackageGenerator
@synthesize scenesDirectory=scenesDirectory_;
@synthesize generator=generator_;
@synthesize outputFilepath=outputFilepath_;

- (id)init
{
    self = [super init];
    if (self) {
        scenesDirectory_ = nil;
        generator_ = nil;
        outputFilepath_ = nil;
    }
    
    return self;
}

- (void)generate
{
    assert(scenesDirectory_ != nil && generator_ != nil);
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString *scenePackageFilepath = [[scenesDirectory_ stringByDeletingLastPathComponent]
                                      stringByAppendingPathComponent:kMCScenePackageFilename];
    NSArray *files = [fileManager contentsOfDirectoryAtPath:scenesDirectory_ error:nil];
    NSMutableArray *tmxFiles = [[NSMutableArray alloc] init];
    MCScenes *scenes = [[MCScenes alloc] init];
    scenes.outputFilepath = scenePackageFilepath;

    for (NSString *filename in files) {
        if ([filename endswith:@".tmx"]) {
            [tmxFiles addObject:[scenesDirectory_ stringByAppendingPathComponent:filename]];
        }
    }
    
    for (NSString *tmxFile in tmxFiles) {
        MCScenePackage *scenePackage = [generator_ generate:tmxFile];
        
        [scenes writeScenePackage:scenePackage];
        
        [scenePackage release];
    }
    
    [scenes flush];
    [scenes release];
    
    [tmxFiles release];
}

@end
