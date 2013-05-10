//
//  MCTrap.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTrap.h"
#include "MCRole.h"

const char *kMCUnknownTrap = "未知陷阱";
const char *kMCFireballTrap = "火球陷阱";
const char *kMCCurseTrap = "诅咒陷阱";
const char *kMCParalysisTrap = "麻痹陷阱";
const char *kMCFogTrap = "迷雾陷阱";
const char *kMCFlashTrap = "闪光陷阱";

const char *
MCTrapGetNameWithTrapType(MCTrapType aTrapType)
{
    switch (aTrapType) {
        case MCFireballTrap:
            return kMCFireballTrap;
        case MCCurseTrap:
            return kMCCurseTrap;
        case MCParalysisTrap:
            return kMCParalysisTrap;
        case MCFogTrap:
            return kMCFogTrap;
        case MCFlashTrap:
            return kMCFlashTrap;
            
        default:
            return kMCUnknownTrap;
    }
}

static bool
_mcCircleWithRadiusCollidesWithRect(const CCPoint &circleCenterPoint, float circleRadius, const CCRect &aRect)
{
        //分别计算圆心和矩形中心，X轴和Y轴方向上的长度。
    float rectHalfWidth = aRect.size.width / 2;
    float rectHalfHeigth = aRect.size.height / 2;
    CCPoint rectPoint = ccpAdd(aRect.origin, ccp(rectHalfWidth, rectHalfHeigth));
    float distanceX = abs(circleCenterPoint.x - rectPoint.x);
    float distanceY = abs(circleCenterPoint.y - rectPoint.y);
    
        //图我就不想画了，在脑海里想象一下，两个形状。当两个中心在X轴方向的距离大于radius＋halfW时，
        //两个形状必不相交。同理，在Y轴也是这个原理。这里展示了圆跟矩形的中心满足最大距离时不相交的情况。
    if (distanceX > (rectHalfWidth + circleRadius)) { return false; }
    if (distanceY > (rectHalfHeigth + circleRadius)) { return false; }
    
        //接下来是圆心与矩形的中心距离，满足相交的最小距离，这个条件正确的前提是先执行上面的判断，
        //然后再执行如下判断。两个中心在X轴方向的距离小于halfW时，两个形状必相交。同理，在Y轴也一样。
    if (distanceX <= rectHalfWidth) { return true; }if (distanceY <= rectHalfHeigth) { return true; }
    
        //好了，可能感觉上上面已经满足了判断圆与矩形相交的所有情况，其实不是，仔细在脑海里想下，如果
        //在X轴方向的距离大于halfW小于halfW＋radius，同时在Y轴方向的距离大于halfH小于halfH＋radius，
        //这时候也有可能相交。如果画图表现的话就是矩形的一个角与圆形相交，满足这个情况的条件是，矩形的角
        //那个点与圆心的距离小于圆的半径，代码如下：
    
    float distance = (distanceX - rectHalfWidth)*(distanceX - rectHalfWidth) + (distanceY-rectHalfHeigth)*(distanceY-rectHalfHeigth);
    
    return (distance <= circleRadius * circleRadius);
}

MCTrap::~MCTrap()
{
    CC_SAFE_RELEASE(effect);
}

bool
MCTrap::init(mc_object_id_t anObjectId)
{
    if (MCEffectiveItem::init()) {
        setID(anObjectId);
        position_ = CCPointZero;
        collidedTarget_ = NULL;
        userObject_ = NULL;
        
        return true;
    }
    
    return false;
}

MCTrap *
MCTrap::create(mc_object_id_t anObjectId)
{
    MCTrap *trap = new MCTrap;
    
    if (trap && trap->init(anObjectId)) {
        trap->autorelease();
    } else {
        CC_SAFE_DELETE(trap);
        trap = NULL;
    }
    
    return trap;
}

CCObject *
MCTrap::copy()
{
    MCTrap *trap = new MCTrap;
    
    trap->init(id_);
    trap->tag_ = tag_;
    trap->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    trap->name_->retain();
    if (description_ != NULL) {
        trap->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
        trap->description_->retain();
    }
    trap->price_ = price_;
    trap->itemType_ = itemType_;
    trap->icon_ = CCString::create(icon_->getCString()); /* 会被释放掉，所以要copy一个 */
    trap->icon_->retain();
    trap->radius = radius;
//    trap->effect = dynamic_cast<MCEffect *>(effect->copy());
    trap->effect = effect;
    effect->retain();
    
    /* effect */
    trap->hp = hp;             /* HP变化值 */
    trap->pp = pp;             /* PP变化值 */
    trap->positive_state = positive_state; /* 会增加的状态 */
    trap->lasting_time = lasting_time;   /* 效果时间 */

//    trap->trapType_ = trapType_;
    
    return trap;
}

bool
MCTrap::collidesWith(MCRole *aRole)
{
    CCRect aabb = aRole->getEntity()->getOBB().getAABB();
    
    if (_mcCircleWithRadiusCollidesWithRect(position_,
                                            radius,
                                            aabb)) {
        collidedTarget_ = aRole;
        
        return true;
    }
    
    return false;
}
