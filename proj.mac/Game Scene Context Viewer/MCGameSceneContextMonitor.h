//
//  MCGameSceneContextMonitor.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString *kMCDataDidRefreshNotification;

@interface MCGameSceneContextMonitor : NSObject

@property (assign) IBOutlet NSWindow *logWindow;
@property (assign) IBOutlet NSTextView *textView;

@property (nonatomic, copy) NSString *sceneID;
@property (nonatomic, copy) NSString *sceneName;
@property (nonatomic, copy) NSString *scenePackageType;
@property (nonatomic, copy) NSString *internal;
@property (nonatomic, copy) NSString *tmx;
@property (nonatomic, copy) NSString *sound;
@property (nonatomic, copy) NSString *trigger;

/**
 * 场景内所有入口
 */
@property (nonatomic, retain) NSArray *entrances;

/**
 * 场景内所有对象
 */
@property (nonatomic, retain) NSArray *objects;

/**
 * 监听服务器发的刷新通知
 */
- (void)listen;

/**
 * 刷新数据
 */
- (void)refresh;

@end
