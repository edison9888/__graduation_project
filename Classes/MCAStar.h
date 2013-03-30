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
#include "MCGeometry.h"

extern const char *kMCAStarDidFinishAlgorithmNotification;

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
    static MCAStarNode *createWithParent(MCAStarNode *aParent, float dx, float dy);
    
    void manhattanMethod(MCAStarNode *aTargetPosition);
    
    CCPoint *getPosition() const;
    
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
    
private:
    CCPoint position_;
    
	CC_SYNTHESIZE(int, g_, G);
	CC_SYNTHESIZE(int, h_, H);
	CC_SYNTHESIZE(int, f_, F);
    CC_PROPERTY(MCAStarNode *, parent_, Parent);
};

#define MCPositionToDictKey(x, y) \
    (0 | ((mc_dict_key_t)(x)) * 0x1000000 \
       | ((mc_dict_key_t)(y)) * 0x100) \

class MCAStarAlgorithm : public CCObject {
    friend class MCAStar;
public:
    ~MCAStarAlgorithm();
    
    bool init(unsigned int *tiles, float layerWidth, const CCPoint &aStartPoint, const CCPoint &anEndPoint);
    
    static MCAStarAlgorithm *create(unsigned int *tiles, float layerWidth, const CCPoint &aStartPoint, const CCPoint &anEndPoint);
    
    void execute();
    
protected:
    MCAStarNode *minFItemAtList(CCDictionary *aList);
    
    /**
     * 坐标(x,y)是否为障碍 
     */
    bool isBarrier(int x, int y);
    bool isBarrier(const CCPoint &aPoint);
    
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
    void process(CCObject *obj);
    
    inline void setMetaLayer(CCTMXLayer *metaLayer) {
        metaLayer_ = metaLayer;
    }
    
private:
    unsigned int *tiles_;
    float layerWidth_;
    MCAStarNode *startPoint_;
    MCAStarNode *endPoint_;
    
    CCTMXLayer *metaLayer_;
    
    CC_SYNTHESIZE(CCArray *, route_, Route); /* 寻路结果 */
};

class MCAStar : public CCObject {
public:
    MCAStar();
    ~MCAStar();
    
    bool init(CCTMXTiledMap *aMap);
    
    static MCAStar *create(CCTMXTiledMap *aMap);
    
    void findPath(MCRole *aRole, const CCPoint &aDestinationLocation);
    
protected:
    CCPoint tileCoordinateAt(const CCPoint &aMapLocation) const;
    
    /**
     * 算法结束
     */
    void algorithmDidFinish(CCObject *obj);
    
    
private:
    CCArray *algoInstances_; /* 算法实例，算法执行完毕后会自动删除 */

    CC_SYNTHESIZE(CCTMXTiledMap *, map_, Map);
};

#endif /* defined(__Military_Confrontation__MCAStar__) */
