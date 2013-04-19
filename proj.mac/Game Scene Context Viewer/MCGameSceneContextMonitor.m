//
//  MCGameSceneContextMonitor.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <unistd.h>

#import "JSONKit.h"
#import "MCGameSceneContextMonitor.h"
#import "MCEntrance.h"
#import "MCRole.h"
#import "MCMercenary.h"
#import "MCEnemy.h"

NSString *kMCDataDidRefreshNotification = @"kMCDataDidRefreshNotification";

//static char kMCStopServerConstant[] = { 0x13, 'S', 'T', 'O', 'P', 0x13 };
static char kMCRequestDataConstant[] = { 0x13, 'R', 'E', 'Q', 'D', 0x13 };

static char kMCSceneDidChangeNotificationConstant[] = { 0x13, 'S', 'D', 'C', 'N', 0x13 };

static const int kMCListeningPort = 4444;
static const int kMCNotifyingPort = 4445;

@interface MCGameSceneContextMonitor ()
{
    int listeningSocketFD_;
}

@end

@implementation MCGameSceneContextMonitor
@synthesize sceneID=sceneID_;
@synthesize sceneName=sceneName_;
@synthesize scenePackageType=scenePackageType_;
@synthesize internal=internal_;
@synthesize tmx=tmx_;
@synthesize sound=sound_;
@synthesize trigger=trigger_;
@synthesize entrances=entrances_;
@synthesize objects=objects_;

- (id)init
{
    self = [super init];
    if (self) {
        sceneID_ = nil;
        sceneName_ = nil;
        scenePackageType_ = nil;
        internal_ = nil;
        tmx_ = nil;
        sound_ = nil;
        entrances_ = [[NSMutableArray alloc] init];
        objects_ = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)dealloc
{
    [objects_ release];
    [entrances_ release];
    [super dealloc];
}

/**
 * 监听服务器发的刷新通知
 */
- (void)listen
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    size_t optval = 1;
    
    if (sockfd < 0) {
        perror("不能创建socket！");
        return;
    }
    
    struct sockaddr_in addr;
    socklen_t addrLength = sizeof(addr);
    
    /* 重用地址 */
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
    
    /* 发送超时 */
    optval = 6000; /* 6秒 */
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &optval, sizeof(optval));
    
    memset(&addr, 0, addrLength);
    addr.sin_len = addrLength;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(kMCNotifyingPort);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (const struct sockaddr *) &addr, addrLength) == -1) {
        perror("cannot bind address!");
        return;
    }
    
    listeningSocketFD_ = sockfd;
    
    [self performSelectorInBackground:@selector(receiveData:) withObject:nil];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        int sockfd = listeningSocketFD_;
        struct sockaddr_in clientAddr;
        socklen_t clientLength = sizeof(struct sockaddr_in);
        ssize_t n;
        
        for (;;) {
            char buffer[4096];
            
            memset(buffer, 0, sizeof(buffer));
            n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                         (struct sockaddr *) &clientAddr, &clientLength);
            if (n < 0) {
                perror("接收数据错误");
            }
            if (memcmp(buffer,
                       kMCSceneDidChangeNotificationConstant,
                       sizeof(kMCSceneDidChangeNotificationConstant)) == 0) { /* 场景刷新了 */
                [self refresh];
            } else {
                [self.textView setString:[[self.textView string] stringByAppendingString:[NSString stringWithUTF8String:buffer]]];
                [self.textView scrollToEndOfDocument:self];
                [self.logWindow orderFront:self];
            }
        }
    });
    
    NSLog(@"listening began");
}

/**
 * 刷新数据
 */
- (void)refresh
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [self requestData:self];
    });
}

- (void)receiveData:(id)anObject
{
}

- (void)requestData:(id)anObject
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    socklen_t addrLength = sizeof(addr);
    char buffer[40960];
    ssize_t n;
    size_t optval = 1000; /* 1秒 */
    
    /* 发送超时 */
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &optval, sizeof(optval));
    /* 接收超时 */
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &optval, sizeof(optval));
    
    memset(&addr, 0, addrLength);
    addr.sin_len = addrLength;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(kMCListeningPort);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    sendto(sockfd, kMCRequestDataConstant, sizeof(kMCRequestDataConstant), 0,
           (const struct sockaddr *) &addr, addrLength);
    
    memset(buffer, 0, sizeof(buffer));
    n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                 (struct sockaddr *) &addr, &addrLength);
    
    NSData *data = [NSData dataWithBytes:buffer length:n];
    JSONDecoder *decoder = [JSONDecoder decoder];
    NSDictionary *objects = [decoder objectWithData:data];
    
    self.sceneID = [objects objectForKey:@"id"];
    self.sceneName = [objects objectForKey:@"name"];
    self.scenePackageType = [objects objectForKey:@"scene-type"];
    self.internal = [objects objectForKey:@"internal"];
    self.trigger = [objects objectForKey:@"trigger"];
    
    NSDictionary *background = [objects objectForKey:@"background"];
    self.tmx = [background objectForKey:@"tmx"];
    self.sound = [background objectForKey:@"sound"];
    
    NSArray *scenes = [objects objectForKey:@"scenes"];
    NSMutableArray *entrances = (NSMutableArray *) self.entrances;
    [entrances removeAllObjects];
    for (NSDictionary *scene in scenes) {
        MCEntrance *entrance = [[MCEntrance alloc] init];
        entrance.ID = [scene objectForKey:@"id"];
        entrance.name = [scene objectForKey:@"name"];
        [entrances addObject:entrance];
    }
    
    NSDictionary *roles = [objects objectForKey:@"roles"];
    NSMutableArray *roleObjects = (NSMutableArray *) self.objects;
    NSArray *team = [roles objectForKey:@"team"];
    NSString *roleType;
    MCRole *role;
    MCMercenary *mercenary;
    [roleObjects removeAllObjects];
    for (NSDictionary *aRole in team) {
        roleType = [aRole objectForKey:@"role-type"];
        if ([roleType isEqualToString:@"佣兵"]) {
            role = (MCRole *) [[MCMercenary alloc] init];
            mercenary = (MCMercenary *) role;
        } else {
            role = [[MCRole alloc] init];
            mercenary = nil;
        }
        role.roleType = roleType;
        
        role.AIState = [aRole objectForKey:@"ai-state"];
        role.defaultDialogue = [aRole objectForKey:@"default-dialogue"];
        role.exhausted = [[aRole objectForKey:@"exhausted"] boolValue];
        role.exhaustion = [[aRole objectForKey:@"exhaustion"] unsignedIntegerValue];
        role.face = [aRole objectForKey:@"face"];
        role.hp = [[aRole objectForKey:@"hp"] unsignedIntegerValue];
        role.ID = [aRole objectForKey:@"id"];
        role.maxHP = [[aRole objectForKey:@"max-hp"] unsignedIntegerValue];
        role.maxPP = [[aRole objectForKey:@"max-pp"] unsignedIntegerValue];
        role.name = [aRole objectForKey:@"name"];
        role.pp = [[aRole objectForKey:@"pp"] unsignedIntegerValue];
        role.roleRace = [aRole objectForKey:@"role-race"];
        role.roleState = [aRole objectForKey:@"role-state"];
        role.spriteSheet = [aRole objectForKey:@"sprite-sheet"];
        role.tired = [[aRole objectForKey:@"tired"] unsignedIntegerValue];
        role.trigger = [aRole objectForKey:@"trigger"];
        if (mercenary) {
            mercenary.ac = [[aRole objectForKey:@"ac"] unsignedIntegerValue];
            mercenary.armorCheckPenalty = [[aRole objectForKey:@"armor-check-penalty"] unsignedIntegerValue];
            mercenary.cost = [[aRole objectForKey:@"cost"] unsignedIntegerValue];
            mercenary.criticalHit = [[aRole objectForKey:@"critical-hit"] unsignedIntegerValue];
            
            NSDictionary *diceRangeDictionary = [aRole objectForKey:@"critical-hit-invisible"];
            MCDiceRange diceRange;
            diceRange.min = [[diceRangeDictionary objectForKey:@"min"] unsignedIntegerValue];
            diceRange.max = [[diceRangeDictionary objectForKey:@"max"] unsignedIntegerValue];
            diceRange.dice.count = [[diceRangeDictionary objectForKey:@"count"] unsignedIntegerValue];
            diceRange.dice.size = [[diceRangeDictionary objectForKey:@"size"] unsignedIntegerValue];
            mercenary.criticalHitInvisible = diceRange;
            
            diceRangeDictionary = [aRole objectForKey:@"critical-hit-visible"];
            diceRange.min = [[diceRangeDictionary objectForKey:@"min"] unsignedIntegerValue];
            diceRange.max = [[diceRangeDictionary objectForKey:@"max"] unsignedIntegerValue];
            diceRange.dice.count = [[diceRangeDictionary objectForKey:@"count"] unsignedIntegerValue];
            diceRange.dice.size = [[diceRangeDictionary objectForKey:@"size"] unsignedIntegerValue];
            mercenary.criticalHitVisible = diceRange;
            
            NSDictionary *diceDictionary = [aRole objectForKey:@"damage"];
            MCDice dice;
            dice.count = [[diceDictionary objectForKey:@"count"] unsignedIntegerValue];
            dice.size = [[diceDictionary objectForKey:@"size"] unsignedIntegerValue];
            mercenary.damage = dice;
            
            mercenary.damageBonus = [[aRole objectForKey:@"damage-bonus"] unsignedIntegerValue];
            
            mercenary.dexterity = [[aRole objectForKey:@"dexterity"] unsignedIntegerValue];
            mercenary.distance = [[aRole objectForKey:@"distance"] unsignedIntegerValue];
            mercenary.dying = [[aRole objectForKey:@"dying"] unsignedIntegerValue];
            
            mercenary.effect = [aRole objectForKey:@"effect"];
            if ([mercenary.effect length] > 0) {
                diceRangeDictionary = [aRole objectForKey:@"effect-check"];
                diceRange.min = [[diceRangeDictionary objectForKey:@"min"] unsignedIntegerValue];
                diceRange.max = [[diceRangeDictionary objectForKey:@"max"] unsignedIntegerValue];
                diceRange.dice.count = [[diceRangeDictionary objectForKey:@"count"] unsignedIntegerValue];
                diceRange.dice.size = [[diceRangeDictionary objectForKey:@"size"] unsignedIntegerValue];
                mercenary.effectCheck = diceRange;
            }
                                
        }
        [roleObjects addObject:role];
        
    }
    
    NSArray *others = [roles objectForKey:@"others"];
    MCEnemy *enemy;
    for (NSDictionary *aRole in others) {
        roleType = [aRole objectForKey:@"role-type"];
        if ([roleType isEqualToString:@"敌人"]) {
            role = (MCRole *) [[MCEnemy alloc] init];
            enemy = (MCEnemy *) role;
        } else {
            role = [[MCRole alloc] init];
            enemy = nil;
        }
        role.roleType = roleType;
        
        role.AIState = [aRole objectForKey:@"ai-state"];
        role.defaultDialogue = [aRole objectForKey:@"default-dialogue"];
        role.exhausted = [[aRole objectForKey:@"exhausted"] boolValue];
        role.exhaustion = [[aRole objectForKey:@"exhaustion"] unsignedIntegerValue];
        role.face = [aRole objectForKey:@"face"];
        role.hp = [[aRole objectForKey:@"hp"] unsignedIntegerValue];
        role.ID = [aRole objectForKey:@"id"];
        role.maxHP = [[aRole objectForKey:@"max-hp"] unsignedIntegerValue];
        role.maxPP = [[aRole objectForKey:@"max-pp"] unsignedIntegerValue];
        role.name = [aRole objectForKey:@"name"];
        role.pp = [[aRole objectForKey:@"pp"] unsignedIntegerValue];
        role.roleRace = [aRole objectForKey:@"role-race"];
        role.roleState = [aRole objectForKey:@"role-state"];
        role.spriteSheet = [aRole objectForKey:@"sprite-sheet"];
        role.tired = [[aRole objectForKey:@"tired"] unsignedIntegerValue];
        role.trigger = [aRole objectForKey:@"trigger"];
        if (enemy) {
            enemy.ac = [[aRole objectForKey:@"ac"] unsignedIntegerValue];
            enemy.armorCheckPenalty = [[aRole objectForKey:@"armor-check-penalty"] unsignedIntegerValue];
            enemy.criticalHit = [[aRole objectForKey:@"critical-hit"] unsignedIntegerValue];
            
            NSDictionary *diceRangeDictionary = [aRole objectForKey:@"critical-hit-invisible"];
            MCDiceRange diceRange;
            diceRange.min = [[diceRangeDictionary objectForKey:@"min"] unsignedIntegerValue];
            diceRange.max = [[diceRangeDictionary objectForKey:@"max"] unsignedIntegerValue];
            diceRange.dice.count = [[diceRangeDictionary objectForKey:@"count"] unsignedIntegerValue];
            diceRange.dice.size = [[diceRangeDictionary objectForKey:@"size"] unsignedIntegerValue];
            enemy.criticalHitInvisible = diceRange;
            
            diceRangeDictionary = [aRole objectForKey:@"critical-hit-visible"];
            diceRange.min = [[diceRangeDictionary objectForKey:@"min"] unsignedIntegerValue];
            diceRange.max = [[diceRangeDictionary objectForKey:@"max"] unsignedIntegerValue];
            diceRange.dice.count = [[diceRangeDictionary objectForKey:@"count"] unsignedIntegerValue];
            diceRange.dice.size = [[diceRangeDictionary objectForKey:@"size"] unsignedIntegerValue];
            enemy.criticalHitVisible = diceRange;
            
            NSDictionary *diceDictionary = [aRole objectForKey:@"damage"];
            MCDice dice;
            dice.count = [[diceDictionary objectForKey:@"count"] unsignedIntegerValue];
            dice.size = [[diceDictionary objectForKey:@"size"] unsignedIntegerValue];
            enemy.damage = dice;
            
            enemy.damageBonus = [[aRole objectForKey:@"damage-bonus"] unsignedIntegerValue];
            
            enemy.dexterity = [[aRole objectForKey:@"dexterity"] unsignedIntegerValue];
            enemy.distance = [[aRole objectForKey:@"distance"] unsignedIntegerValue];
            
            enemy.effect = [aRole objectForKey:@"effect"];
            if ([enemy.effect length] > 0) {
                diceRangeDictionary = [aRole objectForKey:@"effect-check"];
                diceRange.min = [[diceRangeDictionary objectForKey:@"min"] unsignedIntegerValue];
                diceRange.max = [[diceRangeDictionary objectForKey:@"max"] unsignedIntegerValue];
                diceRange.dice.count = [[diceRangeDictionary objectForKey:@"count"] unsignedIntegerValue];
                diceRange.dice.size = [[diceRangeDictionary objectForKey:@"size"] unsignedIntegerValue];
                enemy.effectCheck = diceRange;
            }
        }
        
        [roleObjects addObject:role];
    }
    
    [[NSNotificationCenter defaultCenter] postNotificationName:kMCDataDidRefreshNotification
                                                        object:nil];
}

@end
