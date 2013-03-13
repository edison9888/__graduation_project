//
//  MCRole.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRole__
#define __Military_Confrontation__MCRole__

#include "MCObject.h"
#include "MCRoleEntity.h"
#include "MCEffect.h"
#include "MCViewport.h"

/* 基础角色 */
class MCRole : public MCObject {
public:
    MCRole();
    ~MCRole();
    virtual bool init() = 0;
    
    /* 下一句话。如果没有则返回NULL；如果本来就没话说则返回默认对白 */
    const char *nextSentence();
    
    void loadSpriteSheet();
    void loadSpriteSheet(const char *aSpritesheetPath);
    
    /* 回调事件 */
    /**
     * 某人进入视野
     * 默认看到的都是敌人
     */
    virtual void onSeeSomeone(MCRole *aRole, bool isEnermy = true);
    
    /**
     * 某人离开视野
     * 默认离开的都是敌人
     */
    virtual void onSomeoneDidLeave(MCRole *aRole, bool isEnermy = true);
    
    /**
     * 被攻击
     */
    virtual void wasAttacked(const MCEffect &anEffect);
    
    /**
     * 死亡
     */
    virtual void died();
    
    /* 角色属性 */
    CC_SYNTHESIZE(MCRoleRace, roleRace_, RoleRace); /* 角色种族 */
    CC_SYNTHESIZE(MCRoleType, roleType_, RoleType); /* 角色类型 */
    CC_SYNTHESIZE(mc_hp_t, hp_, HP); /* 角色生命值 */
    CC_SYNTHESIZE(mc_pp_t, pp_, PP); /* 角色体力值 */
    CC_SYNTHESIZE(mc_load_t, load_, Load); /* 角色负重 */
    CC_SYNTHESIZE(MCRoleState, roleState_, RoleState); /* 角色状态 */
    CC_SYNTHESIZE(CCString *, face_, Face); /* 角色头像 */
    CC_SYNTHESIZE(CCString *, spriteSheet_, SpriteSheet); /* 角色精灵表 */
    
    CC_SYNTHESIZE(CCString *, defaultDialogue_, DefaultDialogue); /* 默认对白 */
    CC_SYNTHESIZE(CCArray *, dialogues_, Dialogues); /* 当前场景可能的对白 */
    CC_SYNTHESIZE(mc_index_t, nextSentenceIndex_, NextSentenceIndex); /* 下一句话的索引号 */
    
    /* 显示相关 */
    CC_PROPERTY_READONLY(MCRoleEntity *, entity_, Entity); /* entity_将共用metadata_ */
    
    CC_SYNTHESIZE_READONLY(MCRoleEntityMetadata *, entityMetadata_, EntityMetadata); /* entity_将共用这个metadata */
    
    /* 视觉 */
    CC_SYNTHESIZE_READONLY(MCViewport *, viewport_, Viewport);
};

#endif /* defined(__Military_Confrontation__MCRole__) */
