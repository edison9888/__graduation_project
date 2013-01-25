//
//  MCTestControllerScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCTestControllerScene.h"

#include "MCPointExtension.h"

#define kMCDuraitonMap   0.25f
#define kMCDurationHero  0.25f

bool
MCTestControllerLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(64, 64, 64, 255)))
    {
        CCSize winSize = CCDirectorGetWindowsSize();
        
        map_ = CCTMXTiledMap::create("1.tmx");
        addChild(map_);
        
        metaLayer_ = map_->layerNamed("meta");
        metaLayer_->setVisible(false);
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("mc_crow_cocos2dx.plist");
        CCSpriteBatchNode* spriteSheet = CCSpriteBatchNode::create("mc_crow_cocos2dx.png");
        
        addChild(spriteSheet);
        
        CCArray *flyToLeftAnimFrames = CCArray::create();
        char str[64];
        for(int i = 0; i < 4; i++)
        {
            sprintf(str, "mc_crow_left_fly_%d.png", i);
            CCSpriteFrame *spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
            flyToLeftAnimFrames->addObject(spriteFrame);
        }
        CCAnimation * animation = CCAnimation::createWithSpriteFrames(flyToLeftAnimFrames, 0.1f);
        
        CCSpriteFrame *spriteFrame = (CCSpriteFrame *) flyToLeftAnimFrames->objectAtIndex(0);
        CCSprite* crow = CCSprite::createWithSpriteFrame(spriteFrame);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        spriteSheet->addChild(crow);
        
        hero_ = crow;
        flyAction_ = CCRepeatForever::create(CCAnimate::create(animation));
        
        mapSize = MCSizeFromCCSize(map_->getMapSize());
        tileSize = MCSizeFromCCSize(map_->getTileSize());
        contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
        mapWidth = mapSize.width * tileSize.width / contentScaleFactor;
        mapHeight = mapSize.height * tileSize.height / contentScaleFactor;
        winWidth = winSize.width;
        winHeight = winSize.height;
        
        blockWidth = tileSize.width * 2 / contentScaleFactor;
        blockHeight = tileSize.height * 2 / contentScaleFactor;
        offsetUp = MCPointMake(0, blockHeight);
        offsetDown = MCPointMake(0, -blockHeight);
        
        offsetLeft = MCPointMake(-blockWidth, 0);
        offsetRight = MCPointMake(blockWidth, 0);
        
        mapAction_ = NULL;
        heroAction_ = NULL;
        
        return true;
    }
    
    return false;
}

void
MCTestControllerLayer::controllerMoveUp(MCControllerDelegate *sender)
{
    moveTo(MCControlButtonUp);
}

void
MCTestControllerLayer::controllerMoveDown(MCControllerDelegate *sender)
{
    moveTo(MCControlButtonDown);
}

void
MCTestControllerLayer::controllerMoveLeft(MCControllerDelegate *sender)
{
    hero_->setFlipX(false);
    moveTo(MCControlButtonLeft);
}

void
MCTestControllerLayer::controllerMoveRight(MCControllerDelegate *sender)
{
    hero_->setFlipX(true);
    moveTo(MCControlButtonRight);
}

MCPoint
MCTestControllerLayer::tileCoordForPosition(MCPoint pos)
{
    int x = pos.x / tileSize.width;
    int y = pos.y / tileSize.height;
    CCLog("(%d %d) (%d %d)", pos.x, pos.y, blockWidth, blockHeight);

    /* Cocos2d-x的默认Y坐标是由下至上的，所以要做一个相减操作 */
    y = mapSize.height - y - 2;
    
    return mcp(x, y);
}


void
MCTestControllerLayer::onEnter()
{
    CCLayerColor::onEnter();
    hero_->setAnchorPoint(CCPointZero);
    hero_->setPosition(ccp(blockWidth, blockHeight));
    CCPointLog(hero_->getPosition());
    hero_->runAction(flyAction_);
    schedule(schedule_selector(MCTestControllerLayer::update));
}

void
MCTestControllerLayer::onExit()
{
    unschedule(schedule_selector(MCTestControllerLayer::update));
    CCLayerColor::onExit();
}

void
MCTestControllerLayer::update(float dt)
{
    if (mapAction_ != NULL && mapAction_->isDone()) {
        mapAction_ = NULL;
    }
    if (heroAction_ != NULL && heroAction_->isDone()) {
        heroAction_->release();
        heroAction_ = NULL;
    }
}

void
MCTestControllerLayer::moveTo(MCControlButtonCode aCode)
{
    if (aCode == MCControlButtonLeft || aCode == MCControlButtonRight) {
        horizontallyMove(aCode);
    } else if (aCode == MCControlButtonUp || aCode == MCControlButtonDown) {
        verticallyMove(aCode);
    }
}

void
MCTestControllerLayer::verticallyMove(MCControlButtonCode aCode)
{
    if (mapAction_ != NULL || heroAction_ != NULL) {
        return;
    }
    
    MCPoint heroCurrentPosition = MCPointFromCCPoint(hero_->getPosition());
    MCPoint mapCurrentPosition;
    MCPoint heroCurrentAtMapPosition;
    CCPoint heroMaybeMoveToPosition;
    CCPoint mapMaybeMoveToPosition;
    MCPoint heroMaybeAtMapPosition;
    MCPoint tempPoint;
    MCPoint delta = aCode == MCControlButtonUp ? offsetUp : offsetDown;
    int offset;
    CCPoint tilePosition;
    int tileGID;
    CCNode *target;
    float duration;
    CCPoint targetPosition;
    CCAction *targetAction;
    int edgeTop = mapHeight - winHeight / 2 + (aCode == MCControlButtonUp ? 0 : blockHeight);
    int edgeBottom = winHeight / 2 + (aCode == MCControlButtonDown ? blockHeight : 0);
    
    /* 坐标对齐 */
    offset = heroCurrentPosition.y % blockHeight;
    heroCurrentPosition.y -= offset;
    if (offset > blockHeight / 2) {
        heroCurrentPosition.y += blockHeight;
    }
    mapCurrentPosition = MCPointFromCCPoint(map_->getPosition());
    offset = mapCurrentPosition.y % blockHeight;
    mapCurrentPosition.y -= offset;
    if (offset > blockHeight / 2) {
        mapCurrentPosition.y += blockHeight;
    }
    
    heroCurrentAtMapPosition = mcpSub(heroCurrentPosition, mapCurrentPosition);
    tempPoint = mcpAdd(heroCurrentPosition, delta);
    heroMaybeMoveToPosition = CCPointFromMCPoint(tempPoint);
    mapMaybeMoveToPosition = CCPointFromMCPoint(mcpAdd(mapCurrentPosition, mcpNeg(delta)));
    heroMaybeAtMapPosition = mcpSub(tempPoint, mapCurrentPosition); /* tempPoint当前为heroMaybeMoveToPosition */
    if (heroCurrentAtMapPosition.y > edgeBottom
        && heroCurrentAtMapPosition.y < edgeTop) { /* 移动地图 */
        target = map_;
        duration = kMCDuraitonMap;
        targetPosition = mapMaybeMoveToPosition;
    } else {
        target = hero_;
        duration = kMCDurationHero;
        targetPosition = heroMaybeMoveToPosition;
    }
    tilePosition = CCPointFromMCPoint(tileCoordForPosition(heroMaybeAtMapPosition));
    tileGID = metaLayer_->tileGIDAt(tilePosition);
    
    CCLog("%d(%.0f, %.0f)", tileGID, tilePosition.x, tilePosition.y);
    if(tileGID != 0) { /* 不为0，代表存在这个格子 */
        CCDictionary* propertiesDict = map_->propertiesForGID(tileGID);
        const CCString* prop = propertiesDict->valueForKey("Type");
        
        if(prop->m_sString.compare("1") == 0) {
            targetPosition.y += blockHeight * ((aCode == MCControlButtonUp
                                                ? (target == hero_ ? -1 : 1)
                                                : (target == hero_ ? 1 : -1)));
        }
    }
    targetAction = new CCMoveTo;
    ((CCMoveTo *)targetAction)->initWithDuration(duration, targetPosition);
    if (target == map_) {
        mapAction_ = targetAction;
    } else {
        heroAction_ = targetAction;
    }
    target->runAction(targetAction);
}

void
MCTestControllerLayer::horizontallyMove(MCControlButtonCode aCode)
{
    if (mapAction_ != NULL || heroAction_ != NULL) {
        return;
    }
    
    MCPoint heroCurrentPosition = MCPointFromCCPoint(hero_->getPosition());
    MCPoint mapCurrentPosition;
    MCPoint heroMaybeAtMapPosition;
    CCPoint heroMaybeMoveToPosition;
    CCPoint mapMaybeMoveToPosition;
    MCPoint heroCurrentAtMapPosition;
    MCPoint tempPoint;
    MCPoint delta = aCode == MCControlButtonLeft ? offsetLeft : offsetRight;
    int offset;
    CCPoint tilePosition;
    int tileGID;
    CCNode *target;
    float duration;
    CCPoint targetPosition;
    CCAction *targetAction;
    int edgeLeft = winWidth / 2 + (aCode == MCControlButtonLeft ? blockWidth : 0);
    int edgeRight = mapWidth - winWidth / 2 + (aCode == MCControlButtonRight ? 0 : blockWidth);
    
    /* 坐标对齐 */
    offset = heroCurrentPosition.x % blockWidth;
    heroCurrentPosition.x -= offset;
    if (offset > blockWidth / 2) {
        heroCurrentPosition.x += blockWidth;
    }
    mapCurrentPosition = MCPointFromCCPoint(map_->getPosition());
    offset = mapCurrentPosition.x % blockWidth;
    mapCurrentPosition.x -= offset;
    if (offset > blockWidth / 2) {
        targetPosition.x += blockWidth;
    }
    
    heroCurrentAtMapPosition = mcpSub(heroCurrentPosition, mapCurrentPosition);
    tempPoint = mcpAdd(heroCurrentPosition, delta);
    heroMaybeMoveToPosition = CCPointFromMCPoint(tempPoint);
    mapMaybeMoveToPosition = CCPointFromMCPoint(mcpAdd(mapCurrentPosition, mcpNeg(delta)));
    heroMaybeAtMapPosition = mcpSub(tempPoint, mapCurrentPosition); /* tempPoint当前为heroMaybeMoveToPosition */
    if (heroCurrentAtMapPosition.x > edgeLeft
        && heroCurrentAtMapPosition.x < edgeRight) { /* 移动地图 */
        target = map_;
        duration = kMCDuraitonMap;
        targetPosition = mapMaybeMoveToPosition;
    } else {
        target = hero_;
        duration = kMCDurationHero;
        targetPosition = heroMaybeMoveToPosition;
    }
    tilePosition = CCPointFromMCPoint(tileCoordForPosition(heroMaybeAtMapPosition));
    tileGID = metaLayer_->tileGIDAt(tilePosition);
    
    CCLog("%d(%.0f, %.0f)", tileGID, tilePosition.x, tilePosition.y);
    if(tileGID != 0) { /* 不为0，代表存在这个格子 */
        CCDictionary* propertiesDict = map_->propertiesForGID(tileGID);
        const CCString* prop = propertiesDict->valueForKey("Type");
        
        if(prop->m_sString.compare("1") == 0) {
            targetPosition.x += blockWidth * ((aCode == MCControlButtonRight
                                               ? (target == hero_ ? -1 : 1)
                                               : (target == hero_ ? 1 : -1)));
        }
    }
    targetAction = new CCMoveTo;
    ((CCMoveTo *)targetAction)->initWithDuration(duration, targetPosition);
    if (target == map_) {
        mapAction_ = targetAction;
    } else {
        heroAction_ = targetAction;
    }
    target->runAction(targetAction);
}
