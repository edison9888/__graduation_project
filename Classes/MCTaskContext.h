//
//  MCTaskContext.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-4.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTaskContext__
#define __Military_Confrontation__MCTaskContext__

#include "MCTask.h"

/* 行动类别，对应action字段 */
enum {
    MCMoveToAction      = MCMakeEnum(0), /* 移动到某个区域 */
    MCTalkAction        = MCMakeEnum(1), /* 与某人谈话 */
    MCGetAction         = MCMakeEnum(2), /* 从某人处获得某物 */
    MCGiveAction        = MCMakeEnum(3), /* 给予某人某物 */
    MCHuntAction        = MCMakeEnum(4), /* 击杀/击败某人/某物 */
    MCCollectionAction  = MCMakeEnum(5), /* 收集/拯救某物/某人 */
    MCEventAction       = MCMakeEnum(6), /* 发生事件 */
};
typedef mc_enum_t MCActionCategory;

/* 任务上下文 */
class MCActionContext : public MCObject {
    friend class MCTask;
    
    CC_SYNTHESIZE_READONLY(MCActionCategory, action_, Action); /* 行动类别 */
};

/* 移动到某个区域的任务 */
class MCMoveActionContext : public MCActionContext {
public:
    CC_SYNTHESIZE_READONLY(mc_object_id_t, objectId_, ObjectId); /* 地图ID */
    CC_SYNTHESIZE_READONLY(int, x_, X);                          /* 地图中的x坐标 */
    CC_SYNTHESIZE_READONLY(int, y_, Y);                          /* 地图中的y坐标 */
    CC_SYNTHESIZE_READONLY(int, w_, W);                          /* 区域宽度 */
    CC_SYNTHESIZE_READONLY(int, h_, H);                          /* 区域高度 */
};

/* 与某人谈话的任务 */
class MCTalkActionContext : public MCActionContext {
public:
    CC_SYNTHESIZE_READONLY(mc_object_id_t, objectId_, ObjectId); /* 要对话的人物的ID */
};

/* 从某人处获得某物的任务 */
class MCGetActionContext : public MCActionContext {
public:
    CC_SYNTHESIZE_READONLY(mc_object_id_t, objectId_, ObjectId); /* NPC的ID */
    CC_SYNTHESIZE_READONLY(CCDictionary *, items_, Items); /* 物品的字典，key为物品的ID，value为物品的数量 */
};

/* 给予某人某物的任务 */
class MCGiveActionContext : public MCActionContext {
public:
    CC_SYNTHESIZE_READONLY(mc_object_id_t, objectId_, ObjectId); /* 需要给予的NPC的ID */
    CC_SYNTHESIZE_READONLY(CCDictionary *, items_, Items); /* 物品的字典，key为物品的ID，value为物品的数量 */
};

/* 击杀/击败某人/某物的任务 */
class MCHuntActionContext : public MCActionContext {
public:
    CC_SYNTHESIZE_READONLY(CCDictionary *, targets_, Targets); /* 目标的字典，key为目标的ID，value为目标的数量 */
};

/* 收集/拯救某物/某人的任务 */
class MCCollectionActionContext : public MCActionContext {
public:
    CC_SYNTHESIZE_READONLY(CCDictionary *, targets_, Targets); /* 目标的字典，key为目标的ID，value为目标的数量 */
};

/* 收集/拯救某物/某人的任务 */
class MCScriptActionContext : public MCActionContext {
public:
    CC_SYNTHESIZE_READONLY(CCString *, script_, Script); /* 脚本文件路径 */
};

#endif /* defined(__Military_Confrontation__MCTaskContext__) */
