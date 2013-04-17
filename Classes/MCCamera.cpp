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
MCCamera::locate()
{
    if (sceneDelegate_) {
        sceneDelegate_->moveSceneToLocation(ccpNeg(viewport_.origin));
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
        CCPoint rolePosition = ccpSub(aRole->getEntity()->getPosition(), mapOffset);
        CCPoint expectedViewportOrigin = ccp(rolePosition.x - winSize.width / 2,
                                             rolePosition.y - winSize.height / 2);
        /* 清除偏移 */
        /* 基于地图 */
        float viewportLeft = expectedViewportOrigin.x;
        float viewportRight = expectedViewportOrigin.x + viewport_.size.width;
        float viewportTop = expectedViewportOrigin.y + viewport_.size.height;
        float viewportBottom = expectedViewportOrigin.y;
        
        if (sceneSize.width < winSize.width) {
            sceneSize.width = winSize.width;
        }
        if (sceneSize.height < winSize.height) {
            sceneSize.height = winSize.height;
        }
        
        if (viewportLeft < 0) {
            expectedViewportOrigin.x -= viewportLeft;
        } else if (viewportRight > sceneSize.width) {
            expectedViewportOrigin.x -= (viewportRight - sceneSize.width);
        }
        if (viewportTop > sceneSize.height) {
            expectedViewportOrigin.y -= (viewportTop - sceneSize.height);
        } else if (viewportBottom < 0) {
            expectedViewportOrigin.y -= viewportBottom;
        }
        
        viewport_.origin = expectedViewportOrigin;
        sceneDelegate_->moveSceneToLocation(ccpNeg(expectedViewportOrigin));
    }
}

void
MCCamera::focusHero()
{
    focus(MCHero::sharedHero());
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
