//
//  MCTMXGenerator.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-31.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCTMXGenerator.h"
#import "MCScenePackage.h"

@implementation MCTMXGenerator

- (id)init
{
    self = [super init];
    if (self) {
    }
    return self;
}

- (MCScenePackage *)generate:(NSString *)aTMXMapFilepath
{
    MCScenePackage *package = [[MCScenePackage alloc] init];
    
    NSData *data = [[NSData alloc] initWithContentsOfFile:aTMXMapFilepath];
    NSError *err;
    NSXMLDocument *mapDocument = [[NSXMLDocument alloc]
                                  initWithData:data
                                       options:NSXMLDocumentTidyXML
                                         error:&err];
    NSString *mapName = [aTMXMapFilepath lastPathComponent];
    /* scene-type */
    NSArray *properties = [mapDocument nodesForXPath:@"map/properties/property[@name='scene-type']" error:nil];
    NSXMLElement *property;
    if ([properties count] > 0) {
        property = [properties objectAtIndex:0];
        NSString *sceneType = [[property attributeForName:@"value"] objectValue];
        if ([sceneType compare:@"field"] == NSOrderedSame) {
            [package setObject:@(MCBattleFieldScenePackage) forKey:kMCScenePackageSceneType];
        }
    } else {
        [package setObject:@(MCGameScenePackage) forKey:kMCScenePackageSceneType];
    }
    
    properties = [mapDocument nodesForXPath:@"map/properties/property[@name='internal']" error:nil];
    BOOL isInternal = NO;
    if ([properties count] > 0) {
        property = [properties objectAtIndex:0];
        NSString *internal = [[property attributeForName:@"value"] objectValue];
        if ([internal intValue] == 1) {
            isInternal = YES;
        }
    }
    [package setObject:@(isInternal == YES ? 1 : 0) forKey:kMCScenePackageInternal];
    
    /* background */
    NSMutableDictionary *background = [[NSMutableDictionary alloc] init];
    /* tmx */
    properties = [mapDocument nodesForXPath:@"map/properties/property[@name='tmx']" error:nil];
    property = [properties objectAtIndex:0];
    NSString *path = [[property attributeForName:@"value"] objectValue];
    /* ID */
    [package setID:[[[path stringByDeletingPathExtension] pathComponents] objectAtIndex:1]];
//    NSLog(@"%@: %@", [aTMXMapFilepath lastPathComponent], [package ID]);
    [package setObject:[package ID] forKey:kMCScenePackageID];
    [package setOutputFilepath:[[aTMXMapFilepath stringByDeletingLastPathComponent]
                                stringByAppendingPathComponent:[[package ID] stringByAppendingString:@".jpkg"]]];
    NSString *mapPath = [@"scenes/" stringByAppendingString:mapName];;
    [background setObject:mapPath forKey:kMCScenePackageTMXMap];
    /* sound */
    properties = [mapDocument nodesForXPath:@"map/properties/property[@name='sound']" error:nil];
    property = [properties objectAtIndex:0];
    [background setObject:[[property attributeForName:@"value"] objectValue] forKey:kMCScenePackageSound];
    [package setObject:background forKey:kMCScenePackageBackground];
    
    /* objects */
    NSArray *objectNodes = [mapDocument nodesForXPath:@"map/objectgroup[@name='objects']/object" error:nil];
    NSMutableArray *objects = [[NSMutableArray alloc] initWithCapacity:[objectNodes count]];
    for (NSXMLElement *objectElement in objectNodes) {
        if ([[objectElement attributeForName:@"name"] objectValue]) { /* 有name的是复活点，暂时来说 */
            continue;
        }
        NSMutableDictionary *object = [[NSMutableDictionary alloc] initWithCapacity:3];
        /* ID */
//        NSLog(@"%@", objectElement);
        [object setObject:[[[[objectElement nodesForXPath:@"properties/property[@name='ID']"
                                                  error:nil] objectAtIndex:0] attributeForName:@"value"] objectValue]
                   forKey:kMCScenePackageObjectsID];
        /* x */
        [object setObject:@([[[objectElement attributeForName:@"x"] objectValue] integerValue]) forKey:kMCScenePackageObjectsX];
        /* y */
        [object setObject:@([[[objectElement attributeForName:@"y"] objectValue] integerValue]) forKey:kMCScenePackageObjectsY];
        /* flags */
        properties = [objectElement nodesForXPath:@"properties/property[@name='flags']" error:nil];
        if ([properties count] > 0) {
            property = [properties objectAtIndex:0];
            NSString *flags = [[property attributeForName:@"value"] objectValue];
            [object setObject:[flags componentsSeparatedByString:@","] forKey:kMCScenePackageObjectsFlags];
        }
        [objects addObject:object];
    }
    [package setObject:objects forKey:kMCScenePackageObjects];
    
    /* entrances */
    NSArray *entranceNodes = [mapDocument nodesForXPath:@"map/objectgroup[@name='entrances']/object" error:nil];
    NSMutableDictionary *entrances = [[NSMutableDictionary alloc] initWithCapacity:[objectNodes count]];
    for (NSXMLElement *entranceElement in entranceNodes) {
        NSMutableDictionary *entance = [[NSMutableDictionary alloc] initWithCapacity:2];
        /* ID */
        property = [[entranceElement nodesForXPath:@"properties/property[@name='ID']" error:nil] objectAtIndex:0];
        [entance setObject:[[property attributeForName:@"value"] objectValue] forKey:kMCScenePackageScenesID];
        /* dest */
        property = [[entranceElement nodesForXPath:@"properties/property[@name='dest']" error:nil] objectAtIndex:0];
        [entance setObject:[[property attributeForName:@"value"] objectValue] forKey:kMCScenePackageScenesDestination];
        
        [entrances setObject:entance forKey:[[entranceElement attributeForName:@"name"] objectValue]];
    }
    [package setObject:entrances forKey:kMCScenePackageScenes];
    
    /* 以下两项可能未配置 */
    /* name */
    properties = [mapDocument nodesForXPath:@"map/properties/property[@name='name']" error:nil];
    if ([properties count] > 0) { /* 木有配置选项 */
        property = [properties objectAtIndex:0];
        [package setObject:[[property attributeForName:@"value"] objectValue] forKey:kMCScenePackageName];
    }
    /* description */
    properties = [mapDocument nodesForXPath:@"map/properties/property[@name='description']" error:nil];
    if ([properties count] > 0) { /* 木有配置选项 */
        property = [properties objectAtIndex:0];
        [package setObject:[[property attributeForName:@"value"] objectValue] forKey:kMCScenePackageDescription];
    }
    
    return package;
}

@end
