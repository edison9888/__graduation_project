#ifndef __ASTARALGORITHM_H
#define __ASTARALGORITHM_H

#include "MCRole.h"

#include "MCAStarItem_deprecated.h"

class MCAStarAlgorithm_deprecated : public CCObject {
public:
    MCAStarAlgorithm_deprecated();
    ~MCAStarAlgorithm_deprecated();

    bool init(MCRole        *aRole,
              const CCPoint &anEndPoint_,
              unsigned int  *tileSets);

    static MCAStarAlgorithm_deprecated *create(MCRole        *aRole,
                                    const CCPoint &anEndPoint,
                                    unsigned int  *tileSets);

	//calculate 
    int calcG(MCAStarItem_deprecated *anAStarItem);
    int calcH(MCAStarItem_deprecated *anAStarItem);
    int calcF(MCAStarItem_deprecated *anAStarItem);

	//寻路
    CCArray *calcRoute();

	//选取open表中的最末端节点S，加入到close表中
	//void OpentToClose();
	//扩展open表（对S的附近节点T判断）
    void extendOpenList(int);

	//节点是否是障碍点
    bool isNotBarrierNode(int, int);
	//节点是否在战斗区域上（用于NPC寻路）
    bool inPermitArea(int, int);
	//节点是否在非障碍点（用于玩家寻路）


    //排序实现，对open表实现排序
    void sortOpenList(CCArray *&list);

	//判断一个节点是否已存在某表中
    bool contains(CCArray *list, MCAStarItem_deprecated *anAStarItem, MCAStarItem_deprecated *&sameItem);

    //转换成ccpoint序列
    //void getPath();


	 /*未实现****************************/
     void changeTileSets(const unsigned int*);


private:
    CCArray* openList_;
    CCArray* closeList_;
	//最终确定的行走路径
    //CCArray* path_;
	//地图
	//unsigned int* MyTileSets;

    MCAStarItem_deprecated *startPoint_;
    MCAStarItem_deprecated *endPoint_;
};



#endif
