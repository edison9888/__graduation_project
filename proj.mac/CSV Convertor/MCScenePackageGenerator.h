//
//  MCScenePackageGenerator.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-31.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MCTMXGenerator;

@interface MCScenePackageGenerator : NSObject
{
    NSString *scenesDirectory_;
    NSString *outputFilepath_;
    MCTMXGenerator *generator_;
}

@property (nonatomic, copy) NSString *scenesDirectory;
@property (nonatomic, copy) NSString *outputFilepath;
@property (nonatomic, assign) MCTMXGenerator *generator;

- (void)generate;

@end
