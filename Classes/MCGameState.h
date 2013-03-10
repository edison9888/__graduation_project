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
 */
class MCGameState {
private:
    MCGameState();
    
public:
    ~MCGameState();
    static MCGameState* sharedGameState();
    
    /**
     * 保存checkpoint
     */
    void saveCheckPoint();
    
    /**
     * 读取checkpoint
     */
    void loadCheckPoint();
    
    /**
     * 中断游戏
     */
    void interrupt();
    
    /**
     * 读取中断的游戏
     */
    void loadInterruption();
};

#endif /* defined(__Military_Confrontation__MCGameState__) */
