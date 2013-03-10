//
//  MCTaskStatus.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-4.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTaskStatus.h"

/**
 * 任务是否已经完成
 */
bool
MCTaskStatus::isDone()
{
    return progress_ == task_->getContent()->count();
}
