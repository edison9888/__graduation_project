//
//  MCObjectLayer.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-11.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCObjectLayer.h"
#include "MCFlagManager.h"
#include "MCEntrance.h"

#define kMCDuraitonMap   0.025f

#include "MCHero.h"
#include "MCBarrier.h"
#include "MCMicsUtil.h"

#include "MCGameScene.h"

MCObjectLayer::~MCObjectLayer()
{
    CC_SAFE_RELEASE(npcs_);
    CC_SAFE_RELEASE(monsters_);
    CC_SAFE_RELEASE(mercenaries_);
    
    CC_SAFE_RELEASE(entrances_);
    CC_SAFE_RELEASE(semiTransparents_);
    CC_SAFE_RELEASE(barriers_);
}

bool
MCObjectLayer::init()
{
    if (CCLayer::init()) {
        setTouchEnabled(true);
        
        npcs_ = CCArray::create();
        npcs_->retain();
        monsters_ = CCArray::create();
        monsters_->retain();
        mercenaries_ = CCArray::create();
        mercenaries_->retain();
        
        return true;
    }
    
    return false;
}

void
MCObjectLayer::setTMXTiledMap(CCTMXTiledMap *aMap)
{
    CCSize winSize = CCDirectorGetWindowsSize();
    
    map_ = aMap;
    
    metaLayer_ = map_->layerNamed("meta");
    metaLayer_->setVisible(false);
    
    /* 添加障碍物 */
    CCTMXObjectGroup *barrierObejctsObejcts = map_->objectGroupNamed("barriers");
    CCArray *barriers = barrierObejctsObejcts->getObjects(); /* 还是原始数据，先处理一次转换成CCRect先 */
    CCObject *obj;
    barriers_ = CCArray::createWithCapacity(barriers->count());
    barriers_->retain();
    CCARRAY_FOREACH(barriers, obj) {
        CCDictionary *dict = (CCDictionary *) obj;
        CCRect rect = CCRectMake(dict->valueForKey("x")->floatValue(),
                                 dict->valueForKey("y")->floatValue(),
                                 dict->valueForKey("width")->floatValue(),
                                 dict->valueForKey("height")->floatValue());
        MCBarrier *barrier;
        
        if (0 == dict->valueForKey("type")->m_sString.compare(kMCTypeBarrier)) {
            barrier = MCBarrier::create(rect, dict->valueForKey("barrier-type")->uintValue());
            barriers_->addObject(barrier);
        }
    }
    
    /* 添加半透明遮掩物 */
    CCTMXObjectGroup *semiTransparentObejcts = map_->objectGroupNamed("semi-transparents");
    CCArray *semiTransparents = semiTransparentObejcts->getObjects(); /* 还是原始数据，先处理一次转换成CCRect先 */
    semiTransparents_ = CCArray::createWithCapacity(barriers->count());
    semiTransparents_->retain();
    CCARRAY_FOREACH(semiTransparents, obj) {
        CCDictionary *dict = (CCDictionary *) obj;
        CCRect rect = CCRectMake(dict->valueForKey("x")->floatValue(),
                                 dict->valueForKey("y")->floatValue(),
                                 dict->valueForKey("width")->floatValue(),
                                 dict->valueForKey("height")->floatValue());
        MCSemiTransparent *semiTransparent;
        
        if (0 == dict->valueForKey("type")->m_sString.compare(kMCTypeSemiTransparent)) {
            semiTransparent = MCSemiTransparent::create(rect);
            semiTransparents_->addObject(semiTransparent);
        }
    }
    
    /* 添加入口 */
    CCTMXObjectGroup *entranceObejcts = map_->objectGroupNamed("entrances");
    CCArray *entrances = entranceObejcts->getObjects(); /* 还是原始数据，先处理一次转换成CCRect先 */
    entrances_ = CCArray::createWithCapacity(entrances->count());
    entrances_->retain();
    CCARRAY_FOREACH(entrances, obj) {
        CCDictionary *dict = (CCDictionary *) obj;
        CCRect rect = CCRectMake(dict->valueForKey("x")->floatValue(),
                                 dict->valueForKey("y")->floatValue(),
                                 dict->valueForKey("width")->floatValue(),
                                 dict->valueForKey("height")->floatValue());
        MCEntrance *entrance;
        
        if (0 == dict->valueForKey("type")->m_sString.compare(kMCTypeEntrance)) {
            entrance = MCEntrance::create(rect);
            CCString *ccstring = CCString::create(dict->valueForKey("name")->getCString());
            entrance->setName(ccstring);
            ccstring->retain();
            ccstring = CCString::create(dict->valueForKey("dest")->getCString());
            entrance->setDestination(ccstring);
            ccstring->retain();
            entrances_->addObject(entrance);
        }
    }
    
    mapSize_ = map_->getMapSize();
    tileSize_ = map_->getTileSize();
    contentScaleFactor_ = CCDirector::sharedDirector()->getContentScaleFactor();
    mapWidth_ = mapSize_.width * tileSize_.width / contentScaleFactor_;
    mapHeight_ = mapSize_.height * tileSize_.height / contentScaleFactor_;
    scaledTileSize_ = CCSizeMake(tileSize_.width / contentScaleFactor_,
                                 tileSize_.height / contentScaleFactor_);
    winWidth_ = winSize.width;
    winHeight_ = winSize.height;

}

void
MCObjectLayer::onEnter()
{
    CCLayer::onEnter();
    MCGameSceneContext *context = MCGameSceneContextManager::sharedGameSceneContextManager()->currentContext();
    MCScenePackage *package = context->getScene()->getScenePackage();
    CCDictionary *dataDict;
    CCDictElement *elemNPC;
    CCDictElement *elemMonster;
    MCNPC *npc;
    MCMonster *monster;
    
    /* load all roles */
    /* hero */
    MCHero *hero = MCHero::sharedHero();
    hero_ = hero->getEntity();
    CCSpriteBatchNode *spriteSheet = hero_->getSpriteSheet();
    addChild(spriteSheet);
    
    /* npcs */
    npcs_->removeAllObjects();
    dataDict = package->getNPCs();
    CCDICT_FOREACH(dataDict, elemNPC) {
        npc = (MCNPC *) elemNPC->getObject();
        /* 初始化NPC数据 */
        addChild(npc->getEntity()->getSpriteSheet());
        npc->getEntity()->setPosition(npc->getEntityMetadata()->getPosition());
        npcs_->addObject(npc);
    }
    
    /* monsters */
    monsters_->removeAllObjects();
    dataDict = package->getMonsters();
    CCDICT_FOREACH(dataDict, elemMonster) {
        monster = (MCMonster *) elemMonster->getObject();
        /* 初始化NPC数据 */
#warning initialize monsters data
        monsters_->addObject(monster);
    }
    
    /* mercenaries */
    mercenaries_->removeAllObjects();
#warning initialize here?
    
    /* load initialize position */
    CCTMXObjectGroup *objects = map_->objectGroupNamed("objects");
    if (objects) {
        CCDictionary *spawnPoint = objects->objectNamed("spawn-point");
#warning 搞个入口用着先
        mc_object_id_t flagId = {
            'F', '0', '0', '1'
        };
        MCFlag *flag = MCFlagManager::sharedFlagManager()->flagForObjectId(flagId);
        flag->setState(MCOnState);
        if (spawnPoint && flag->getState() == MCOnState) {
            hero_->setPosition(ccp(spawnPoint->valueForKey("x")->floatValue(),
                                   spawnPoint->valueForKey("y")->floatValue()));
        }
    }
}

void
MCObjectLayer::onExit()
{
    /* remove all roles */
    CCObject *obj;
//    MCRole *role;
    MCRoleEntity *entity;
    /* hero */
    hero_->getSpriteSheet()->removeFromParent();
    hero_->face(MCFacingDown);
    /* npcs */
    CCARRAY_FOREACH(npcs_, obj) {
        entity = ((MCRole *) obj)->getEntity();
        entity->getSpriteSheet()->removeFromParentAndCleanup(true);
        entity->face(MCFacingDown);
    }
    /* monsters */
    CCARRAY_FOREACH(monsters_, obj) {
        entity = ((MCRole *) obj)->getEntity();
        entity->getSpriteSheet()->removeFromParentAndCleanup(true);
        entity->face(MCFacingDown);
    }
    /* mercenaries */
    CCARRAY_FOREACH(mercenaries_, obj) {
        entity = ((MCRole *) obj)->getEntity();
        entity->getSpriteSheet()->removeFromParentAndCleanup(true);
        entity->face(MCFacingDown);
    }
    
    CCLayer::onExit();
}

void
MCObjectLayer::loadEntrancesFromScenePackage(MCScenePackage *aScenePackage)
{
    CCObject *obj;
    MCEntrance *entrance;
    MCEntrance *entranceInScenePackage;
    CCDictionary *entrances = aScenePackage->getScenes();
    CCARRAY_FOREACH(entrances_, obj) {
        entrance = (MCEntrance *) obj;
        entranceInScenePackage = (MCEntrance *) entrances->objectForKey(entrance->getName()->getCString());
        if (entranceInScenePackage) {
            entrance->setID(entranceInScenePackage->getID());
        }
    }
}

void
MCObjectLayer::controllerMove(MCControllerDelegate *sender, const CCPoint &delta)
{
    moveTo(delta);
}

CCPoint
MCObjectLayer::viewLocationToTiledMapLocation(const CCPoint &aViewLocation)
{
    CCPoint mapPosition = map_->getPosition();
    CCPoint mapLocation = CCPointMake(aViewLocation.x - mapPosition.x,
                                      mapSize_.height * scaledTileSize_.height - aViewLocation.y + mapPosition.y);
    float x = (mapLocation.x / scaledTileSize_.width);
    float y = (mapLocation.y / scaledTileSize_.height) - 1;
    
    return CCPointMake(x, y);
}

void
MCObjectLayer::moveTo(const CCPoint &offset)
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
    int edgeTop = mapHeight_ - winHeight_ / 2;
    int edgeBottom = winHeight_ / 2;
    int edgeLeft = winWidth_ / 2;
    int edgeRight = mapWidth_ - winWidth_ / 2;
    
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
    if (mapMaybeMoveToPosition.x < -(mapWidth_ - winWidth_)) { /* 过左 */
        deltaForMap.x -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).x;
    } else if (mapMaybeMoveToPosition.x > 0) { /* 过右 */
        deltaForMap.x -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).x;
    }
    if (mapMaybeMoveToPosition.y < -(mapHeight_ - winHeight_)) { /* 过低 */
        deltaForMap.y -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).y;
    } else if (mapMaybeMoveToPosition.y > 0) { /* 过高 */
        deltaForMap.y -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).y;
    }

    /* 矩形框检测方案 */
    /* 场景切换检测 */
    detectsCollidesWithEntrances(heroMaybeMoveToPositionAtMapForCheck);
    if (detectsCollidesWithNPCs(heroMaybeMoveToPositionAtMapForCheck)) {
        deltaForMap = CCPointZero;
        deltaForHero = CCPointZero;
    }
    
    CCObject *obj;
    CCAction *scrollAction = CCSequence::create(CCMoveBy::create(kMCDuraitonMap, deltaForMap),
                                                CCCallFunc::create(hero_, callfunc_selector(MCRoleEntity::walkEnded)),
                                                NULL);
    map_->runAction(scrollAction);
    if (deltaForMap.x != 0.0f || deltaForMap.y != 0.0f) {
        MCNPC *npc;
        MCMonster *monster;
        if (npcs_) {
            CCARRAY_FOREACH(npcs_, obj) {
                npc = (MCNPC *) obj;
                npc->getEntity()->moveBy(deltaForMap);
            }
        }
        if (monsters_) {
            CCARRAY_FOREACH(monsters_, obj) {
                monster = (MCMonster *) obj;
                monster->getEntity()->moveBy(deltaForMap);
            }
        }
    }
    hero_->walkOnScreen(deltaForHero, offset);
}

void
MCObjectLayer::detectsCollidesWithEntrances(const CCPoint &heroMaybeMoveToPositionAtMapForCheck)
{
    CCObject *obj;
    MCEntrance *entrance;
    CCARRAY_FOREACH(entrances_, obj) {
        entrance = (MCEntrance *) obj;
        if (entrance->collidesWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
            if (entrance->collidesWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
                sceneDelegate_->gotoScene(entrance->getID(), entrance->getDestination()->getCString());
            }
        }
    }
}

bool
MCObjectLayer::detectsCollision(const CCPoint &heroMaybeMoveToPositionAtMapForCheck)
{
    if (detectsCollidesWithSemiTransparents(heroMaybeMoveToPositionAtMapForCheck)
        || detectsCollidesWithBarriers(heroMaybeMoveToPositionAtMapForCheck)
        || detectsCollidesWithNPCs(heroMaybeMoveToPositionAtMapForCheck)
        || detectsCollidesWithMonsters(heroMaybeMoveToPositionAtMapForCheck)
        || detectsCollidesWithMercenaries(heroMaybeMoveToPositionAtMapForCheck)) {
        return true;
    }
    
    return false;
}

bool
MCObjectLayer::detectsCollidesWithSemiTransparents(const CCPoint &heroMaybeMoveToPositionAtMapForCheck)
{
    CCObject *obj;
    MCSemiTransparent *semiTransparent;
    bool shouldBeTransparent = false;
    CCARRAY_FOREACH(semiTransparents_, obj) {
        semiTransparent = (MCSemiTransparent *) obj;
        if (semiTransparent->collidesWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
            if (semiTransparent->collidesWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
                shouldBeTransparent = true;
                break;
            }
        }
    }
    hero_->setOpacity(shouldBeTransparent ? 160 : 255);
    return shouldBeTransparent;
}

bool
MCObjectLayer::detectsCollidesWithBarriers(const CCPoint &heroMaybeMoveToPositionAtMapForCheck)
{
    CCObject *obj;
    MCBarrier *barrier;
    CCARRAY_FOREACH(barriers_, obj) {
        barrier = (MCBarrier *) obj;
        if (barrier->collidesWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
//            if (deltaForHero.x < 1.5) {
//                deltaForCheck.x = 0;
//            } else if (deltaForHero.x > -1.5) {
//                deltaForCheck.x = 0;
//            }
//            if (deltaForHero.y < 1.5) {
//                deltaForCheck.y = 0;
//            } else if (deltaForHero.y > -1.5) {
//                deltaForCheck.y = 0;
//            }
//            
//            heroMaybeMoveToPositionAtMapForCheck = ccpAdd(heroCurrentPositionAtMap, deltaForCheck);
            if (barrier->collidesWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
                return true;
            }
        }
    }
    
    return false;
}

bool
MCObjectLayer::detectsCollidesWithNPCs(const CCPoint &heroMaybeMoveToPositionAtMapForCheck)
{
//    CCObject *obj;
//    MCNPC *npc;
//    CCARRAY_FOREACH(barriers_, obj) {
//        npc = (MCNPC *) obj;
//        if (npc->getEntity()->collidesWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
//            return true;
//        }
//    }
    
    return false;
}

bool
MCObjectLayer::detectsCollidesWithMonsters(const CCPoint &heroMaybeMoveToPositionAtMapForCheck)
{
//    CCObject *obj;
//    MCMonster *monster;
//    CCARRAY_FOREACH(barriers_, obj) {
//        monster = (MCMonster *) obj;
//        if (npc->getEntity()->collidesWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
//            return true;
//        }
//    }
    
    return false;
}

bool
MCObjectLayer::detectsCollidesWithMercenaries(const CCPoint &heroMaybeMoveToPositionAtMapForCheck)
{
//    CCObject *obj;
//    MCMonster *monster;
//    CCARRAY_FOREACH(barriers_, obj) {
//        monster = (MCMonster *) obj;
//        if (npc->getEntity()->collidesWith(hero_, heroMaybeMoveToPositionAtMapForCheck)) {
//            return true;
//        }
//    }
    
    return false;
}
