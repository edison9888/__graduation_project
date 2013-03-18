//
//  MCAStar.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-18.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCAStar.h"
//#include "MCOBB.h"
#include "MCRole.h"

const char *kMCAStarDidFinishAlgorithm = "kMCAStarDidFinishAlgorithm";

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
    CC_SAFE_RELEASE(route_);
//    CC_SAFE_RELEASE(openList_);
//    CC_SAFE_RELEASE(closedList_);
}

bool
MCAStarAlgorithm::init(unsigned int *tiles, float layerWidth, const CCPoint &aStartPoint, const CCPoint &anEndPoint)
{
    do {
        tiles_ = tiles;
        layerWidth_ = layerWidth;
        
//        openList_ = CCDictionary::create();
//        CC_BREAK_IF(!openList_);
//        openList_->retain();
//        
//        closedList_ = CCDictionary::create();
//        CC_BREAK_IF(!closedList_);
//        closedList_->retain();
        
        route_ = CCArray::create();
        CC_BREAK_IF(!route_);
        route_->retain();
        
        startPoint_ = MCAStarNode::createWithParent(NULL, aStartPoint.x, aStartPoint.y);
        endPoint_ = MCAStarNode::createWithParent(NULL, anEndPoint.x, anEndPoint.y);
        
        return true;
    } while (0);
    
    return false;
}

MCAStarAlgorithm *
MCAStarAlgorithm::create(unsigned int *tiles, float layerWidth, const CCPoint &aStartPoint, const CCPoint &anEndPoint)
{
    MCAStarAlgorithm *algo = new MCAStarAlgorithm;
    
    if (algo && algo->init(tiles, layerWidth, aStartPoint, anEndPoint)) {
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
    int gid = tiles_[(int) (x + y * layerWidth_)] & kCCFlippedMask;
    
    return gid != 0;
}

//MCAStarItem *
//MCAStarAlgorithm::minFItemAtOpenList()
//{
//    MCAStarItem *item;
//    MCAStarItem *minFItem = NULL;
//    CCDictElement *elem;
//
//    CCDICT_FOREACH(openList_, elem) {
//        item = (MCAStarItem *) elem->getObject();
//        if (minFItem == NULL) {
//            minFItem = item;
//        }
//        if (item->f_ < minFItem->f_) {
//            minFItem = item;
//        }
//    }
//    
//    return minFItem;
//}

MCAStarNode *
MCAStarAlgorithm::minFItemAtList(CCDictionary *aList)
{
    MCAStarNode *item;
    MCAStarNode *minFItem = NULL;
    CCDictElement *elem;
    
    CCDICT_FOREACH(aList, elem) {
        item = (MCAStarNode *) elem->getObject();
        if (minFItem == NULL) {
            minFItem = item;
        }
        if (item->f_ < minFItem->f_) {
            minFItem = item;
        }
    }
    
    return minFItem;
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
    mc_dict_key_t key;
    CCDictionary *openList = CCDictionary::create();   /* 开放列表，待检验位置的列表 */
    CCDictionary *closedList = CCDictionary::create(); /* 关闭列表，已经检验过的位置的列表 */
    mc_index_t c = 0;
    
    /* 初始化开放列表 */
    key = MCPositionToDictKey(startPoint_->position_.x,
                              startPoint_->position_.y);
    openList->setObject(startPoint_, key);
    for (;;) {
        ++c;
        /* 开放列表为空,表明已无可以添加的新节点,而已检验的节点中没有终点节点则意味着没有找到路径 */
        if (openList->count() == 0) {
            break;
        }
        CCLog("openlist size: %d",openList->count());
        /* 选取开放列表中有最小的F值的位置为当前位置，并加入到关闭列表中 */
        currentPosition = minFItemAtList(openList);
        /* 当前节点为目标节点，找到目标 */
        if (currentPosition->position_.equals(endPoint_->position_)) {
            break;
        }
        key = MCPositionToDictKey(currentPosition->position_.x,
                                  currentPosition->position_.y);
        closedList->setObject(currentPosition, key);
        openList->removeObjectForKey(key);
        
        /**
         * 检查格仔四周的位置，全部加入到开放列表。
         * 对于每一个相邻位置，将当前位置保存为它们的“父位置”。
         */
        for (mc_index_t i = 0; i < sizeof(__delta) / sizeof(struct __mc_offset); ++i) {
            /* 先检查关闭列表中有没此位置 */
            key = MCPositionToDictKey(currentPosition->position_.x + __delta[i].x,
                                      currentPosition->position_.y + __delta[i].y);
            tempObject = closedList->objectForKey(key);
            
            /* 如果它不可通过或者已经在关闭列表中，略过它。反之如下。 */
            if (tempObject
                || isBarrier(currentPosition->position_.x + __delta[i].x,
                             currentPosition->position_.y + __delta[i].y)) {
                continue;
            }
            
            tempObject = openList->objectForKey(key);
            /**
             * 如果它已经在开启列表中。
             */
            if (tempObject) {
                side = (MCAStarNode *) tempObject;
                /**
                 * 用G值为参考检查新的路径是否更好。更低的G值意味着更好的路径。
                 * 如果是这样，就把这一格的父节点改成当前格，并且重新计算这一格的G和F值。
                 */
                tempNode.position_ = side->position_;
                tempNode.setParent(currentPosition);
                tempNode.manhattanMethod(endPoint_);
//                CCLog("already at open list %d: old: (%.2f %.2f)[%d] - new: (%.2f %.2f)[%d]",
//                      i + 1,
//                      side->position_.x,side->position_.y,side->f_,
//                      tempNode.position_.x,tempNode.position_.y,tempNode.f_);
                
//                int g = currentPosition->g_
//                        + ((abs(side->position_.x - currentPosition->position_.x) == 1
//                            && abs(side->position_.y - currentPosition->position_.y) == 1) ? 14 : 10);
//                CCLog("already at open list %d: (%.2f %.2f)[old: %d][new: %d][%p]",
//                      i + 1,
//                      side->position_.x,side->position_.y,side->g_,g,side->parent_);
//                if (g < side->g_) {
//                    side->setParent(currentPosition);
//                    side->setG(g);
//                    side->manhattanMethod(endPoint_);
//                }
                openList->removeObjectForKey(key);
                closedList->setObject(side, key);
            /**
             * 如果它不在开启列表中，把它添加进去。把当前格作为这一格的父节点。记录这一格的F,G,和H值。
             */
            } else { /* 不在开放列表 */
                side = MCAStarNode::createWithParent(currentPosition,
                                                     __delta[i].x,
                                                     __delta[i].y);
                side->manhattanMethod(endPoint_);
//                CCLog("add to open %d: (%.2f %.2f)[%d][%p]",
//                      i + 1,
//                      side->position_.x,side->position_.y,side->g_,side->parent_);
                
                key = MCPositionToDictKey(side->position_.x,
                                          side->position_.y);
                openList->setObject(side, key);
            }
        }
    }
    
    CCLog("end->parent: %p(%u)", endPoint_->parent_, c);
    key = MCPositionToDictKey(endPoint_->position_.x,
                              endPoint_->position_.y);
    side = (MCAStarNode *) openList->objectForKey(key);
    for (;;) {
        CCPoint *position = new CCPoint(side->position_.x, side->position_.y);
        position->autorelease();
        route_->addObject(position);
        if (side->parent_ == NULL) {
            break;
        }
        side = side->parent_;
    }
    
    /* 反序 */
    route_->reverseObjects();
    /* 发出算法结束的通知 */
    notificatinCenter->postNotification(kMCAStarDidFinishAlgorithm);
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
    CCTMXLayer *metaLayer = map_->layerNamed("meta");
    CCPoint mapOffset = map_->getPosition();
    MCAStarAlgorithm *algo;
    
    startPoint = ccpAdd(startPoint, mapOffset);
    CCLog("%s::start: (%.2f %.2f)", __FILE__+85, startPoint.x, startPoint.y);
    startPoint = tileCoordinateAt(startPoint);
    CCLog("%s::start: (%.2f %.2f)[tileCoordinate]", __FILE__+85, startPoint.x, startPoint.y);
    endPoint = ccpAdd(aDestinationLocation, mapOffset);
    CCLog("%s::end: (%.2f %.2f)", __FILE__+85, endPoint.x, endPoint.y);
    endPoint = tileCoordinateAt(endPoint);
    CCLog("%s::end: (%.2f %.2f)[tileCoordinate]", __FILE__+85, endPoint.x, endPoint.y);
    algo = new MCAStarAlgorithm;
    algo->init(metaLayer->getTiles(),
               metaLayer->getLayerSize().width,
               startPoint,
               endPoint);
    algoInstances_->addObject(algo);
    CCLog("%s::finished: %d",__FILE__+85,algo->retainCount());
//    algo->release();
    notificatinCenter->addObserver(this,
                                   callfuncO_selector(MCAStar::algorithmDidFinish),
                                   kMCAStarDidFinishAlgorithm,
                                   algo);
    notificatinCenter->addObserver(aRole->getEntity(),
                                   callfuncO_selector(MCRoleEntity::findPathDidFinish),
                                   kMCAStarDidFinishAlgorithm,
                                   algo);
    algo->execute();
    
}

CCPoint
MCAStar::tileCoordinateAt(const CCPoint &aMapLocation) const
{
    float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
    CCSize tileSize = map_->getTileSize();
    CCSize scaledTileSize = CCSizeMake(tileSize.width / contentScaleFactor,
                                       tileSize.height / contentScaleFactor);
    
    int x = (int) (aMapLocation.x / scaledTileSize.width);
    int y = map_->getMapSize().height - (int) (aMapLocation.y / scaledTileSize.height) - 1;
    
    return CCPointMake(x, y);
}

/**
 * 算法结束
 */
void
MCAStar::algorithmDidFinish(CCObject *obj)
{
    CCNotificationCenter *notificatinCenter = CCNotificationCenter::sharedNotificationCenter();
    
    notificatinCenter->removeObserver(this, kMCAStarDidFinishAlgorithm);
    algoInstances_->removeObject(obj);
    CCTime c;
    struct cc_timeval tp;
    c.gettimeofdayCocos2d(&tp, NULL);
    CCLog("end: %ld.%ld",tp.tv_sec,tp.tv_usec);
}
