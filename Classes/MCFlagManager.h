//
//  MCFlagManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-10.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCFlagManager__
#define __Military_Confrontation__MCFlagManager__

#include "MCFlag.h"

class MCFlagManager {
private:
    MCFlagManager();
    
public:
    ~MCFlagManager();
    
    static MCFlagManager *sharedFlagManager();
    
    MCFlag *flagForObjectId(mc_object_id_t anObjectId);
    MCFlagState flagStateForObjectId(mc_object_id_t anObjectId);
    
    void updateFlag(MCFlag *aFlag);
    
    /**
     * 清除数据
     */
    void erase();
    
    /**
     * save all flags
     */
    void saveAllFlags();
    
    inline bool isTaskStarted() {
        return taskFlag_->getState() == MCOnState;
    }
    
    inline void setTaskStarted(bool flag) {
        taskFlag_->setState(flag ? MCOnState : MCOffState);
    }
    
    inline void unblockArea() {
        areaBlockedFlag_->setState(MCOffState);
    }
    
    inline bool isSpawnFlagOn() {
        return spawnFlag_->getState() == MCOnState;
    }
    
    inline void setSpawnFlagOff() {
        spawnFlag_->setState(MCOffState);
    }
    
    inline void spawn() {
        spawnFlag_->setState(MCOnState);
    }
    
    /**
     * load all flags
     */
    void loadAllFlags();
    
private:
    
    CCDictionary *sourceFlags_; /* 源文件的标志 */
    CCDictionary *flags_;       /* 存储过的标志 */
    
    /* 特殊flag */
    MCFlag *taskFlag_; /* 任务状态 */
    MCFlag *areaBlockedFlag_; /* 高级区域封锁 */
    MCFlag *spawnFlag_; /* 是否在重生点出现 */
};

#endif /* defined(__Military_Confrontation__MCFlagManager__) */
