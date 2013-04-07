//
//  MCCamera.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-5.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCCamera.h"
#include "MCSceneManager.h"
#include "MCScene.h"
#include "MCHero.h"

MCCamera::MCCamera()
{
    viewport_.size = CCDirectorGetWindowsSize();
}

/**
 * 重置位置，必须先设置场景代理
 */
void
MCCamera::restore()
{
    viewport_.origin = ccpNeg(location_);
}

/**
 * 设置位置，必须先设置场景代理
 */
void
MCCamera::locate(bool adjusted)
{
    if (sceneDelegate_) {
        sceneDelegate_->moveSceneToLocation(ccpNeg(viewport_.origin), adjusted);
    }
}

/**
 * 聚焦到某人身上，必须先设置场景代理
 *
 * 聚焦：
 *    1.假定人物位于屏幕中间
 *    2.消除地图滚动的偏移
 */
void
MCCamera::focus(MCRole *aRole)
{
    if (sceneDelegate_) {
        CCSize winSize = CCDirectorGetWindowsSize();
        MCScene *scene = sceneDelegate_->getScene();
        CCTMXTiledMap *map = sceneDelegate_->getScene()->getMap();
        CCSize sceneSize = scene->getSceneSize();
        CCPoint mapOffset = map->getPosition();
        CCPoint rolePosition = aRole->getEntity()->getPosition();
        CCPointLog(rolePosition);
        CCRect expectedViewport = CCRectMake(rolePosition.x - winSize.width / 2,
                                             rolePosition.y - winSize.height / 2,
                                             winSize.width,
                                             winSize.height);
//        CCPoint expectedCenter = ccp(rolePosition.x - winSize.width / 2,
//                                     rolePosition.y - winSize.height / 2);
//        CCPointLog(expectedCenter);
        /* 清除偏移 */
        /* 基于地图 */
        float viewportLeft = expectedViewport.getMinX();
        float viewportRight = expectedViewport.getMaxX();
        float viewportTop = expectedViewport.getMaxY();
        float viewportBottom = expectedViewport.getMinY();
        
        if (viewportLeft < 0) {
            expectedViewport.origin.x -= viewportLeft;
        } else if (viewportRight > sceneSize.width) {
            expectedViewport.origin.x -= (viewportRight - sceneSize.width);
        }
        if (viewportTop > sceneSize.height) {
            expectedViewport.origin.y -= (viewportTop - sceneSize.height);
        } else if (viewportBottom < 0) {
            expectedViewport.origin.y -= viewportBottom;
        }
        
        viewport_ = expectedViewport;
        sceneDelegate_->moveSceneToLocation(ccpNeg(expectedViewport.origin));
    }
//    if (sceneDelegate_) {
//        CCSize winSize = CCDirectorGetWindowsSize();
//        CCPoint mapOffset = sceneDelegate_->getScene()->getMapOffset();
//        CCPoint rolePosition = aRole->getEntity()->getPosition();
//        CCPoint center = ccp(winSize.width / 2, winSize.height / 2);
//        CCPoint roleOffset = ccpSub(ccpSub(center, mapOffset), rolePosition);
//        sceneDelegate_->moveSceneToLocation(roleOffset, true);
//    }
}

void
MCCamera::focusHero()
{
    focus(MCHero::sharedHero());
//    if (sceneDelegate_) {
//        CCSize winSize = CCDirectorGetWindowsSize();
//        CCPoint mapOffset = sceneDelegate_->getScene()->getMapOffset();
//        CCPoint rolePosition = MCHero::sharedHero()->getEntity()->getPosition();
//        CCPointLog(rolePosition);
//        CCPoint expectedCenter = ccp(rolePosition.x - winSize.width / 2,
//                                     rolePosition.y - winSize.height / 2);
//        CCPointLog(expectedCenter);
//        sceneDelegate_->moveSceneToLocation(ccpNeg(expectedCenter), true);
//    }
}

MCSceneDelegate *
MCCamera::getSceneDelegate()
{
    return sceneDelegate_;
}

void
MCCamera::setSceneDelegate(MCSceneDelegate *var)
{
    sceneDelegate_ = var;
    location_ = sceneDelegate_->getDefaultLocation();
}
