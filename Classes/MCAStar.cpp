//
//  MCAStar.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCAStar.h"
#include "MCRole.h"
#include <bits/stl_algo.h>

const char *kMCAStarDidFinishAlgorithmNotification = "kMCAStarDidFinishAlgorithmNotification";

const mc_byte_t kMCBarrier = MCMakeEnum(0);
//const mc_byte_t kMCStartPoint = MCMakeEnum(1);
//const mc_byte_t kMCEndPoint = MCMakeEnum(2);

struct __mc_offset {
    int x;
    int y;
};

static struct __mc_offset __delta[] = {
    {-1, -1},
    {0, -1},
    {1, -1},
    
    {-1, 0},
    {1, 0},
    
    {-1, 1},
    {0, 1},
    {1, 1}
};

/**
 * Used for sort
 */
static int
less(const void *p1, const void *p2)
{
    return ((MCAStarNode *) p1)->getF() < ((MCAStarNode *) p2)->getF();
}

static void
ccArraySortByF(ccArray *anArray)
{
    std::sort(anArray->arr, anArray->arr + anArray->num, less);
}

static inline MCAStarNode *
ccArrayGetAStarNodeWithMinF(ccArray *anArray)
{
    ccArraySortByF(anArray);
    
    return dynamic_cast<MCAStarNode *>(anArray->arr[0]);
}

static inline void
CCArrayRemoveAStarNodeByPoint(CCArray *anArray, const CCPoint &aPoint)
{
    CCObject *obj;
    CCARRAY_FOREACH(anArray, obj) {
        if (dynamic_cast<MCAStarNode *>(obj)->equals(aPoint)) {
            anArray->removeObject(obj);
        }
    }
}

static inline MCAStarNode *
CCArrayGetAStarNodeByPoint(CCArray *anArray, const CCPoint &aPoint)
{
    CCObject *obj;
    int c=0;
    int s = anArray->count();
    CC_UNUSED_PARAM((s));
    CCARRAY_FOREACH(anArray, obj) {
        if (obj && dynamic_cast<MCAStarNode *>(obj)->equals(aPoint)) {
            return dynamic_cast<MCAStarNode *>(obj);
        }
        c++;
    }
    return NULL;
}

/**
 * 算法执行过程
 */
/*
 * A*方法总结
 * 1. 把起始格添加到开启列表。
 * 2. 重复如下的工作：
 * a) 寻找开启列表中F值最低的格子。我们称它为当前格。
 * b) 把它切换到关闭列表。
 * c) 对相邻的8格中的每一个？
 *    * 如果它不可通过或者已经在关闭列表中，略过它。反之如下。
 *    * 如果它不在开启列表中，把它添加进去。把当前格作为这一格的父节点。记录这一格的F,G,和H值。
 *    * 如果它已经在开启列表中，用G值为参考检查新的路径是否更好。更低的G值意味着更好的路径。
 * 如果是这样，就把这一格的父节点改成当前格，并且重新计算这一格的G和F值。
 * 如果你保持你的开启列表按F值排序，改变之后你可能需要重新对开启列表排序。
 * d) 停止，当你
 *    * 把目标格添加进了关闭列表(注解)，这时候路径被找到，或者
 *    * 没有找到目标格，开启列表已经空了。这时候，路径不存在。
 * 3. 保存路径。从目标格开始，沿着每一格的父节点移动直到回到起始格。这就是你的路径。
 */
static void *
mc_astar_algorithm_process(void *obj)
{
    MCAStarAlgorithm *algo = (MCAStarAlgorithm *) obj;
    std::stack<CCPoint> &route = algo->route;
#if MC_DEBUG_ASTAR == 1
    mc_byte_t *mapAltas = algo->getMapAltas();
#endif
    mc_size_t edge = algo->getEdge();
    MCAStarNode *startPoint = algo->getStartPoint();
    MCAStarNode *endPoint = algo->getEndPoint();
    MCAStarNode *currentPosition;
    MCAStarNode *side;
    MCAStarNode tempNode;
    CCObject *tempObject;
    mc_size_t mapWidth = algo->getMapWidth();
    mc_size_t mapHeight = algo->getMapHeight();
    CCArray *openList = new CCArray;   /* 开放列表，待检验位置的列表 */
    CCArray *closedList = new CCArray; /* 关闭列表，已经检验过的位置的列表 */
    CCPoint currentPositionOrigin;
    CCPoint destinationPosition = endPoint->getPosition();
    
    openList->init();
    closedList->init();
//    CCLog("start-altas(%d %d) end-altas(%d %d)",
//          startPoint->getX() * edge, startPoint->getY() * edge,
//          endPoint->getX() * edge, endPoint->getY() * edge);
//    CCLog("start-altas(%d %d) end-altas(%d %d)",
//          startPoint->getX(), startPoint->getY(),
//          endPoint->getX(), endPoint->getY());
    /* 初始化开放列表 */
    /* 至少终点不能是障碍吧~ */
    if (! algo->isBarrier(destinationPosition)) {
        openList->addObject(startPoint);
    }
    for (;algo->isProcessing();) {
        /* 开放列表为空,表明已无可以添加的新节点,而已检验的节点中没有终点节点则意味着没有找到路径 */
        if (openList->count() == 0) {
//            CCLog("虾米都没有！");
            break;
        }
        /* 选取开放列表中有最小的F值的位置为当前位置，并加入到关闭列表中 */
        currentPosition = ccArrayGetAStarNodeWithMinF(openList->data);
        currentPositionOrigin = currentPosition->getPosition();
        /* 当前节点为目标节点，找到目标 */
        if (currentPositionOrigin.x == destinationPosition.x
            && currentPositionOrigin.y == destinationPosition.y) {
            endPoint = currentPosition;
//            CCLog("有了");
            break;
        }
        closedList->addObject(currentPosition);
        openList->removeObject(currentPosition);
        
        /**
         * 检查格仔四周的位置，全部加入到开放列表。
         * 对于每一个相邻位置，将当前位置保存为它们的“父位置”。
         */
        for (mc_index_t i = 0; i < 8; ++i) {
            /* 先检查关闭列表中有没此位置 */
            CCPoint checkPoint = ccp(currentPositionOrigin.x + __delta[i].x,
                                     currentPositionOrigin.y + __delta[i].y);
            if (checkPoint.x < 0 || checkPoint.x > mapWidth
                || checkPoint.y < 0 || checkPoint.y > mapHeight) {
                continue;
            }
            tempObject = CCArrayGetAStarNodeByPoint(closedList, checkPoint);
            
            /* 如果它不可通过或者已经在关闭列表中，略过它。反之如下。 */
            if (tempObject || algo->isBarrier(checkPoint)) {
                continue;
            }
            
            tempObject = CCArrayGetAStarNodeByPoint(openList, checkPoint);
            /**
             * 如果它已经在开启列表中。
             */
            if (tempObject) {
                side = dynamic_cast<MCAStarNode *>(tempObject);
                /**
                 * 用G值为参考检查新的路径是否更好。更低的G值意味着更好的路径。
                 * 如果是这样，就把这一格的父节点改成当前格，并且重新计算这一格的G和F值。
                 */
                tempNode.setPosition(side->getPosition());
                tempNode.setParent(currentPosition);
                int g = currentPosition->getG()
                        + ((abs(side->getX() - currentPosition->getX()) == 1
                            && abs(side->getY() - currentPosition->getY()) == 1) ? 14 : 10);
                if (g < side->getG()) {
                    side->setParent(currentPosition);
                    side->setG(g);
                    side->manhattanMethod(endPoint);
                }
                if (tempNode.getG() < side->getG()) {
                    side->setG(tempNode.getG());
                    side->setParent(currentPosition);
                    side->manhattanMethod(endPoint);
                }
                /**
                 * 如果它不在开启列表中，把它添加进去。把当前格作为这一格的父节点。记录这一格的F,G,和H值。
                 */
            } else { /* 不在开放列表 */
                side = new MCAStarNode;
                side->setup(currentPosition,
                            __delta[i].x,
                            __delta[i].y);
                side->manhattanMethod(endPoint);
                openList->addObject(side);
                side->release();
            }
        }
    }
    
    side = CCArrayGetAStarNodeByPoint(openList, endPoint->getPosition());
    if (side != NULL) {
#if MC_DEBUG_ASTAR == 1
        mapAltas[startPoint->getX() + startPoint->getY() * mapWidth] = 3;
        mapAltas[endPoint->getX() + endPoint->getY() * mapWidth] = 7;
#endif
        for (;;) {
            CCPoint target = ccp(side->getX() * edge, side->getY() * edge);
            route.push(target);
#if MC_DEBUG_ASTAR == 1
            mapAltas[(mc_ssize_t) side->getX() + (mc_ssize_t) side->getY() * mapWidth] = 4;
#endif
            
            if (side->getParent() == NULL
                || (side->getParent() != NULL && side->getParent()->getParent() == NULL)) {
                break;
            }
            side = side->getParent();
        }
    }
#if MC_DEBUG_ASTAR == 1
        CCLog("start-altas(%.0f %.0f) end-altas(%.0f %.0f)",
              startPoint->getX() * (float) edge, startPoint->getY() * (float) edge,
              endPoint->getX() * (float) edge, endPoint->getY() * (float) edge);
        
        for (mc_ssize_t y = mapWidth - 1; y >= 0; --y) {
            for (mc_ssize_t x = 0; x < mapWidth; ++x) {
                printf("%d ", mapAltas[x + y * mapWidth]);
            }
            printf("\n");
        }
    mapAltas[startPoint->getX() + startPoint->getY() * mapWidth] = 0;
    mapAltas[endPoint->getX() + endPoint->getY() * mapWidth] = 0;
#endif
    CC_SAFE_RELEASE(openList);
    CC_SAFE_RELEASE(closedList);
    
    /* 发出算法结束的通知 */
    if (algo->isProcessing()) {
        algo->notifyPathFindingDidFinish();
    }
    
    return NULL;
}

#pragma mark -
#pragma mark *** MCAStarNode ***

/**
 * 根据父位置创建新的MCAStarItem。
 * 若父位置不为NULL，则dx和dy是相对于父位置的位置偏移。
 * 否则dx和dy是地图上的位置。
 */
void
MCAStarNode::setup(MCAStarNode *aParent, float dx, float dy)
{
    if (aParent != NULL) {
        position_.x = aParent->position_.x + dx;
        position_.y = aParent->position_.y + dy;
        g_ = aParent->g_ + ((abs(dx) == 1 && abs(dy) == 1) ? 14 : 10);
    } else {
        position_.x = dx;
        position_.y = dy;
        g_ = 0;
    }
    parent_ = aParent;
    h_ = 0;
    f_ = 0;
}

void
MCAStarNode::manhattanMethod(MCAStarNode *aTargetPosition)
{
    h_ = 10 * (abs(position_.x - aTargetPosition->position_.x)
               + abs(position_.y - aTargetPosition->position_.y));
    f_ = g_ + h_;
}

void
MCAStarNode::setParent(MCAStarNode *aParent)
{
    parent_ = aParent;
    g_ = aParent->g_ + ((abs(position_.x - aParent->position_.x) == 1 && abs(position_.y - aParent->position_.y) == 1) ? 14 : 10);

}

#pragma mark -
#pragma mark *** MCAStarAlgorithm ***

MCAStarAlgorithm::~MCAStarAlgorithm()
{
    CC_SAFE_RELEASE(startPoint_);
    CC_SAFE_RELEASE(endPoint_);
    CC_SAFE_DELETE_ARRAY(mapAltas_);
}

bool
MCAStarAlgorithm::init()
{
    do {
        mapAltas_ = NULL;
        
        startPoint_ = new MCAStarNode;
        endPoint_ = new MCAStarNode;
        
        processing_ = false;
        
        return true;
    } while (0);
    
    return false;
}

void
MCAStarAlgorithm::setDestination(const CCPoint &aDestinationLocation)
{
    CCPoint roleOrigin = roleEntity_->getOBB().getOrigin();
    startPoint_->setup(NULL,
                       (float) ((mc_size_t) roleOrigin.x / (mc_size_t) edge_),
                       (float) ((mc_size_t) roleOrigin.y / (mc_size_t) edge_));
    endPoint_->setup(NULL,
                     (float) ((mc_size_t) aDestinationLocation.x / (mc_size_t) edge_),
                     (float) ((mc_size_t) aDestinationLocation.y / (mc_size_t) edge_));
}

bool
MCAStarAlgorithm::testPosition(const CCPoint &aDestinationLocation)
{
    return isBarrier(aDestinationLocation);
}

void
MCAStarAlgorithm::stopPathFinding() {
    processing_ = false;
    
    /* 貌似很快就找到了，完全没需要停止线程 */
    if (pid_) {
        pthread_join(pid_, NULL);
        pid_ = 0;
    }
    
    while (route.size() > 0) {
        route.pop();
    }
}

void
MCAStarAlgorithm::execute()
{
    processing_ = true;
    
    /* 创建线程 */
    pthread_attr_t pthreadAttr;
    int errCode = pthread_attr_init(&pthreadAttr);
    if (errCode != 0) {
        processing_ = false;
        perror("线程创建失败！");
        return;
    }
    
    errCode = pthread_attr_setdetachstate(&pthreadAttr, PTHREAD_CREATE_JOINABLE);
    if (errCode != 0) {
        pthread_attr_destroy(&pthreadAttr);
        processing_ = false;
        perror("线程创建失败！");
        return;
    }
    
    errCode = pthread_create(&pid_, &pthreadAttr, mc_astar_algorithm_process, this);
    if (errCode != 0) {
        processing_ = false;
        perror("线程创建失败！");
        return;
    }
}

/**
 * 生成对象适用的变形地图
 */
void
MCAStarAlgorithm::generateMapAltas(MCRoleEntity *aRoleEntity, const CCSize &aMapSize, CCArray *barriers)
{
    MCOBB obb = aRoleEntity->getOBB();
    mc_ssize_t obbEdge = obb.width; /* width=height */
    CCPoint checkFrameOrigin = CCPointZero;
    CCRect checkFrame = CCRectMake(checkFrameOrigin.x,
                                   checkFrameOrigin.y,
                                   obbEdge,
                                   obbEdge); /* 第一个检测矩形 */
    mc_ssize_t width = (mc_ssize_t) (aMapSize.width / obbEdge) + 1;
    mc_ssize_t height = (mc_ssize_t) (aMapSize.height / obbEdge) + 1;
    mapAltas_ = new mc_byte_t[width * height];
    CCObject *obj;
    MCBarrier *barrier;
    mc_ssize_t nodeType;
    
    /* 生成 */
    mapWidth_ = width;
    mapHeight_ = height;
    edge_ = obbEdge;
    for (mc_ssize_t y = 0; y < height; ++y) {
        for (mc_ssize_t x = 0; x < width; ++x) {
            nodeType = 0;
            CCARRAY_FOREACH(barriers, obj) {
                barrier = dynamic_cast<MCBarrier *>(obj);
#if (MC_COLLISION_USE_OBB == 1)
                CCRect aabb = barrier->getOBB().getAABB();
#else
                CCRect aabb = barrier->getFrame();
#endif
                if (aabb.intersectsRect(checkFrame)) {
                    nodeType = kMCBarrier;
                    break;
                }
            }
            mapAltas_[x + y * width] = nodeType;
            checkFrame.origin.x += obbEdge;
        }
        checkFrame.origin.x = checkFrameOrigin.x;
        checkFrame.origin.y += obbEdge;
    }
}

void
MCAStarAlgorithm::pathFindingDidFinish(CCObject *anObject)
{
    processing_ = false;
    pid_ = 0;
    roleEntity_->pathFindingDidFinish(this);
}

#pragma mark *** MCAStar ***

MCAStar::MCAStar()
{
}

MCAStar::~MCAStar()
{
}

bool
MCAStar::init(CCTMXTiledMap *aMap)
{
    if (aMap) {
        map_ = aMap;
        
        return true;
    }
    
    return false;
}

MCAStar *
MCAStar::create(CCTMXTiledMap *aMap)
{
    MCAStar *aStar = new MCAStar;
    
    if (aStar && aStar->init(aMap)) {
        aStar->autorelease();
    } else {
        CC_SAFE_DELETE(aStar);
        aStar = NULL;
    }
    
    return aStar;
}

MCAStarAlgorithm *
MCAStar::createAlgoInstance(MCRoleEntity *aRoleEntity)
{
    MCAStarAlgorithm *algo;
    CCSize mapSize = map_->getMapSize();
    CCSize tileSize = map_->getTileSize();
    float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
    CCSize mapRealSize = CCSizeMake(mapSize.width * tileSize.width / contentScaleFactor,
                                    mapSize.height * tileSize.height / contentScaleFactor);
    
    algo = new MCAStarAlgorithm;
    algo->init();
    algo->roleEntity_ = aRoleEntity;
    algo->generateMapAltas(aRoleEntity, mapRealSize, barriers_);
    
    return algo;
}
