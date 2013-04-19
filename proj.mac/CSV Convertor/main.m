//
//  main.m
//  CSV Convertor
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "MCCSVConvertor.h"
#import "MCNPCHandler.h"
#import "MCMercenaryHandler.h"
#import "MCEnemyHandler.h"
#import "MCWeaponHandler.h"
#import "MCArmorHandler.h"
#import "MCItemHandler.h"
#import "MCFlagHandler.h"
#import "MCTaskHandler.h"
#import "MCRegionHandler.h"

#import "MCScenePackageGenerator.h"
#import "MCTMXGenerator.h"

static NSString *root = @"/Users/DF/Desktop/游戏策划";
static NSString *kMCScenesRoot = @"/Users/DF/Projects/XCode/Cocos2d-X/__graduation_project/Resources/iphone/scenes";

const BOOL do_csv_convert = NO;
const BOOL do_scenes_generation = YES;

int main(int argc, const char * argv[])
{
    @autoreleasepool {
        
        if (do_csv_convert) {
            MCCSVConvertor *convertor = [[MCCSVConvertor alloc] init];
            MCCSVHandler *handler;
            
            [convertor setCsvDirectory:root];
            
            /* NPC */
            handler = [[MCNPCHandler alloc] init];
            handler.startLine = 1;
            handler.ignoreLine = 0;
            [convertor setHandler:handler];
            [convertor convert];
            [handler release];
            
            /* 佣兵 */
            handler = [[MCMercenaryHandler alloc] init];
            handler.startLine = 3;
            handler.ignoreLine = 5;
            [convertor setHandler:handler];
            [convertor convert];
            [handler release];
            
            /* 敌人 */
            handler = [[MCEnemyHandler alloc] init];
            handler.startLine = 3;
            handler.ignoreLine = 4;
            [convertor setHandler:handler];
            [convertor convert];
            [handler release];
            
            /* 武器 */
            handler = [[MCWeaponHandler alloc] init];
            handler.startLine = 3;
            handler.ignoreLine = 4;
            [convertor setHandler:handler];
            [convertor convert];
            [handler release];
            
            /* 防具 */
            handler = [[MCArmorHandler alloc] init];
            handler.startLine = 1;
            handler.ignoreLine = 0;
            [convertor setHandler:handler];
            [convertor convert];
            [handler release];
            
            /* 道具 */
            handler = [[MCItemHandler alloc] init];
            handler.startLine = 3;
            handler.ignoreLine = 0;
            [convertor setHandler:handler];
            [convertor convert];
            [handler release];
            
            /* 标志 */
            handler = [[MCFlagHandler alloc] init];
            handler.startLine = 1;
            handler.ignoreLine = 0;
            [convertor setHandler:handler];
            [convertor convert];
            [handler release];
            
            /* 任务 */
            handler = [[MCTaskHandler alloc] init];
            handler.startLine = 2;
            handler.ignoreLine = 0;
            [convertor setHandler:handler];
            [convertor convert];
            [handler release];
            
            /* 区域 */
            handler = [[MCRegionHandler alloc] init];
            handler.startLine = 1;
            handler.ignoreLine = 0;
            [convertor setHandler:handler];
            [convertor convert];
            [handler release];
            
            [convertor release];
        }
        
        if (do_scenes_generation) {
            MCScenePackageGenerator *generator = [[MCScenePackageGenerator alloc] init];
            MCTMXGenerator *tmxGenerator;
            
            [generator setScenesDirectory:kMCScenesRoot];
            
            tmxGenerator = [[MCTMXGenerator alloc] init];
            [generator setGenerator:tmxGenerator];
            [generator generate];
            [tmxGenerator release];
            
            [generator release];
        }
    }
    return 0;
}

