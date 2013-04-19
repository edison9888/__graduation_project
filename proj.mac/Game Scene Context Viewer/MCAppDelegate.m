//
//  MCAppDelegate.m
//  Game Scene Context Viewer
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCAppDelegate.h"
#import "MCGameSceneContextMonitor.h"
#import "MCDetail.h"
#import "MCEntrance.h"
#import "MCRole.h"

NSString * const kMCSceneEntrances = @"kMCSceneEntrances";
NSString * const kMCSceneObjects = @"kMCSceneObjects";
NSString * const kMCSceneObjectID = @"kMCSceneObjectID";
NSString * const kMCSceneObjectName = @"kMCSceneObjectName";

@implementation MCAppDelegate

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    NSNotificationCenter *notificationCenter = [NSNotificationCenter defaultCenter];
    [notificationCenter addObserver:self
                           selector:@selector(dataDidRefresh:)
                               name:kMCDataDidRefreshNotification
                             object:nil];
    [_gameSceneContextViewer listen];
    
    [NSTimer scheduledTimerWithTimeInterval:6.0f
                                     target:self
                                   selector:@selector(refresh:)
                                   userInfo:nil
                                    repeats:YES];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
    NSNotificationCenter *notificationCenter = [NSNotificationCenter defaultCenter];
    [notificationCenter removeObserver:_gameSceneContextViewer];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    NSString *identifier = [tableView identifier];
    
    if ([identifier isEqualToString:kMCSceneEntrances]) {
        return [_gameSceneContextViewer.entrances count];
    } else if ([identifier isEqualToString:kMCSceneObjects]) {
        return [_gameSceneContextViewer.objects count];
    }
    
    return 0;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    NSString *identifier = [tableView identifier];
    NSString *columnIdentifier;
    
    if ([identifier isEqualToString:kMCSceneEntrances]) {
        MCEntrance *entrance = [_gameSceneContextViewer.entrances objectAtIndex:row];
        return entrance.name;
    } else if ([identifier isEqualToString:kMCSceneObjects]) {
        columnIdentifier = [tableColumn identifier];
        MCRole *role = [_gameSceneContextViewer.objects objectAtIndex:row];
        if ([columnIdentifier isEqualToString:kMCSceneObjectID]) {
            return role.ID;
        } else if ([columnIdentifier isEqualToString:kMCSceneObjectName]) {
            return role.name;
        }
    }
    
    return nil;
}

- (IBAction)refresh:(id)sender {
    [_progressIndicator startAnimation:self];
    [_progressIndicator setHidden:NO];
    [_gameSceneContextViewer refresh];
}

- (BOOL)tableView:(NSTableView *)tableView shouldSelectRow:(NSInteger)row
{
    return [[tableView identifier] isEqualToString:kMCSceneObjects];
}

- (void)tableViewSelectionDidChange:(NSNotification *)notification
{
    [self showDetail:self];
}

- (IBAction)showDetail:(id)sender
{
    NSInteger row = [_objects selectedRow];
    if (row == -1) {
        return;
    }

    _detail.selectedRow = row;
    [_detail.window orderFront:self];
}

- (void)dataDidRefresh:(id)anObject {
    if (! _gameSceneContextViewer.sceneID) {
        return;
    }
    [self.sceneID setStringValue:_gameSceneContextViewer.sceneID];
    if ([_gameSceneContextViewer.sceneName length] > 0) {
        [self.sceneName setStringValue:_gameSceneContextViewer.sceneName];
    } else {
        [self.sceneName setStringValue:@"木有名字~~~~"];
    }
    [self.scenePackageType setStringValue:_gameSceneContextViewer.scenePackageType];
    [self.internal setStringValue:_gameSceneContextViewer.internal];
    [self.tmx setStringValue:_gameSceneContextViewer.tmx];
    [self.sound setStringValue:_gameSceneContextViewer.sound];
    if ([_gameSceneContextViewer.trigger length] > 0) {
        [self.trigger setStringValue:_gameSceneContextViewer.trigger];
    } else {
        [self.trigger setStringValue:@"木有场景触发器~~~~"];
    }
    
    [self.entrances reloadData];
    [self.objects reloadData];
    
    [_progressIndicator setHidden:YES];
    [_progressIndicator stopAnimation:self];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

@end
