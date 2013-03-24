//
//  main.m
//  CSV Convertor
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "MCCSVConvertor.h"
#import "MCMercenaryHandler.h"
#import "MCEnemyHandler.h"
#import "MCWeaponHandler.h"
#import "MCArmorHandler.h"
#import "MCItemHandler.h"

static NSString *root = @"/Users/DF/Desktop/游戏策划";

int main(int argc, const char * argv[])
{

    @autoreleasepool {
        MCCSVConvertor *convertor = [[MCCSVConvertor alloc] init];
        MCCSVHandler *handler;
        
        [convertor setCsvDirectory:root];
        
        /* 佣兵 */
        handler = [[MCMercenaryHandler alloc] init];
        handler.startLine = 3;
        handler.ignoreLine = 4;
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
    }
    return 0;
}

