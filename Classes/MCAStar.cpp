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
const char *kMCAStarAlgorithmWillRemoveNotification = "kMCAStarAlgorithmWillRemoveNotification";
static const mc_byte_t kMCBarrier = 1;
static const mc_byte_t kMCStartPoint = 5;
static const mc_byte_t kMCEndPoint = 8;

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
//static int
//less(const CCObject *p1, const CCObject *p2)
//{
//    return dynamic_cast<const MCAStarNode *>(p1)->getF() > dynamic_cast<const MCAStarNode *>(p2)->getF();
//}
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
    CCARRAY_FOREACH(anArray, obj) {
        if (dynamic_cast<MCAStarNode *>(obj)->equals(aPoint)) {
            return dynamic_cast<MCAStarNode *>(obj);
        }
    }
    return NULL;
}

/**
 * 根据父位置创建新的MCAStarItem。
 * 若父位置不为NULL，则dx和dy是相对于父位置的位置偏移。
 * 否则dx和dy是地图上的位置。
 */
MCAStarNode *
MCAStarNode::createWithParent(MCAStarNode *aParent, float dx, float dy)
{
    MCAStarNode *node = new MCAStarNode;
    
    if (node) {
        node->autorelease();
        if (aParent != NULL) {
            node->position_.x = aParent->position_.x + dx;
            node->position_.y = aParent->position_.y + dy;
            node->g_ = aParent->g_ + ((abs(dx) == 1 && abs(dy) == 1) ? 14 : 10);
        } else {
            node->position_.x = dx;
            node->position_.y = dy;
            node->g_ = 0;
        }
        node->parent_ = aParent;
        node->h_ = 0;
        node->f_ = 0;
    }
    
    return node;
}

void
MCAStarNode::manhattanMethod(MCAStarNode *aTargetPosition)
{
    h_ = 10 * (abs(position_.x - aTargetPosition->position_.x)
               + abs(position_.y - aTargetPosition->position_.y));
    f_ = g_ + h_;
}

CCPoint *
MCAStarNode::getPosition() const
{
    return (CCPoint *)&position_;
}

MCAStarNode *
MCAStarNode::getParent()
{
    return parent_;
}

void
MCAStarNode::setParent(MCAStarNode *var)
{
    parent_ = var;
    g_ = var->g_ + ((abs(position_.x - var->position_.x) == 1 && abs(position_.y - var->position_.y) == 1) ? 14 : 10);

}

#pragma mark -
#pragma mark *** MCAStarAlgorithm ***

MCAStarAlgorithm::~MCAStarAlgorithm()
{
    CC_SAFE_DELETE_ARRAY(mapAltas_);
//    CC_SAFE_RELEASE(route_);
}

bool
MCAStarAlgorithm::init(const CCPoint &aStartPoint, const CCPoint &anEndPoint)
{
    do {
        mapAltas_ = NULL;
        
//        route_ = CCArray::create();
//        CC_BREAK_IF(!route_);
//        route_->retain();
        
        startPoint_ = MCAStarNode::createWithParent(NULL, aStartPoint.x, aStartPoint.y);
        endPoint_ = MCAStarNode::createWithParent(NULL, anEndPoint.x, anEndPoint.y);
        
        return true;
    } while (0);
    
    return false;
}

MCAStarAlgorithm *
MCAStarAlgorithm::create(const CCPoint &aStartPoint, const CCPoint &anEndPoint)
{
    MCAStarAlgorithm *algo = new MCAStarAlgorithm;
    
    if (algo && algo->init(aStartPoint, anEndPoint)) {
        algo->autorelease();
    } else {
        CC_SAFE_DELETE(algo);
        algo = NULL;
    }
    
    return algo;
}

void
MCAStarAlgorithm::execute()
{
//    CCScheduler *scheduler = CCDirector::sharedDirector()->getScheduler();
//    scheduler->scheduleSelector(schedule_selector(MCAStarAlgorithm::process), this, 0.0f , 0, 0.0f, false);
    process(NULL);
}

/**
 * 坐标(x,y)是否为障碍
 */
bool
MCAStarAlgorithm::isBarrier(int x, int y)
{
    return mapAltas_[x + y * mapWidth_] == kMCBarrier;
}

bool
MCAStarAlgorithm::isBarrier(const CCPoint &aPoint)
{
    return mapAltas_[(mc_ssize_t) aPoint.x + (mc_ssize_t) aPoint.y * mapWidth_] == kMCBarrier;
}

/**
 * 生成对象适用的变形地图
 */
void
MCAStarAlgorithm::generateMapAltas(MCRole *aRole, const CCSize &aMapSize, CCArray *barriers)
{
    MCOBB obb = aRole->getEntity()->getOBB();
    CCPoint origin = obb.getOrigin(); /* OBB的左下角 */
    mc_ssize_t obbEdge = obb.width > obb.height ? obb.width : obb.height;
    mc_ssize_t offsetX = (mc_ssize_t) origin.x % (mc_ssize_t) obbEdge;
    mc_ssize_t offsetY = (mc_ssize_t) origin.y % (mc_ssize_t) obbEdge;
    CCPoint checkFrameOrigin = ccp(offsetX > 0 ? offsetX - (mc_ssize_t) obbEdge : 0,
                             offsetY > 0 ? offsetY - (mc_ssize_t) obbEdge : 0);
    CCRect checkFrame = CCRectMake(checkFrameOrigin.x,
                                   checkFrameOrigin.y,
                                   obbEdge,
                                   obbEdge); /* 第一个检测矩形 */
    CCRectLog(checkFrame);
    mc_ssize_t width = (mc_ssize_t) (aMapSize.width / obbEdge) + 1;
    mc_ssize_t height = (mc_ssize_t) (aMapSize.height / obbEdge) + 1;
    mapAltas_ = new mc_byte_t[width * height];
    CCObject *obj;
    MCBarrier *barrier;
    mc_ssize_t collided;
    
    /* 生成 */
    mapWidth_ = width;
    mapHeight_ = height;
    edge_ = obbEdge;
    startPoint_->position_.x = (float) ((mc_ssize_t) startPoint_->position_.x / (mc_ssize_t) obbEdge);
    startPoint_->position_.y = (float) ((mc_ssize_t) startPoint_->position_.y / (mc_ssize_t) obbEdge);
    endPoint_->position_.x = (float) ((mc_ssize_t) endPoint_->position_.x / (mc_ssize_t) obbEdge);
    endPoint_->position_.y = (float) ((mc_ssize_t) endPoint_->position_.y / (mc_ssize_t) obbEdge);
    for (mc_ssize_t y = 0; y < height; ++y) {
        for (mc_ssize_t x = 0; x < width; ++x) {
            collided = 0;
            CCARRAY_FOREACH(barriers, obj) {
                barrier = dynamic_cast<MCBarrier *>(obj);
                CCRect aabb = barrier->getOBB().getAABB();
                if (barrier->getOBB().getAABB().intersectsRect(checkFrame)) {
                    collided = kMCBarrier;
                    break;
                }
            }
            mapAltas_[x + y * width] = collided;
//            printf("%ld ", collided);
            checkFrame.origin.x += obbEdge;
        }
//        printf("\n");
        checkFrame.origin.x = checkFrameOrigin.x;
        checkFrame.origin.y += obbEdge;
    }
    
    mapAltas_[(mc_ssize_t) startPoint_->position_.x + (mc_ssize_t) startPoint_->position_.y * width] += kMCStartPoint;
    mapAltas_[(mc_ssize_t) endPoint_->position_.x + (mc_ssize_t) endPoint_->position_.y * width] += kMCEndPoint;
    for (mc_ssize_t y = height - 1; y >= 0; --y) {
        for (mc_ssize_t x = 0; x < width; ++x) {
            printf("%d ", mapAltas_[x + y * width]);
        }
        printf("\n");
    }
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
void
MCAStarAlgorithm::process(CCObject *obj)
{
    CCNotificationCenter *notificatinCenter = CCNotificationCenter::sharedNotificationCenter();
    MCAStarNode *currentPosition;
    MCAStarNode *side;
    MCAStarNode tempNode;
    CCObject *tempObject;
    CCArray *openList = CCArray::create();   /* 开放列表，待检验位置的列表 */
    CCArray *closedList = CCArray::create(); /* 关闭列表，已经检验过的位置的列表 */
    mc_index_t c = 0;
    
    /* 初始化开放列表 */
    CCLog("start-altas(%.0f %.0f) end-altas(%.0f %.0f)",
          startPoint_->position_.x, startPoint_->position_.y,
          endPoint_->position_.x, endPoint_->position_.y);
    openList->addObject(startPoint_);
    for (;;) {
        ++c;
        if (c > 100000) {
            break;
        }
        /* 开放列表为空,表明已无可以添加的新节点,而已检验的节点中没有终点节点则意味着没有找到路径 */
        if (openList->count() == 0) {
            CCLog("虾米都没有！");
            break;
        }
        /* 选取开放列表中有最小的F值的位置为当前位置，并加入到关闭列表中 */
        currentPosition = ccArrayGetAStarNodeWithMinF(openList->data);
        printf("(%.0f %.0f)[%d] => ", currentPosition->position_.x,currentPosition->position_.y, currentPosition->getF());
        /* 当前节点为目标节点，找到目标 */
        if (mapAltas_[(mc_ssize_t) currentPosition->position_.x
                        + (mc_ssize_t) currentPosition->position_.y * mapWidth_] == kMCEndPoint) {
            endPoint_ = currentPosition;
            CCLog("有了");
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
            CCPoint checkPoint = ccp(currentPosition->position_.x + __delta[i].x,
                                     currentPosition->position_.y + __delta[i].y);
            if (checkPoint.x < 0 || checkPoint.x > mapWidth_
                || checkPoint.y < 0 || checkPoint.y > mapHeight_) {
                continue;
            }
            tempObject = CCArrayGetAStarNodeByPoint(closedList, checkPoint);
            
            /* 如果它不可通过或者已经在关闭列表中，略过它。反之如下。 */
            if (tempObject || isBarrier(checkPoint)) {
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
                tempNode.position_ = side->position_;
                tempNode.setParent(currentPosition);
                int g = currentPosition->g_
                        + ((abs(side->position_.x - currentPosition->position_.x) == 1
                            && abs(side->position_.y - currentPosition->position_.y) == 1) ? 14 : 10);
                if (g < side->g_) {
                    side->setParent(currentPosition);
                    side->setG(g);
                    side->manhattanMethod(endPoint_);
                }
                if (tempNode.g_ < side->g_) {
                    side->g_ = tempNode.g_;
                    side->setParent(currentPosition);
                    side->manhattanMethod(endPoint_);
                }
            /**
             * 如果它不在开启列表中，把它添加进去。把当前格作为这一格的父节点。记录这一格的F,G,和H值。
             */
            } else { /* 不在开放列表 */
                side = MCAStarNode::createWithParent(currentPosition,
                                                     __delta[i].x,
                                                     __delta[i].y);
                side->manhattanMethod(endPoint_);
                openList->addObject(side);
            }
        }
    }
    
    CCLog("end at %dth", c);
    side = CCArrayGetAStarNodeByPoint(openList, endPoint_->position_);
    for (;;) {
        CCPoint target = ccp(side->position_.x * edge_, side->position_.y * edge_);
        route.push(ccpSub(target, mapOffset_));
        if (side->parent_ != NULL && side->parent_->parent_ == NULL) {
            break;
        }
        side = side->parent_;
    }
    
    /* 不需要反序了，直接当做栈处理 */
    /* 发出算法结束的通知 */
    notificatinCenter->postNotification(kMCAStarDidFinishAlgorithmNotification);
}

#pragma mark *** MCAStar ***

MCAStar::MCAStar()
{
}

MCAStar::~MCAStar()
{
    CC_SAFE_RELEASE(algoInstances_);
}

bool
MCAStar::init(CCTMXTiledMap *aMap)
{
    if (aMap) {
        algoInstances_ = CCArray::create();
        algoInstances_->retain();
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

void
MCAStar::findPath(MCRole *aRole, const CCPoint &aDestinationLocation)
{
    CCNotificationCenter *notificatinCenter = CCNotificationCenter::sharedNotificationCenter();
    MCOBB obb = aRole->getEntity()->getOBB();
    CCPoint startPoint = obb.getOrigin();
    CCPoint endPoint;
    CCPoint mapOffset = map_->getPosition();
    MCAStarAlgorithm *algo;
    CCSize mapSize = map_->getMapSize();
    CCSize tileSize = map_->getTileSize();
    CCSize mapRealSize = CCSizeMake(mapSize.width * tileSize.width,
                                    mapSize.height * tileSize.height);
    
    startPoint = ccpAdd(startPoint, mapOffset);
    endPoint = ccpAdd(aDestinationLocation, mapOffset);
    CCLog("start(%.0f %.0f) end(%.0f %.0f)",
          startPoint.x, startPoint.y,
          endPoint.x, endPoint.y);
    algo = new MCAStarAlgorithm;
    algo->init(startPoint, endPoint);
    algo->generateMapAltas(aRole, mapRealSize, barriers_);
    algo->mapOffset_ = mapOffset;
    algoInstances_->addObject(algo);
    notificatinCenter->addObserver(this,
                                   callfuncO_selector(MCAStar::algorithmWillRemove),
                                   kMCAStarAlgorithmWillRemoveNotification,
                                   algo);
    notificatinCenter->addObserver(aRole->getEntity(),
                                   callfuncO_selector(MCRoleEntity::findPathDidFinish),
                                   kMCAStarDidFinishAlgorithmNotification,
                                   algo);
    algo->execute();
    
}

/**
 * 算法结束
 */
void
MCAStar::algorithmWillRemove(CCObject *obj)
{
    CCNotificationCenter *notificatinCenter = CCNotificationCenter::sharedNotificationCenter();
    
    notificatinCenter->removeObserver(this, kMCAStarDidFinishAlgorithmNotification);
    algoInstances_->removeObject(obj);
}
