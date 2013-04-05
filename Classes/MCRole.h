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
#include "MCAI.h"

/* 基础角色 */
class MCRole : public MCObject {
public:
    MCRole();
    ~MCRole();
    virtual bool init();
    
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
    
    virtual CCObject *copy() = 0;
    
    inline mc_hp_t updateHP(mc_hp_t var) {
        hp_ += var;
        if (hp_ > maxHP_) {
            hp_ = maxHP_;
        } else if (hp_ < 0) {
            hp_ = 0;
        }
        
        return hp_;
    }

    inline mc_pp_t updatePP(mc_pp_t var) {
        pp_ += var;
        if (pp_ > maxPP_) {
            pp_ = maxPP_;
        } else if (pp_ < 0) {
            pp_ = 0;
        }
        
        return pp_;
    }
    
    /* 角色属性 */
    CC_SYNTHESIZE(MCRoleRace, roleRace_, RoleRace); /* 角色种族 */
    CC_SYNTHESIZE(mc_hp_t, hp_, HP); /* 角色生命值 */
    CC_SYNTHESIZE(mc_pp_t, pp_, PP); /* 角色体力值 */
    CC_SYNTHESIZE(mc_hp_t, maxHP_, MaxHP); /* 角色满生命值 */
    CC_SYNTHESIZE(mc_pp_t, maxPP_, MaxPP); /* 角色满体力值 */
    CC_SYNTHESIZE(MCRoleState, roleState_, RoleState); /* 角色状态 */
    CC_SYNTHESIZE(CCString *, face_, Face); /* 角色头像 */
    CC_SYNTHESIZE(CCString *, spriteSheet_, SpriteSheet); /* 角色精灵表 */
    
    CC_SYNTHESIZE(CCString *, defaultDialogue_, DefaultDialogue); /* 默认对白 */
    
    /* 显示相关 */
    CC_PROPERTY_READONLY(MCRoleEntity *, entity_, Entity); /* entity_将共用metadata_ */
    
    CC_SYNTHESIZE_READONLY(MCRoleEntityMetadata *, entityMetadata_, EntityMetadata); /* entity_将共用这个metadata */
    
    /* AI */
    CC_SYNTHESIZE_READONLY(MCAI *, ai_, AI);
};

#endif /* defined(__Military_Confrontation__MCRole__) */
