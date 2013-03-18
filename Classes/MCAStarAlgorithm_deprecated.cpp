#include "MCAStarAlgorithm_deprecated.h"

const int kMCMapWidth = 50;
//获取地图的宽

MCAStarAlgorithm_deprecated::MCAStarAlgorithm_deprecated()
{


}

MCAStarAlgorithm_deprecated::~MCAStarAlgorithm_deprecated()
{
    CC_SAFE_RELEASE(openList_);
    CC_SAFE_RELEASE(closeList_);
//    CC_SAFE_RELEASE(path_);
}

bool
MCAStarAlgorithm_deprecated::init(MCRole        *aRole,
                       const CCPoint &anEndPoint,
                       unsigned int  *tileSets)
{
    openList_ = CCArray::create();
    closeList_ = CCArray::create();
//    path_ = CCArray::create();
    openList_->retain();
    closeList_->retain();
//    path_->retain();

    MCOBB obb = aRole->getEntity()->getOBB();
    CCPoint anStartPoint = ccp(obb.center.x - obb.extents.width,
                               obb.center.y - obb.extents.height);//???????
//    CCSize size = aRole->getEntity()->getAABB().size;//需改动

    //设置起点
    startPoint_ = new MCAStarItem_deprecated;
    startPoint_->col_ = anStartPoint.x;
    startPoint_->row_ = anStartPoint.y;
    startPoint_->f_ = 0;
    //设置终点
    endPoint_ = new MCAStarItem_deprecated;
    endPoint_->col_ = anEndPoint.x;
    endPoint_->row_ = anEndPoint.y;

    return true;
}

MCAStarAlgorithm_deprecated *
MCAStarAlgorithm_deprecated::create(MCRole        *aRole,
                         const CCPoint &anendPoint_,
                         unsigned int  *tileSets)
{
    MCAStarAlgorithm_deprecated *algo = new MCAStarAlgorithm_deprecated;

    if (algo && algo->init(aRole, anendPoint_, tileSets)) {
        algo->autorelease();
    } else {
        CC_SAFE_DELETE(algo);
        algo = NULL;
    }

    return algo;
}

CCArray *
MCAStarAlgorithm_deprecated::calcRoute()
 {
 	//将S节点放入open表中
    openList_->addObject(startPoint_);
	//open表为空？
    while (0 != openList_->count()) {
		//选取open表中的最末端节点S，加入到close表中
        closeList_->addObject(openList_->lastObject());
        openList_->removeLastObject();
        //
        int fid = closeList_->count();
		//close表末端节点S是否为目标节点
        if ((((MCAStarItem_deprecated *) closeList_->lastObject())->col_ == endPoint_->col_) &&
            (((MCAStarItem_deprecated *) closeList_->lastObject())->row_ == endPoint_->row_)) {
			//完成
            //求得路径path_，并返回。。。。。。。。未求

            return closeList_;
        } else {
			//扩展open表（对S的附近节点T判断）
            extendOpenList(fid);
			//排序：选择F值最小且p_parent指向S的节点T排在open表最末端
            sortOpenList(closeList_);
		}
	}
	//失败
	return false;
 }


int
MCAStarAlgorithm_deprecated::calcG(MCAStarItem_deprecated *anAStarItem)
{

    int fid = anAStarItem->parent_;
    int fx = ((MCAStarItem_deprecated*)closeList_->objectAtIndex(fid))->getCol();
    int fy = ((MCAStarItem_deprecated*)closeList_->objectAtIndex(fid))->getRow();
    int fg = ((MCAStarItem_deprecated*)closeList_->objectAtIndex(fid))->getG();

    if(fx != anAStarItem->col_ && fy != anAStarItem->row_)
	{
        return (fg + 14);
	}
	else
	{
        return (fg + 10);
	}
}

int
MCAStarAlgorithm_deprecated::calcH(MCAStarItem_deprecated *anAStarItem)
{

    return (abs(endPoint_->row_ - anAStarItem->row_) * 10
            + abs(endPoint_->col_ - anAStarItem->col_) * 10);
}

int
MCAStarAlgorithm_deprecated::calcF(MCAStarItem_deprecated *anAStarItem)
{
    return (anAStarItem->g_ + anAStarItem->h_);
}

void
MCAStarAlgorithm_deprecated::extendOpenList(int fid)
{
    //创建T节点（insertItem）
    MCAStarItem_deprecated *insertItem = new MCAStarItem_deprecated;
    insertItem->autorelease();
    
    int partrialX = -1;
    int partrialY = -1;
    int col = ((MCAStarItem_deprecated*)closeList_->objectAtIndex(fid - 1))->getCol();
    int row = ((MCAStarItem_deprecated*)closeList_->objectAtIndex(fid - 1))->getRow();
    for(; partrialX <= 1; ++partrialX) {
        for(; partrialY <= 1; ++partrialY) {
            if(partrialX == 0 && partrialY == 0) {
                continue;
            }
                // 是否为障碍点 是否在地图范围内
            if (isNotBarrierNode(col + partrialX, row + partrialY)  &&
                inPermitArea(col + partrialX, row + partrialY)) {
                insertItem->setPosition(col + partrialX , row + partrialY);
                
                
                
                MCAStarItem_deprecated *sameItem = NULL;//一个元素不会既存在于open表与close表中
                bool inCloseList_ = contains(closeList_, insertItem, sameItem);
                bool inOpenList_ = contains(openList_, insertItem, sameItem);
                
                insertItem->parent_ = closeList_->count()-1;
                
                if ((!inCloseList_) && (!inOpenList_)) { //不在open表和close表中
                        //T的p.g.h.f
                    
                    insertItem->f_ = calcF(insertItem);
                    insertItem->g_ = calcG(insertItem);
                    insertItem->h_ = calcH(insertItem);
                        //将T加入open表
                    openList_->addObject(insertItem);
                }
                else if(inOpenList_) { //在open表中
                    if(insertItem->g_ < sameItem->g_) {
                            //查找，修改g,f值和p值。
                        MCAStarItem_deprecated* tmp = ((MCAStarItem_deprecated*)openList_->objectAtIndex(sameItem->f_));
                        tmp->parent_ = insertItem->parent_;
                        tmp->g_ = insertItem->g_;
                        tmp->f_ = calcF(tmp);
                        
                    }
                }
                else { //在close表中
//                    if(insertItem->g_ < sameItem->g_) {
//                            //查找，修改g,f值和p值。
//                        MCAStarItem_deprecated* tmp = ((MCAStarItem_deprecated*)closeList_->objectAtIndex(sameItem->f_));
//                        tmp->parent_ = insertItem->parent_;//?
//                        tmp->g_ = insertItem->g_;
//                        tmp->f_ = calcF(tmp);
//                    }
                }
            }
        }
    }
}


//先跳过判断这，等待测试。应该加not
bool
MCAStarAlgorithm_deprecated::isNotBarrierNode(int x, int y)
{
//    if(MyTileSets[ x + y * weight ] == 0) {//暂时设定，待具体扩充，障碍等级！
//        return false;
//    } else {
//        return true;
//    }
    return true;
}

bool
MCAStarAlgorithm_deprecated::inPermitArea(int x, int y)
{
//    if(x < 1 || y < 1) { //待扩充怪物的战斗区域
//        return false;
//    } else {
//        return true;
//    }
    return true;
} 
//简单选择排序
void
MCAStarAlgorithm_deprecated::sortOpenList(CCArray *&list)
{

    //MCAStarItem_deprecated *tmp = NULL;

    unsigned int i;
    unsigned int j;
    unsigned int k;

    for (i = 1;i <= list->count(); ++i){

        k = i;
        for(j = i+1; j <= list->count(); ++j){
            if( ((MCAStarItem_deprecated*)list->objectAtIndex(j))->f_ > ((MCAStarItem_deprecated*)list->objectAtIndex(k))->f_ )
            {k = j;}
        }
        //swap
        //复制构造函数
        MCAStarItem_deprecated tmp(  *((MCAStarItem_deprecated*)list->objectAtIndex(i - 1))  );
        list->exchangeObjectAtIndex(i - 1,k);
        list->replaceObjectAtIndex(k,&tmp);

       // tmp = i;
       // i = k;
       // k = tmp;



    }

}
//*?
bool
MCAStarAlgorithm_deprecated::contains(CCArray *list, MCAStarItem_deprecated *anAStarItem, MCAStarItem_deprecated *&sameItem)
{
	CCObject* obj = NULL;
    CCARRAY_FOREACH(list, obj) {
        if (((MCAStarItem_deprecated *) obj)->col_ == anAStarItem->col_
                && ((MCAStarItem_deprecated *) obj)->row_ == anAStarItem->row_) {
                sameItem = (MCAStarItem_deprecated*) obj;
                return true ;
            }
    }
	return false;
}

//void
//getPath(){
//    CCObject* obj = NULL;
//    CCARRAY_FOREACH(closeList_,obj){
//        ((MCAStarItem_deprecated *) obj)->col_

//    }
//}
