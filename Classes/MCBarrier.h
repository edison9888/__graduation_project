//
//  MCBarrier.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBarrier__
#define __Military_Confrontation__MCBarrier__

#include "MCRoleEntity.h"

extern const char *kMCTypeBarrier;

/* 障碍物类型 */
enum {
    MCNormalBarrier    = MCMakeEnum(0), /* 普通障碍物，路上生物不能越过它 */
    MCAdvancedBarrier  = MCMakeEnum(1), /* 高级障碍物，飞行生物不能越过它 */
    MCGodLikeBarrier   = MCMakeEnum(2), /* 如同神一般的障碍物，要超过它你需要超越神的存在 */
    MCBeyondGodBarrier = MCMakeEnum(3)  /* 超越神的存在的障碍物，你绝对不可能越过它！ */
};
typedef mc_enum_t MCBarrierType;

class MCBarrier : public CCObject {
public:
    bool init(const CCRect &aRect, MCBarrierType aBarrierType);
    
    static MCBarrier* create(const CCRect &aRect, MCBarrierType aBarrierType);
    
    bool collideWith(const CCRect &aTargetRect);
    
    /**
     * 检测是否与人物碰撞
     *
     * aRoleEntity(in):   人物实体
     * anOffsetAtMap(in): 人物在地图上的位置
     *
     * 返回值你懂的
     */
    bool collideWith(MCRoleEntity *aRoleEntity, const CCPoint &anOffsetAtMap);
    
    CC_SYNTHESIZE_READONLY(CCRect, rect_, Rect);
    CC_SYNTHESIZE_READONLY(MCBarrierType, type_, BarrierType);
};

#endif /* defined(__Military_Confrontation__MCBarrier__) */
