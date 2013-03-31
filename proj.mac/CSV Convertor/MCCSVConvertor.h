//
//  MCCSVConvertor.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MCCSVHandler;

@interface MCCSVConvertor : NSObject
{
    NSString *csvDirectory_;
    NSString *outputFilepath_;
    MCCSVHandler *handler_;
}

@property (nonatomic, copy) NSString *csvDirectory;
@property (nonatomic, copy) NSString *outputFilepath;
@property (nonatomic, assign) MCCSVHandler *handler;

- (void)convert;

@end
