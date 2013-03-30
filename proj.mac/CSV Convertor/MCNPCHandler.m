//
//  MCNPCHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-30.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCNPCHandler.h"
#import "MCCSVHandlerMacros.h"

static const NSUInteger kMCIDIndex = 0;

MCDefineIndexAndKey(Name, 1, name);
MCDefineIndexAndKey(Face, 2, face);
MCDefineIndexAndKey(SpriteSheet, 3, sprite-sheet);
MCDefineIndexAndKey(DefaultDialogue, 4, default-dialogue);
MCDefineIndexAndKey(Description, 5, description);

@implementation MCNPCHandler

+ (NSString *)filename
{
    return @"N000.jpkg";
}

+ (NSString *)sourceFilename
{
    return @"游戏角色-NPC列表.csv";
}

- (void)handleLine:(NSString *)aLine
{
    if ([aLine rangeOfString:@"N"].location != 0
        || [aLine rangeOfString:@"00"].location == 2) {
        return;
    }
    NSArray *data = [aLine componentsSeparatedByString:@","];
    NSMutableDictionary *content = [[NSMutableDictionary alloc] initWithCapacity:24];
    
    /* ID */
    NSString *ID = [data objectAtIndex:0];
    
    /* name */
    [content setObject:[data objectAtIndex:kMCNameIndex] forKey:kMCNameKey];
    /* face */
    [content setObject:[data objectAtIndex:kMCFaceIndex] forKey:kMCFaceKey];
    /* sprite-sheet */
    [content setObject:[data objectAtIndex:kMCSpriteSheetIndex] forKey:kMCSpriteSheetKey];
    /* default-dialogue */
    [content setObject:[data objectAtIndex:kMCDefaultDialogueIndex] forKey:kMCDefaultDialogueKey];
    /* description */
    [content setObject:[data objectAtIndex:kMCDescriptionIndex] forKey:kMCDescriptionKey];
    
    [self setObject:content forKey:ID];
}

@end
