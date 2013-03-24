//
//  MCCSVHandler.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MCCSVHandler : NSObject

@property (nonatomic, assign) NSUInteger startLine;  /* 数据从第几行开始 */
@property (nonatomic, assign) NSUInteger ignoreLine; /* 忽略最后几行 */

+ (NSString *)filename;
+ (NSString *)sourceFilename;

- (void)handleLine:(NSString *)aLine;

- (void)setObject:(NSObject *)anObject forKey:(NSString *)aKey;

- (void)writeTo:(NSString *)anOutputFilepath;

@end
