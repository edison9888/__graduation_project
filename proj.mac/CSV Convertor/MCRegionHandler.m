//
//  MCRegionHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCRegionHandler.h"
#import "MCCoreMacros.h"

static const NSUInteger kMCIDIndex = 0;
static const NSUInteger kMCSubRegionIDIndex = 1;

MCDefineIndexAndKey(Region, 2, region);
MCDefineIndexAndKey(Name, 3, name);

static const NSString *kMCSubRegionsKey = @"sub-regions";

@interface MCRegionHandler ()
{
    NSMutableArray *currentRegion_;
    NSString       *currentRegionID_;
    NSNumber       *region_; /* 区域对应的枚举值 */
    NSString       *name_;
}

@end

@implementation MCRegionHandler

- (id)init
{
    self = [super init];
    if (self) {
        currentRegion_ = nil;
        currentRegionID_ = nil;
    }
    return self;
}

+ (NSString *)filename
{
    return @"R000.jpkg";
}

+ (NSString *)sourceFilename
{
    return @"游戏区域-游戏区域.csv";
}

- (void)handleLine:(NSString *)aLine
{
    if ([aLine length] < 1) {
        return;
    }

    NSArray *data = [aLine componentsSeparatedByString:@","];
    
    /* ID */
    NSString *ID = [data objectAtIndex:kMCIDIndex];
    
    if ([ID compare:@"-"] == NSOrderedSame) { /* sub-region */
        [currentRegion_ addObject:[data objectAtIndex:kMCSubRegionIDIndex]];
    } else {
        if (currentRegion_ == nil) {
            /* 木有地区 */
            currentRegion_ = [[NSMutableArray alloc] init];
        } else { /* 写入上一个 */
            NSDictionary *region = @{kMCRegionKey:region_,
                                     kMCSubRegionsKey:currentRegion_,
                                     kMCNameKey:name_};
            [self setObject:region forKey:currentRegionID_];
            [currentRegion_ release];
            currentRegion_ = [[NSMutableArray alloc] init];
        }
        currentRegionID_ = [NSString stringWithString:ID];
        region_ = @([[data objectAtIndex:kMCRegionIndex] integerValue]);
        name_ = [data objectAtIndex:kMCNameIndex];
    }
}

- (void)didHandle
{
    NSDictionary *region = @{kMCRegionKey:region_,
                             kMCSubRegionsKey:currentRegion_,
                             kMCNameKey:name_};
    [self setObject:region forKey:currentRegionID_];
}

@end
