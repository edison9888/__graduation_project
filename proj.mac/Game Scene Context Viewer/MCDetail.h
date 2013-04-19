//
//  MCDetail.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class MCGameSceneContextMonitor;

@interface MCDetail : NSPanel<NSTableViewDataSource, NSTableViewDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSTableView *tableView;

@property (assign) IBOutlet MCGameSceneContextMonitor *gameSceneContextViewer;
@property (nonatomic) NSInteger selectedRow;

@end
