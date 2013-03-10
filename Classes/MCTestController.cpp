//
//  MCTestController.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCTestController.h"

#include "MCPointExtension.h"

#define kMCDuraitonMap   0.025f

#include "MCHero.h"
#include "MCBarrier.h"
#include "MCMicsUtil.h"

static MCFacade
__facadeForOffset(const CCPoint &offset)
{
    float angle;
        //每个方向分配60度角的空间
    MCGetAngleForPoint(offset, angle);
    if (angle < 22.5f) {
        if (offset.x > 0) {
            return MCFacingRight;
        } else {
            return MCFacingLeft;
        }
    } else if (angle < 67.5f) {
        if (offset.y > 0) {
            return MCFacingUp;
        } else {
            return MCFacingDown;
        }
    } else {
        if (offset.y > 0) {
            return MCFacingUp;
        } else {
            return MCFacingDown;
        }
    }
}

//static void
//__moveHero(MCControlButtonCode aCode)
//{
//    MCHero *hero = MCHero::sharedHero();
//    switch (aCode) {
//    case MCControlButtonUp:
//            hero->walk(MCFacingUp);
//            break;
//    case MCControlButtonDown:
//            hero->walk(MCFacingDown);
//            break;
//    case MCControlButtonLeft:
//    case MCControlButtonUpLeft:
//    case MCControlButtonDownLeft:
//            hero->walk(MCFacingLeft);
//            break;
//    case MCControlButtonRight:
//    case MCControlButtonUpRight:
//    case MCControlButtonDownRight:
//            hero->walk(MCFacingRight);
//            break;
//    }
//}

void
MCTestController::draw()
{
}

bool
MCTestController::init()
{
    if (MCTestLayer::initWithColor(ccc4(64, 64, 64, 255)))
    {
        CCSize winSize = CCDirectorGetWindowsSize();

        map_ = CCTMXTiledMap::create("maps/forest.tmx");
        addChild(map_);
        
//        metaLayer_ = map_->layerNamed("meta");
//        metaLayer_->setVisible(false);
        
//        CCTMXObjectGroup *metaObejcts = map_->objectGroupNamed("meta-objects");
//        CCArray *barriers = metaObejcts->getObjects(); /* 还是原始数据，先处理一次转换成CCRect先 */
//        CCObject *obj;
//        barriers_ = CCArray::createWithCapacity(barriers->count());
//        barriers_->retain();
//        CCARRAY_FOREACH(barriers, obj) {
//            CCDictionary *dict = (CCDictionary *) obj;
//            CCRect rect = CCRectMake(dict->valueForKey("x")->floatValue(),
//                                     dict->valueForKey("y")->floatValue(),
//                                     dict->valueForKey("width")->floatValue(),
//                                     dict->valueForKey("height")->floatValue());
//            MCBarrier *barrier;
//            
//            if (0 == dict->valueForKey("type")->m_sString.compare(kMCTypeBarrier)) {
//                barrier = MCBarrier::create(rect, dict->valueForKey("barrier-type")->uintValue());
//                barriers_->addObject(barrier);
//            }
//        }
        
        mapSize = MCSizeFromCCSize(map_->getMapSize());
        tileSize = MCSizeFromCCSize(map_->getTileSize());
        contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
        mapWidth = mapSize.width * tileSize.width / contentScaleFactor;
        mapHeight = mapSize.height * tileSize.height / contentScaleFactor;
        scaledTileSize = CCSizeMake(tileSize.width / contentScaleFactor, tileSize.height / contentScaleFactor);
        winWidth = winSize.width;
        winHeight = winSize.height;
        
        blockWidth = tileSize.width / contentScaleFactor;
        blockHeight = tileSize.height / contentScaleFactor; 
        
        MCHero *hero = MCHero::sharedHero();
        hero_ = hero->getEntity();
        CCSpriteBatchNode *spriteSheet = hero_->getSpriteSheet();
        addChild(spriteSheet);
        
        mapAction_ = NULL;
        heroAction_ = NULL;
        walkAction_ = NULL;
        
        label_ = CCLabelTTF::create("", "Arial", 24);
        addChild(label_);
        label_->setPosition(CCPointMake(20, 20));
        label_->setAnchorPoint(CCPointZero);
        
        for (int i=0; i<4; ++i) {
            sp[i]=CCSprite::create("p.png");
            addChild(sp[i]);
        }
        
        return true;
    }
    
    return false;
}

CCPoint
MCTestController::viewLocationToTiledMapLocation(CCPoint aViewLocation)
{
    CCPoint mapPosition = map_->getPosition();
    CCPoint mapLocation = CCPointMake(aViewLocation.x - mapPosition.x,
                                      mapSize.height * scaledTileSize.height - aViewLocation.y + mapPosition.y);
//    CCPointLog(mapLocation);
//    CCLog("%f", mapLocation.x / (int) tileSize.width);
    float x = (mapLocation.x / scaledTileSize.width);
    float y = (mapLocation.y / scaledTileSize.height) - 1;

    return CCPointMake(x, y);
}

void
MCTestController::controllerMove(MCControllerDelegate *sender, const CCPoint &delta)
{
    moveTo(delta);
}

void
MCTestController::onEnter()
{
    CCLayerColor::onEnter();
    hero_->setPosition(ccp(blockWidth * 4, blockHeight * 4));
    CCPointLog(hero_->getPosition());
//    CCPointLog(viewLocationToTiledMapLocation(hero_->getPosition()));
//    schedule(schedule_selector(MCTestController::update));
}

void
MCTestController::onExit()
{
//    unschedule(schedule_selector(MCTestController::update));
    CCLayerColor::onExit();
}

void
MCTestController::update(float dt)
{
}

void
MCTestController::moveTo(const CCPoint &offset)
{
    /* screen view */
    CCPoint heroCurrentPosition = hero_->getPosition();
    CCPoint mapCurrentPosition = map_->getPosition();
    CCPoint heroCurrentPositionAtMap = ccpSub(heroCurrentPosition, mapCurrentPosition);
    CCPoint heroMaybeMoveToPosition;
    CCPoint heroMaybeMoveToPositionAtMap;
    CCPoint heroMaybeMoveToPositionAtMapForCheck;
    CCPoint mapMaybeMoveToPosition;
    /* tile view */
    CCPoint heroCurrentPositionAtTiledMap = viewLocationToTiledMapLocation(heroCurrentPositionAtMap);
    CCPoint heroMaybeMoveToPositionAtTiledMap;
    CCPoint heroMaybePositionAtTiledMap;
    CCPoint delta;
    CCPoint deltaForMap;
    CCPoint deltaForHero;
    CCPoint deltaForCheck;
    
    /* 障碍物检测用 */
    CCSize spriteSize = hero_->getContentSize();
    CCPoint pointForCheck = CCPointZero;
    
    /* 地图移动检测用 */
    int edgeTop = mapHeight - winHeight / 2;
    int edgeBottom = winHeight / 2;
    int edgeLeft = winWidth / 2;
    int edgeRight = mapWidth - winWidth / 2;
    
    /* 以TMX地图为参照 */
    delta = ccpNormalize(offset);
    delta = ccpMult(delta, 4);
    deltaForMap = ccpNeg(delta);
    deltaForHero = CCPoint(delta);
    deltaForCheck = CCPoint(deltaForHero);
    
    /* 让移动更平滑~~~~ */
    if (deltaForHero.x > -1.5 && deltaForHero.x < 1.5) {
        deltaForHero.x = 0;
        deltaForCheck.x = deltaForHero.x > 0 ? -0.5 : 0.5;
    }
    if (deltaForHero.y > -1.5 && deltaForHero.y < 1.5) {
        deltaForHero.y = 0;
        deltaForCheck.y = deltaForHero.y > 0 ? -0.5 : 0.5;
    }

    
    heroMaybeMoveToPositionAtMap = ccpAdd(heroCurrentPositionAtMap, deltaForHero);
    heroMaybeMoveToPositionAtMapForCheck = ccpAdd(heroCurrentPositionAtMap, deltaForCheck);
    
    if ((heroMaybeMoveToPositionAtMap.x > edgeLeft && heroMaybeMoveToPositionAtMap.x < edgeRight)) { /* 移动地图 */
        deltaForHero.x = 0;
    } else {
        deltaForMap.x = 0;
    }
    
    if ((heroMaybeMoveToPositionAtMap.y > edgeBottom && heroMaybeMoveToPositionAtMap.y < edgeTop)) { /* 移动地图 */
        deltaForHero.y = 0;
    } else {
        deltaForMap.y = 0;
    }
    
    heroMaybeMoveToPosition = ccpAdd(heroCurrentPosition, deltaForHero);
    mapMaybeMoveToPosition = ccpAdd(mapCurrentPosition, deltaForMap);
    
    /* 检测地图的越界偏移 */
    if (mapMaybeMoveToPosition.x < -(mapWidth - winWidth)) { /* 过左 */
        deltaForMap.x -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).x;
    } else if (mapMaybeMoveToPosition.x > 0) { /* 过右 */
        deltaForMap.x -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).x;
    }
    if (mapMaybeMoveToPosition.y < -(mapHeight - winHeight)) { /* 过低 */
        deltaForMap.y -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).y;
    } else if (mapMaybeMoveToPosition.y > 0) { /* 过高 */
        deltaForMap.y -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).y;
    }
    
    /* 碰撞点debug */
    CCPoint p[4];
    p[0] = ccpAdd(heroMaybeMoveToPosition, hero_->getBounds().origin);
    p[1] = p[0];
    p[2] = p[0];
    p[3] = p[0];
    p[0].y += blockHeight / 2;
    p[1].x += hero_->getBounds().size.width;
    p[1].y += blockHeight / 2;
    p[2].x += hero_->getBounds().size.width;
    p[2].y += hero_->getBounds().size.height;
    p[3].y += hero_->getBounds().size.height;
    for (int i = 0; i < 4; ++i) {
        sp[i]->setPosition(p[i]);
    }
    
    /* 矩形框检测方案 */
//    CCObject *obj;
//    MCBarrier * barrier;
//    CCARRAY_FOREACH(barriers_, obj) {
//        barrier = (MCBarrier *) obj;
//        if (barrier->collideWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
////            if (deltaForHero.x < 1.5) {
////                deltaForCheck.x = 0;
////            } else if (deltaForHero.x > -1.5) {
////                deltaForCheck.x = 0;
////            }
////            if (deltaForHero.y < 1.5) {
////                deltaForCheck.y = 0;
////            } else if (deltaForHero.y > -1.5) {
////                deltaForCheck.y = 0;
////            }
////
////            heroMaybeMoveToPositionAtMapForCheck = ccpAdd(heroCurrentPositionAtMap, deltaForCheck);
////            if (barrier->collideWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
//                deltaForMap = CCPointZero;
//                deltaForHero = CCPointZero;
//                break;
//            CCLog("bingo");
////            }
//        }
//    }
    
//    /* 根据方向2/3点检测方案，抛弃 */
//    unsigned int *tiles = metaLayer_->getTiles();
//    CCPoint p[3];
//    CCRect boundsForCheck = hero_->getBounds();
//    mc_size_t count = MCGetCheckPoints(boundsForCheck, heroMaybeMoveToPosition, offset,
//                                       p[0], p[1], p[2]);
//    
//    p[0].y -= blockHeight;
//    p[1].y -= blockHeight;
//    if (count == 2) {
//        p[2].y -= blockHeight;
//        sp[2]->setPosition(ccp(-100,100));
////        CCLog("<%.1f, %.1f> - <%.1f, %.1f>",
////              p[0].x, p[0].y,
////              p[1].x, p[1].y);
////    } else {
////        CCLog("<%.1f, %.1f> - <%.1f, %.1f> - <%.1f, %.1f>",
////              p[0].x, p[0].y,
////              p[1].x, p[1].y,
////              p[2].x, p[2].y);
//    }
//    for (mc_size_t i = 0; i < count; ++i) {
//        sp[i]->setPosition(p[i]);
//        pointForCheck = viewLocationToTiledMapLocation(p[i]);
//        
//        tileGID = tiles[(int)pointForCheck.x + (int)pointForCheck.y * (int)metaLayer_->getLayerSize().width] & kCCFlippedMask;
//        
//        if(tileGID != 0) { /* 不为0，代表存在这个格子 */
//            CCDictionary* propertiesDict = map_->propertiesForGID(tileGID);
//            const CCString* prop = propertiesDict->valueForKey("Type");
//            
//            if(prop->m_sString.compare("1") == 0) { /* 移动方向有障碍物 */
//                deltaForMap = CCPointZero;
//                deltaForHero = CCPointZero;
//                break;
//            }
//        }
//    }
    /* 直接4点检测方案，抛弃 */
//    unsigned int *tiles = metaLayer_->getTiles();
//    CCPoint p[4];
//    p[0] = ccpAdd(heroMaybeMoveToPosition, boundsForCheck.origin);
//    p[1] = p[0];
//    p[2] = p[0];
//    p[3] = p[0];
//    p[0].y += blockHeight / 2 - blockHeight;
//    p[1].x += boundsForCheck.size.width;
//    p[1].y += blockHeight / 2 - blockHeight;
//    p[2].x += boundsForCheck.size.width;
//    p[2].y += boundsForCheck.size.height - blockHeight;
//    p[3].y += boundsForCheck.size.height - blockHeight;
//    for (int i = 0; i < 4; ++i) {
//        sp[i]->setPosition(p[i]);
//        pointForCheck = viewLocationToTiledMapLocation(p[i]);
//        
//        tileGID = tiles[(int)pointForCheck.x + (int)pointForCheck.y * (int)metaLayer_->getLayerSize().width] & kCCFlippedMask;
//        
//        if(tileGID != 0) { /* 不为0，代表存在这个格子 */
//            CCDictionary* propertiesDict = map_->propertiesForGID(tileGID);
//            const CCString* prop = propertiesDict->valueForKey("Type");
//            
//            if(prop->m_sString.compare("1") == 0) { /* 移动方向有障碍物 */
//                deltaForMap = CCPointZero;
//                deltaForHero = CCPointZero;
//                break;
//            }
//        }
//    }

    map_->runAction(CCSequence::create(CCMoveBy::create(kMCDuraitonMap, deltaForMap),
                                       CCCallFunc::create(hero_, callfunc_selector(MCRoleEntity::walkEnded)),
                                       NULL));
    hero_->walkOnScreen(deltaForHero, offset);
}
