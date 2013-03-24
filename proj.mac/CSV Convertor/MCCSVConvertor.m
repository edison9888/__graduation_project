//
//  MCCSVConvertor.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCCSVConvertor.h"
#import "MCCSVHandler.h"

@implementation MCCSVConvertor
@synthesize csvDirectory=csvDirectory_;
@synthesize handler=handler_;
@synthesize outputFilepath=outputFilepath_;

- (id)init
{
    self = [super init];
    if (self) {
        csvDirectory_ = nil;
        handler_ = nil;
        outputFilepath_ = nil;
    }
    
    return self;
}

- (void)convert
{
    assert(csvDirectory_ != nil && handler_ != nil);
    
    NSString *sourceFilepath = [csvDirectory_ stringByAppendingPathComponent:[[handler_ class] sourceFilename]];
    NSString *csv = [NSString stringWithContentsOfFile:sourceFilepath
                                              encoding:NSUTF8StringEncoding
                                                 error:nil];
    NSArray *csvContent = [csv componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
    NSString *currentContent;
    NSString  *outputFilepath;
    NSInteger index;
    NSUInteger startIndex = [handler_ startLine] * 2;
    NSUInteger ignore = [handler_ ignoreLine] * 2;
    NSUInteger endIndex = [csvContent count] - ignore;
    
    for (index = startIndex; index < endIndex; ++index) {
        currentContent = [csvContent objectAtIndex:index];
        [handler_ handleLine:currentContent];
    }
    
    if (outputFilepath_) {
        outputFilepath = outputFilepath_;
    } else {
        outputFilepath = [NSString stringWithFormat:@"%@.json", sourceFilepath];
    }
    
    NSString *filename = [[handler_ class] filename];
    if (filename != nil) {
        outputFilepath = [[outputFilepath stringByDeletingLastPathComponent]
                            stringByAppendingPathComponent:filename];
    }
    
    [handler_ writeTo:outputFilepath];
}

@end
