//
//  MCAppDelegate.h
//  Game Scene Context Viewer
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class MCGameSceneContextMonitor;
@class MCDetail;

@interface MCAppDelegate : NSObject <NSApplicationDelegate, NSTableViewDataSource, NSTableViewDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet MCGameSceneContextMonitor *gameSceneContextViewer;
@property (assign) IBOutlet MCDetail *detail;

@property (assign) IBOutlet NSTextField *sceneID;
@property (assign) IBOutlet NSTextField *sceneName;
@property (assign) IBOutlet NSTextField *scenePackageType;
@property (assign) IBOutlet NSTextField *internal;
@property (assign) IBOutlet NSTextField *tmx;
@property (assign) IBOutlet NSTextField *sound;
@property (assign) IBOutlet NSTextField *trigger;
@property (assign) IBOutlet NSTableView *entrances;
@property (assign) IBOutlet NSTableView *objects;
@property (assign) IBOutlet NSProgressIndicator *progressIndicator;

- (IBAction)refresh:(id)sender;

- (IBAction)showDetail:(id)sender;

@end
