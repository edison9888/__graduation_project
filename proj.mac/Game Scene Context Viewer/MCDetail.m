//
//  MCDetail.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCDetail.h"
#import "MCGameSceneContextMonitor.h"
#import "MCMercenary.h"

static NSString * const kMCDetailKey = @"kMCDetailKey";
static NSString * const kMCDetailValue = @"kMCDetailValue";

@implementation MCDetail

- (id)init
{
    self = [super init];
    if (self) {
        _selectedRow = -1;
    }
    return self;
}

- (void)setSelectedRow:(NSInteger)selectedRow
{
    _selectedRow = selectedRow;
    if (selectedRow) {
        [_window setTitle:[[_gameSceneContextViewer.objects objectAtIndex:selectedRow] name]];
    } else {
        [_window setTitle:@""];
    }
    [_tableView reloadData];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    return _selectedRow != -1 ? [[[_gameSceneContextViewer.objects objectAtIndex:_selectedRow] keys] count] : 0;
    
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    NSString *identifier = [tableColumn identifier];
    MCRole *role = [_gameSceneContextViewer.objects objectAtIndex:_selectedRow];
    NSArray *keys = [[role class] keys];
    
    if ([identifier isEqualToString:kMCDetailKey]) {
        return [[keys objectAtIndex:row] objectAtIndex:1];
    } else if ([identifier isEqualToString:kMCDetailValue]) {
        return [role valueForKey:[[keys objectAtIndex:row] objectAtIndex:0]];
    }
    
    return nil;
}

@end
