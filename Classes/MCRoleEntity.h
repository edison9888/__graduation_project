//
//  MCRoleEntity.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRoleEntity__
#define __Military_Confrontation__MCRoleEntity__

#include "MCObject.h"
#include "MCOBB.h"

/* 角色朝向 */
enum {
    MCFacingUnknown = 0,             /* 未知，尚未生成实体的时候 */
    MCFacingUp      = MCMakeEnum(0), /* 面朝上 */
    MCFacingDown    = MCMakeEnum(1), /* 面朝下 */
    MCFacingLeft    = MCMakeEnum(2), /* 面朝左 */
    MCFacingRight   = MCMakeEnum(3)  /* 面朝右 */
};
typedef mc_enum_t MCFacade;

class MCRole;

class MCRoleEntityMetadata : public CCSprite {
    friend class MCRole;
    friend class MCRoleEntity;
    
public:
    MCRoleEntityMetadata();
    ~MCRoleEntityMetadata();
    
    CCString *spriteSheetPath_;
    
    /* 显示相关 */
    CC_SYNTHESIZE_READONLY(CCSpriteBatchNode *, spriteSheet_, SpriteSheet);
    CC_SYNTHESIZE_READONLY(MCFacade, facade_, Facade);
    
    /* 移动动画 */
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoUp_, AnimationGoUp);
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoDown_, AnimationGoDown);
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoLeft_, AnimationGoLeft);
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoRight_, AnimationGoRight);
    
    /* 场景包数据 */
public:
    inline float getX() {
        return position_.x;
    }
    inline float getY() {
        return position_.y;
    }
    
    CC_SYNTHESIZE(CCPoint, position_, Position);
    CC_SYNTHESIZE(CCArray *, requirements_, Requirements);
};

class MCRoleEntity : public CCSprite {
    friend class MCRole;
    
public:
    MCRoleEntity();
    ~MCRoleEntity();
    
    void onEnter();
    void onExit();
    void update(float dt);

    /* 选中 */
    bool shouldBeSelected(const CCPoint &aPoint); /* AABB */
    
    void face(MCFacade aFacade);
    void walk(MCFacade aFacade);
    void walkTo(CCPoint &aDestinationPosition);
    
    /* 坑爹啊！直接moveby居然不行！ */
    void moveBy(CCPoint &aDelta);
    
    void walkOnScreen(const CCPoint &aDestinationLocation, const CCPoint &offset);
    bool isWalking();
    void stopWalking();
    
    /**
     * 使用寻路算法
     */
    void findPath(const CCPoint &aDestinationLocation);
    
    /**
     * 寻路结束
     */
    void findPathDidFinish(CCObject *obj);
    
    void walkEnded();
    
protected:
    
    void actionEnded(CCObject* anObject);
    void stopAllMoveToActions();
    
    CC_PROPERTY_READONLY(CCSpriteBatchNode *, spriteSheet_, SpriteSheet);
    CC_PROPERTY_READONLY(MCFacade, facade_, Facade);
    CC_PROPERTY_READONLY(MCRoleEntityMetadata *, metadata_, Metadata);
    
    CC_SYNTHESIZE_READONLY(MCRole *, role_, Prototype); /* 指向MCRole */
    
private:
    CCArray *moveToActions_;
    CCArray *moveToDestinations_; /* 移动目标列表 */
    
    /* 碰撞 */
    CC_PROPERTY_READONLY_PASS_BY_REF(MCOBB, obb_, OBB);    /* OBB */
};

#endif /* defined(__Military_Confrontation__MCRoleEntity__) */
