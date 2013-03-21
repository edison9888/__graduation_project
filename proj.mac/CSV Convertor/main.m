//
//  main.m
//  CSV Convertor
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "MCCSVConvertor.h"
#import "MCEnemyHandler.h"

static NSString *test = @"/Users/DF/Desktop/游戏/e.csv";

int main(int argc, const char * argv[])
{

    @autoreleasepool {
        MCCSVConvertor *convertor = [[MCCSVConvertor alloc] init];
        MCEnemyHandler *handler = [[MCEnemyHandler alloc] init];
        
        [convertor setCsvFilepath:test];
        handler.startLine = 3;
        handler.ignoreLine = 87;
        [convertor setHandler:handler];
        [convertor convert];
    }
    return 0;
}

