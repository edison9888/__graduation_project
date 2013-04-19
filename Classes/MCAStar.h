//
//  MCAStar.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCAStar__
#define __Military_Confrontation__MCAStar__

#include <cocos2d.h>
USING_NS_CC;
#include "MCType.h"
#include "MCGeometry.h"
#include "MCBarrier.h"
#include <stack>

#include <pthread.h>

extern const char *kMCAStarDidFinishAlgorithmNotification;

extern const mc_byte_t kMCBarrier;
//extern const mc_byte_t kMCStartPoint;
//extern const mc_byte_t kMCEndPoint;

class MCRole;
class MCAStarAlgorithm;
class MCAStar;

class MCAStarNode : public CCObject {
    friend class MCAStarAlgorithm;
public:
    /**
     * 根据父位置创建新的MCAStarItem。
     * 若父位置不为NULL，则dx和dy是相对于父位置的位置偏移。
     * 否则dx和dy是地图上的位置。
     */
    void setup(MCAStarNode *aParent, float dx, float dy);
    
    void manhattanMethod(MCAStarNode *aTargetPosition);
    
    inline CCPoint getPosition() {
        return position_;
    }
    
    inline void setPosition(const CCPoint &var) {
        position_ = var;
    }
    
    inline int getX() {
        return position_.x;
    }
    
    inline void setX(int x) {
        position_.x = x;
    }
    
    inline int getY() {
        return position_.y;
    }
    
    inline void setY(int y) {
        position_.y = y;
    }
    
    inline bool equals(const CCPoint &aPoint) {
        return position_.equals(aPoint);
    }
    
    inline MCAStarNode *getParent() {
        return parent_;
    }
    
    void setParent(MCAStarNode *aParent);
    
private:
    CCPoint position_;
    MCAStarNode *parent_;
    
	CC_SYNTHESIZE(int, g_, G);
	CC_SYNTHESIZE(int, h_, H);
	CC_SYNTHESIZE(int, f_, F);
};

class MCAStarAlgorithm : public CCObject {
    friend class MCAStar;
public:
    ~MCAStarAlgorithm();
    
    bool init();
    CREATE_FUNC(MCAStarAlgorithm);
    
    void setDestination(const CCPoint &aDestinationLocation);
    
    bool testPosition(const CCPoint &aDestinationLocation);
    
    /**
     * 坐标(x,y)是否为障碍
     */
    inline bool isBarrier(int x, int y) {
        if (x < 0 || x >= mapWidth_
            || y < 0 || y > mapHeight_) {
            return true;
        }
        
        return mapAltas_[x + y * mapWidth_] == kMCBarrier;
    }
    
    inline bool isBarrier(const CCPoint &aPoint){
        if (aPoint.x < 0 || aPoint.x >= mapWidth_
            || aPoint.y < 0 || aPoint.y > mapHeight_) {
            return true;
        }
        
        return mapAltas_[(mc_ssize_t) aPoint.x + (mc_ssize_t) aPoint.y * mapWidth_] == kMCBarrier;
    }
    
    inline mc_byte_t *getMapAltas() {
        return mapAltas_;
    }
    
    inline mc_size_t getMapWidth() {
        return mapWidth_;
    }
    
    inline mc_size_t getMapHeight() {
        return mapHeight_;
    }
    
    inline mc_size_t getEdge() {
        return edge_;
    }
    
    inline bool isProcessing() {
        return processing_;
    }
    
    void stopPathFinding();
    
    void execute();
    
protected:
    /**
     * 生成对象适用的变形地图
     */
    void generateMapAltas(MCRoleEntity *aRoleEntity, const CCSize &aMapSize, CCArray *barriers);
    
    void pathFindingDidFinish(CCObject *anObject);
    
private:
    mc_byte_t *mapAltas_; /* 根据图块生成的变形地图 */
    mc_size_t mapWidth_;
    mc_size_t mapHeight_;
    mc_size_t edge_; /* 一个块的边长 */
    
#if MC_ASTAR_USING_PTHREAD == 1
    pthread_t pid_;
#endif
    bool processing_;
    
    CC_SYNTHESIZE_READONLY(MCAStarNode *, startPoint_, StartPoint);
    CC_SYNTHESIZE_READONLY(MCAStarNode *, endPoint_, EndPoint);
    CC_SYNTHESIZE_READONLY(MCRoleEntity *, roleEntity_, RoleEntity);

public:
    std::stack<CCPoint> *route; /* 寻路结果 */
};

class MCAStar : public CCObject {
public:
    MCAStar();
    ~MCAStar();
    
    bool init(CCTMXTiledMap *aMap);
    
    static MCAStar *create(CCTMXTiledMap *aMap);
    
    MCAStarAlgorithm *createAlgoInstance(MCRoleEntity *aRoleEntity);
protected:
    
private:
    CC_SYNTHESIZE(CCArray *, barriers_, Barriers);
    CC_SYNTHESIZE(CCTMXTiledMap *, map_, Map);
};

#endif /* defined(__Military_Confrontation__MCAStar__) */
