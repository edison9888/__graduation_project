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

class MCShadow;
class MCRole;
class MCAStarAlgorithm;

class MCRoleEntityMetadata : public CCObject {
    friend class MCRole;
    friend class MCRoleEntity;
public:
    MCRoleEntityMetadata();
    ~MCRoleEntityMetadata();
    
private:
    CCString *spriteSheetPath_;
    
    /* 显示相关 */
    CC_SYNTHESIZE_READONLY(CCSpriteBatchNode *, spriteSheet_, SpriteSheet);
    CC_SYNTHESIZE_READONLY(MCFacade, facade_, Facade);
    
    /* 移动动画 */
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoUp_, AnimationGoUp);
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoDown_, AnimationGoDown);
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoLeft_, AnimationGoLeft);
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoRight_, AnimationGoRight);
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCSize, frameSize_, FrameSize);
    
    /* 场景包数据 */
public:
    inline float getX() {
        return position_.x;
    }
    inline float getY() {
        return position_.y;
    }

    CC_SYNTHESIZE(CCPoint, position_, Position);
    CC_SYNTHESIZE(CCArray *, flags_, Flags);
};

class MCRoleEntity : public CCSprite {
    friend class MCRole;
    friend class MCAI;
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
    void walk(const CCPoint &delta);
    
    void drag(const CCPoint &aDelta);
    
    /**
     * 积累步数消耗体力，用户战斗中移动
     * 1点体力/24像素
     */
    void move(const CCPoint &aDelta);
    
    bool isWalking();
    void stopWalking();
    void stopWalkAction();
    
    /**
     * 使用寻路算法
     * aDestinationLocation为屏幕上的坐标，所以要加上地图偏移
     */
    void findPath(const CCPoint &aDestinationLocation);
    void findPath(const CCPoint &aDestinationLocation, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserdata=NULL);
    
    void findPathAtMap(const CCPoint &aDestinationLocation);
    void findPathAtMap(const CCPoint &aDestinationLocation, CCObject *aTarget, SEL_CallFuncO aSelector, CCObject *anUserdata=NULL);
    
    /**
     * 测试某个位置是否能站
     * aDestinationLocation为屏幕上的坐标，所以要加上地图偏移
     */
//    bool testPosition(const CCPoint &aDestinationLocation);
//    
//    bool testPositionAtMap(const CCPoint &aDestinationLocation);
    
    /**
     * 寻路结束
     */
    void pathFindingDidFinish(CCObject *obj);
    
protected:
    void walkWithPathFinding(CCObject *algoObject);
    
    void actionEnded(CCObject* anObject);
    void stopAllMoveToActions();
    
    /* 创建A星实例 */
    MCAStarAlgorithm *pathFindingAlgoInstance();
    
    /* AI */
    void startThinking();
    void stopThinking();
    void thinking(float dt);
    void checkObjects(float dt); /* 调用AI的checkObjects */
    
    CC_PROPERTY_READONLY(CCSpriteBatchNode *, spriteSheet_, SpriteSheet);
    CC_PROPERTY_READONLY(MCFacade, facade_, Facade);
    CC_PROPERTY_READONLY(MCRoleEntityMetadata *, metadata_, Metadata);
    
    CC_SYNTHESIZE_READONLY(CCSprite *, selectedShadow_, SelectedShadow);
    
    CC_SYNTHESIZE_READONLY(MCRole *, role_, Prototype); /* 指向MCRole */
    
    CC_SYNTHESIZE(MCShadow *, shadow_, Shadow); /* 影子 */
    
private:
    MCAStarAlgorithm *pathFindingAlgo_; /* 寻路算法实例 */
    
    CCArray *moveToActions_;
    
    /* 碰撞 */
    CC_PROPERTY_READONLY_PASS_BY_REF(MCOBB, obb_, OBB);    /* OBB */
    
    /* path finding callback */
    CCObject *target_;
    SEL_CallFuncO pathFindingSelector_;
    CCObject *pathFindingSelectorUserdata_;
    
    CCAction *walkAction_;
};

#endif /* defined(__Military_Confrontation__MCRoleEntity__) */
