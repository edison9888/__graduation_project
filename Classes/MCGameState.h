//
//  MCGameState.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCGameState__
#define __Military_Confrontation__MCGameState__

#include <cocos2d.h>
USING_NS_CC;

/**
 * 保存或读取游戏的状态
 * 单例模式
 * 游戏只会在开始任务前和任务结束的时候，还有退出游戏的时候储存。没有中断游戏的功能！
 */
class MCGameState {
private:
    MCGameState();
    
public:
    ~MCGameState();
    static MCGameState* sharedGameState();
    
    /**
     * 保存
     *
     * 保存的东西列表
     * 背包(装备)信息
     * 背包(道具)信息
     * 佣兵信息 所雇佣的佣兵和佣兵的状态
     * 标志信息
     * 任务信息
     * 技能信息
     * 重生点 重生点所在场景的ID
     */
    void save();
    
    /**
     * 读取
     */
    void load();
};

#endif /* defined(__Military_Confrontation__MCGameState__) */
