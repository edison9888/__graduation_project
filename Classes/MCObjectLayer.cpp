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
#include "MCScript.h"
#include "MCDialog.h"
#include "MCBattleController.h"

#include "MCHero.h"
#include "MCBarrier.h"
#include "MCMiscUtil.h"
#include "MCMercenary.h"
#include "MCTeam.h"
#include "MCScene.h"
#include "MCCamera.h"

MCObjectLayer *
MCObjectLayer::create(MCScenePackageType aScenePackageType)
{
    MCObjectLayer *layer;
    
    CCAssert(aScenePackageType != MCUnknownPackage, "unknown scene package type");
    if (aScenePackageType == MCGameScenePackage) {
        layer = new MCGameSceneObjectLayer;
    } else {
        layer = new MCBattleFieldSceneObjectLayer;
    }
    if (layer && layer->init()) {
        layer->scenePackageType_ = aScenePackageType;
        layer->autorelease();
    } else {
        CC_SAFE_DELETE(layer);
        layer = NULL;
    }
    
    return layer;
}

MCObjectLayer::~MCObjectLayer()
{
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
    float contentScaleFactor = CCDirectorGetContentScaleFactor();
    
    map_ = aMap;
    
    /* 添加障碍物 */
    CCTMXObjectGroup *barrierObejctsObejcts = map_->objectGroupNamed("barriers");
    CCArray *barriers = barrierObejctsObejcts->getObjects(); /* 还是原始数据，先处理一次转换成CCRect先 */
    CCObject *obj;
    barriers_ = CCArray::createWithCapacity(barriers->count());
    barriers_->retain();
    CCARRAY_FOREACH(barriers, obj) {
        CCDictionary *dict = (CCDictionary *) obj;
        CCRect rect = CCRectMake(dict->valueForKey("x")->floatValue() / contentScaleFactor,
                                 dict->valueForKey("y")->floatValue() / contentScaleFactor,
                                 dict->valueForKey("width")->floatValue() / contentScaleFactor,
                                 dict->valueForKey("height")->floatValue() / contentScaleFactor);
        MCBarrier *barrier;
        
        if (0 == dict->valueForKey("type")->m_sString.compare(kMCTypeBarrier)) {
            unsigned int barrierType = dict->valueForKey("barrier-type")->uintValue();
            barrier = MCBarrier::create(rect, barrierType == 0
                                        ? MCNormalBarrier | MCAdvancedBarrier
                                        : barrierType);
            barriers_->addObject(barrier);
        }
    }
    
    /* 添加半透明遮掩物 */
    CCTMXObjectGroup *semiTransparentObejcts = map_->objectGroupNamed("semi-transparents");
    semiTransparents_ = CCArray::create();
    semiTransparents_->retain();
    if (semiTransparentObejcts) {
        CCArray *semiTransparents = semiTransparentObejcts->getObjects(); /* 还是原始数据，先处理一次转换成CCRect先 */
        CCARRAY_FOREACH(semiTransparents, obj) {
            CCDictionary *dict = (CCDictionary *) obj;
            CCRect rect = CCRectMake(dict->valueForKey("x")->floatValue() / contentScaleFactor,
                                     dict->valueForKey("y")->floatValue() / contentScaleFactor,
                                     dict->valueForKey("width")->floatValue() / contentScaleFactor,
                                     dict->valueForKey("height")->floatValue() / contentScaleFactor);
            MCSemiTransparent *semiTransparent;
            
            if (0 == dict->valueForKey("type")->m_sString.compare(kMCTypeSemiTransparent)) {
                semiTransparent = MCSemiTransparent::create(rect);
                semiTransparents_->addObject(semiTransparent);
            }
        }
    }
    
    /* 添加入口 */
    CCTMXObjectGroup *entranceObejcts = map_->objectGroupNamed("entrances");
    CCArray *entrances = entranceObejcts->getObjects(); /* 还是原始数据，先处理一次转换成CCRect先 */
    entrances_ = CCArray::createWithCapacity(entrances->count());
    entrances_->retain();
    MCScenePackage *scenePackage = sceneDelegate_->getScenePackage();
    CCDictionary *scenes = scenePackage->getScenes(); /* 场景包中记录的entrances */
    CCARRAY_FOREACH(entrances, obj) {
        CCDictionary *dict = (CCDictionary *) obj;
        CCRect rect = CCRectMake(dict->valueForKey("x")->floatValue() / contentScaleFactor,
                                 dict->valueForKey("y")->floatValue() / contentScaleFactor,
                                 dict->valueForKey("width")->floatValue() / contentScaleFactor,
                                 dict->valueForKey("height")->floatValue() / contentScaleFactor);
        MCEntrance *entrance;
        
        if (0 == dict->valueForKey("type")->m_sString.compare(kMCTypeEntrance)) {
            /**
             * 除了坐标之外的内容已经在加载数据包的时候完成，现在加载坐标内容。
             */
            entrance = (MCEntrance *) scenes->objectForKey(dict->valueForKey("name")->getCString());
            entrance->setup(rect);
            entrances_->addObject(entrance);
        }
    }
    
    CCSize mapSize = map_->getMapSize();
    CCSize tileSize = map_->getTileSize();
    contentScaleFactor_ = contentScaleFactor;
    mapWidth_ = mapSize.width * tileSize.width / contentScaleFactor;
    mapHeight_ = mapSize.height * tileSize.height / contentScaleFactor;
    
    mapSize_ = CCSizeMake(mapWidth_, mapHeight_);
    
    winWidth_ = winSize.width;
    winHeight_ = winSize.height;
}

void
MCObjectLayer::onEnter()
{
    CCLayer::onEnter();
    MCSceneContext *context = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = context->getScene();
    MCHero *hero = MCHero::sharedHero();
    MCRole *role;
    MCRoleEntity *roleEntity;
    float contentScaleFactor = contentScaleFactor_;
    CCArray *team;
    CCObject *obj;
    CCPoint mapPosition = map_->getPosition();
    
    hero_ = hero->getEntity();
    /* tags: #objects #insert */
    /* load all roles */
    /* objects NPC or enemy */
    
    /* add objects */
    CCArray *roles = context->objects_;
    if (! context->inited_) {
        MCScenePackage *package = scene->getScenePackage();
        CCArray *objects = package->getObjects();
        CCArray *flags;
        bool has;
        
        CCARRAY_FOREACH(objects, obj) {
            role = dynamic_cast<MCRole *>(obj);
            /* 初始化NPC数据 */
            has = false;
            if (scenePackageType_ == MCBattleFieldScenePackage
                && role->getRoleType() == MCRole::MCEnemy) {
                flags = role->getEntityMetadata()->getFlags();
                CCARRAY_FOREACH(flags, obj) {
                    if (dynamic_cast<MCFlag *>(obj)->isOnState()) {
                        has = true;
                        break;
                    }
                }
            } else if (role->getRoleType() == MCRole::MCNPC) {
                has = true;
            }
            if (has) {
                roles->addObject(role);
            }
        }
        
        context->inited_ = true;
    }
    
    CCARRAY_FOREACH(roles, obj) {
        role = dynamic_cast<MCRole *>(obj);
        addChild(role->getEntity()->getSpriteSheet());
        CCPoint positionAtTMX = role->getEntityMetadata()->getPosition();
        positionAtTMX = ccp(positionAtTMX.x / contentScaleFactor,
                            mapHeight_ - positionAtTMX.y / contentScaleFactor);
        role->getEntity()->setPosition(ccpAdd(positionAtTMX, mapPosition));
        
    }
    
    /* hero */
    CCSpriteBatchNode *spriteSheet = hero_->getSpriteSheet();
    addChild(spriteSheet);
    
    /* tags: #spawn-point */
    /* load initialize position */
    CCTMXObjectGroup *objects = map_->objectGroupNamed("objects");
    CCDictionary *spawnPoint = objects->objectNamed("spawn-point");
    MCFlagManager *flagManager = MCFlagManager::sharedFlagManager();
    CCPoint rolePosition;
    bool atEntrance = false;
    if (spawnPoint && flagManager->isSpawnFlagOn()) {
        rolePosition = ccp(spawnPoint->valueForKey("x")->floatValue() / contentScaleFactor,
                           spawnPoint->valueForKey("y")->floatValue() / contentScaleFactor);
        flagManager->setSpawnFlagOff();
    } else if (scene->getEntranceName()) {
        MCEntrance *entrance = dynamic_cast<MCEntrance *>(scene->getEntrances()
                                                          ->objectForKey(scene->getEntranceName()
                                                                         ->getCString()));
        scene->getEntranceName()->release();
        CCAssert(entrance != NULL, "场景没有入口！");
#if (MC_COLLISION_USE_OBB == 1)
        MCOBB obb = entrance->getOBB();
        rolePosition = ccpAdd(obb.getOrigin(), mapPosition);
        rolePosition.x += obb.extents.width - hero_->getOBB().extents.width;
        rolePosition.y += obb.extents.height - hero_->getOBB().extents.height;
#else
        CCRect frame = entrance->getFrame();
        rolePosition = ccpAdd(frame.origin, mapPosition);
        rolePosition.x += (frame.size.width - hero_->getOBB().width) / 2;
        rolePosition.y += (frame.size.height - hero_->getOBB().height) / 2;
#endif
        atEntrance = true;
    } else {
#warning debug
        rolePosition = ccp(200, 200);
//        CCAssert(spawnPoint != NULL, "没有入口，至少应该有个重生点，不然就逻辑错误了。");
//        rolePosition = ccp(spawnPoint->valueForKey("x")->floatValue() / contentScaleFactor,
//                           spawnPoint->valueForKey("y")->floatValue() / contentScaleFactor);
    }
    hero->setAtEntrance(atEntrance);
    hero_->setPosition(rolePosition);
    CCPointLog(hero_->getPosition());
    
    /* mercenaries */
    mercenaries_->removeAllObjects();
    if (scenePackageType_ == MCBattleFieldScenePackage) {
        team = MCTeam::sharedTeam()->getRoles();
        CCARRAY_FOREACH(team, obj) {
            MCRole *role = dynamic_cast<MCRole *>(obj);
            if (role != hero) {
                roleEntity = role->getEntity();
                addChild(roleEntity->getSpriteSheet());
                roleEntity->setPosition(rolePosition);
                mercenaries_->addObject(role);
            }
        }
    }
}

void
MCObjectLayer::onExit()
{
    MCSceneContext *context = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    CCArray *objects = context->objects_;
    /* remove all roles */
    CCObject *obj;
    MCRoleEntity *entity;
    /* hero */
    hero_->getSpriteSheet()->removeFromParent();
    hero_->face(MCFacingDown);
    /* objects */
    CCARRAY_FOREACH(objects, obj) {
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

CCArray *
MCObjectLayer::objects() const
{
    return MCSceneContextManager::sharedSceneContextManager()->currentContext()->getObjects();
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
MCObjectLayer::setSceneOffset(const CCPoint &anOffset)
{
    MCSceneContext *context = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    CCArray *objects = context->objects_;
    CCObject *obj;
    
    /* 不做无用功 */
    if (anOffset.x == 0 && anOffset.y == 0) {
        return;
    }
    
    map_->setPosition(ccpAdd(map_->getPosition(), anOffset));
    MCRole *role;
    if (objects) {
        CCARRAY_FOREACH(objects, obj) {
            role = (MCRole *) obj;
            role->getEntity()->drag(anOffset);
        }
    }
    objects = mercenaries_;
    if (objects) {
        CCARRAY_FOREACH(objects, obj) {
            role = (MCRole *) obj;
            role->getEntity()->drag(anOffset);
        }
    }
    hero_->drag(anOffset);
}

void
MCObjectLayer::moveTo(const CCPoint &offset)
{
    MCRole *aSelectedRole = selectedRole();
    
    if (aSelectedRole == NULL) {
        return;
    }
    
    MCRoleEntity *roleEntity = aSelectedRole->getEntity();
    
    MCCamera *camera = getSceneDelegate()->getScene()->getSceneCamera();
    /* screen view */
    CCPoint heroCurrentPosition = roleEntity->getPosition();
    CCPoint mapCurrentPosition = map_->getPosition();
    CCPoint heroCurrentPositionAtMap = ccpSub(heroCurrentPosition, mapCurrentPosition);
    CCPoint heroMaybeMoveToPosition;
    CCPoint heroMaybeMoveToPositionAtMap;
    CCPoint mapMaybeMoveToPosition;
    CCPoint delta;
    CCPoint deltaForMap;
    CCPoint deltaForRole;
    CCPoint deltaForCheck;
    
    /* 障碍物检测用 */
    CCSize spriteSize = roleEntity->getContentSize();
    CCPoint pointForCheck = CCPointZero;
    
    /* 以TMX地图为参照 */
    delta = ccpNormalize(offset);
    delta = ccpMult(delta, 4);
    deltaForMap = ccpNeg(delta);
    deltaForRole = CCPoint(delta);
    deltaForCheck = CCPoint(deltaForRole);
    
    /* 地图移动检测用 */
    int edgeTop = mapHeight_ - winHeight_ / 2 - (int) (offset.y > 0 ? 0 : delta.y);
    int edgeBottom = winHeight_ / 2 - (int) (offset.y < 0 ? delta.y : 0);
    int edgeLeft = winWidth_ / 2 - (int) (offset.x < 0 ? delta.x : 0);
    int edgeRight = mapWidth_ - winWidth_ / 2 - (int) (offset.x > 0 ? 0 : delta.x);
    
    /* 让移动更平滑~~~~ */
    if (deltaForRole.x > -1.5 && deltaForRole.x < 1.5) {
        deltaForRole.x = 0;
        deltaForCheck.x = deltaForCheck.x > 0 ? -0.5 : 0.5;
    }
    if (deltaForRole.y > -1.5 && deltaForRole.y < 1.5) {
        deltaForRole.y = 0;
        deltaForCheck.y = deltaForCheck.y > 0 ? -0.5 : 0.5;
    }
    
    heroMaybeMoveToPositionAtMap = ccpAdd(heroCurrentPositionAtMap, deltaForRole);
    
    if ((int) floorf(heroMaybeMoveToPositionAtMap.x) > edgeLeft
        && (int) floorf(heroMaybeMoveToPositionAtMap.x) < edgeRight) { /* 移动地图 */
        deltaForRole.x = 0;
    } else {
        deltaForMap.x = 0;
    }
    
    if ((int) floorf(heroMaybeMoveToPositionAtMap.y) > edgeBottom
        && (int) floorf(heroMaybeMoveToPositionAtMap.y) < edgeTop) { /* 移动地图 */
        deltaForRole.y = 0;
    } else {
        deltaForMap.y = 0;
    }
    
    heroMaybeMoveToPosition = ccpAdd(heroCurrentPosition, deltaForRole);
    mapMaybeMoveToPosition = ccpAdd(mapCurrentPosition, deltaForMap);
    
    /* tags: #map #offset */
    /* 检测地图的越界偏移 */
    CCPoint mapDefultLocation = camera->getLocation();
    if (mapMaybeMoveToPosition.x < -(mapWidth_ - winWidth_)) { /* 过左 */
        deltaForMap.x -= mapMaybeMoveToPosition.x - (float) -(mapWidth_ - winWidth_);
    } else if (mapMaybeMoveToPosition.x > mapDefultLocation.x) { /* 过右 */
        deltaForMap.x -= mapMaybeMoveToPosition.x - mapDefultLocation.x;
    }
    if (mapMaybeMoveToPosition.y < -(mapHeight_ - winHeight_)) { /* 过低 */
        deltaForMap.y -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).y;
    } else if (mapMaybeMoveToPosition.y > mapDefultLocation.y) { /* 过高 */
        deltaForMap.y -= mapMaybeMoveToPosition.y - mapDefultLocation.y;
    }
    
    /* tags: #collision */
    /* 矩形框检测方案 */
    /* 场景切换检测 */
#if (MC_COLLISION_USE_OBB == 1)
    MCOBB heroOBB = roleEntity->getOBB();
    /* recal origin */
    heroOBB.setup(deltaForCheck);
    if (roleEntity == hero_) {
        detectsCollidesWithEntrances(heroOBB);
    }
    detectsCollidesWithSemiTransparents(heroOBB);
    if (detectsCollision(heroOBB)) {
        deltaForMap = CCPointZero;
        deltaForHero = CCPointZero;
    }
#else
//    CCRect heroCurrentFrame = roleEntity->getOBB().getAABB();
    CCSize frameSize = roleEntity->getMetadata()->getFrameSize();
    CCRect roleCurrentFrame = CCRectMake(heroCurrentPosition.x - mapCurrentPosition.x,heroCurrentPosition.y - mapCurrentPosition.y,
                                         frameSize.width,frameSize.height);
    CCRect roleFrame(roleCurrentFrame);
    roleFrame.origin = ccpAdd(roleFrame.origin, deltaForCheck);
    if (roleEntity == hero_) {
        detectsCollidesWithEntrances(roleFrame);
    }
    detectsCollidesWithSemiTransparents(roleFrame);
//    deltaForHero = ccpAdd(deltaForHero, feedbackOffset);
//    deltaForMap = ccpSub(deltaForMap, feedbackOffset);
//    if (/*deltaForHero.x == 0.0f && deltaForHero.y == 0.0f
//        &&*/ (feedbackOffset.x != 0.0f || feedbackOffset.y != 0.0f)) {
#if 1
    if (detectsCollidesWithBarriers(roleFrame)) {
        deltaForMap = CCPointZero;
        deltaForRole = CCPointZero;
    }
#else
    CCPoint feedbackOffset = detectsCollidesWithBarriers(heroCurrentFrame, deltaForCheck);
    printf("H<%.0f %.0f> - M<%.0f %.0f> - F<%.0f %.0f>\t|\t",
           deltaForHero.x, deltaForHero.y,
           deltaForMap.x, deltaForMap.y,
           feedbackOffset.x, feedbackOffset.y);
    deltaForHero = ccpAdd(deltaForHero, feedbackOffset);
    printf("H<%.0f %.0f> - ",
           deltaForHero.x, deltaForHero.y);
        //    if (deltaForHero.x == 0.0f && deltaForHero.y == 0.0f) {
        //    if (deltaForMap.x != 0.0f || deltaForMap.y != 0.0f) {
    deltaForMap = ccpSub(deltaForMap, feedbackOffset);
        //    }
    printf("M<%.0f %.0f> \n",
           deltaForMap.x, deltaForMap.y);
#endif
#endif
    
    if (deltaForRole.x == 0.0f && deltaForRole.y == 0.0f
        && deltaForMap.x == 0.0f && deltaForMap.y == 0.0f) {
        return;
    }
    
    roleEntity->walk(offset);
    roleEntity->moveBy(deltaForRole);
    if (deltaForMap.x != 0.0f || deltaForMap.y != 0.0f) {
        camera->translate(deltaForMap);
        camera->locate();
        roleEntity->moveBy(ccpNeg(deltaForMap));
    }
}

#if (MC_COLLISION_USE_OBB == 1)
void
MCObjectLayer::detectsCollidesWithEntrances(const MCOBB &anOBB)
{
    CCObject *obj;
    MCEntrance *entrance;
    MCHero *hero = dynamic_cast<MCHero *>(hero_->getPrototype());
    bool atEntrance = false;
    CCARRAY_FOREACH(entrances_, obj) {
        entrance = (MCEntrance *) obj;
#warning 暂时使用OBB检测，修改检测是否站在入口的方法之后再改为contains
        if (entrance->collidesWith(anOBB)) {
                //        if (entrance->contains(anOBB.getAABB())) {
            /* 全部用push */
            if (hero->atEntrance()) {
                atEntrance = true;
                break;
            }
            sceneDelegate_->gotoScene(entrance->getID(), entrance->getDestination()->getCString());
            break;
        }
    }
    if (hero->atEntrance() && !atEntrance) {
        hero->setAtEntrance(false);
    }
}

void
MCObjectLayer::detectsCollidesWithEntrances(const MCOBB &anOBB, const CCPoint &anOffset)
{
    MCOBB obb(anOBB);
    
    obb.center = ccpAdd(anOBB.center, anOffset);
    detectsCollidesWithEntrances(obb);
}

bool
MCObjectLayer::detectsCollision(const MCOBB &anOBB)
{
    if (detectsCollidesWithBarriers(anOBB)
        //        || detectsCollidesWithObjects(anOBB)
        || detectsCollidesWithMercenaries(anOBB)) {
        return true;
    }
    
    return false;
}

bool
MCObjectLayer::detectsCollision(const MCOBB &anOBB, const cocos2d::CCPoint &anOffset)
{
    MCOBB obb(anOBB);
    
    obb.center = ccpAdd(anOBB.center, anOffset);
    return detectsCollision(obb);
}

void
MCObjectLayer::detectsCollidesWithSemiTransparents(const MCOBB &anOBB)
{
    CCObject *obj;
    MCSemiTransparent *semiTransparent;
    bool shouldBeTransparent = false;
    CCARRAY_FOREACH(semiTransparents_, obj) {
        semiTransparent = (MCSemiTransparent *) obj;
        if (semiTransparent->collidesWith(anOBB)) {
            shouldBeTransparent = true;
            break;
        }
    }
    hero_->setOpacity(shouldBeTransparent ? 160 : 255);
}

void
MCObjectLayer::detectsCollidesWithSemiTransparents(const MCOBB &anOBB, const cocos2d::CCPoint &anOffset)
{
    MCOBB obb(anOBB);
    
    obb.center = ccpAdd(anOBB.center, anOffset);
    detectsCollidesWithSemiTransparents(obb);
}

bool
MCObjectLayer::detectsCollidesWithBarriers(const MCOBB &anOBB)
{
    CCObject *obj;
    MCBarrier *barrier;
    CCARRAY_FOREACH(barriers_, obj) {
        barrier = (MCBarrier *) obj;
        if (barrier->MCSemiTransparent::collidesWith(anOBB)) {
            return true;
        }
    }
    
    return false;
}

bool
MCObjectLayer::detectsCollidesWithBarriers(const MCOBB &anOBB, const cocos2d::CCPoint &anOffset)
{
    MCOBB obb(anOBB);
    
    obb.center = ccpAdd(anOBB.center, anOffset);
    return detectsCollidesWithBarriers(obb);
}
#else
void
MCObjectLayer::detectsCollidesWithEntrances(const CCRect &anFrame)
{
    CCObject *obj;
    MCEntrance *entrance;
    MCHero *hero = dynamic_cast<MCHero *>(hero_->getPrototype());
    bool atEntrance = false;
    CCARRAY_FOREACH(entrances_, obj) {
        entrance = (MCEntrance *) obj;
#warning 暂时使用OBB检测，修改检测是否站在入口的方法之后再改为contains
        if (entrance->collidesWith(anFrame)) {
            /* 全部用push */
            if (hero->atEntrance()) {
                atEntrance = true;
                break;
            }
            sceneDelegate_->gotoScene(entrance->getID(), entrance->getDestination()->getCString());
            break;
        }
    }
    if (hero->atEntrance() && !atEntrance) {
        hero->setAtEntrance(false);
    }
}

void
MCObjectLayer::detectsCollidesWithEntrances(const CCRect &anFrame, const CCPoint &anOffset)
{
    CCRect frame(anFrame);
    
    frame.origin = ccpAdd(frame.origin, anOffset);
    
    detectsCollidesWithEntrances(frame);
}

bool
MCObjectLayer::detectsCollision(const CCRect &anFrame)
{
//    if (detectsCollidesWithBarriers(anFrame))
//        return true;
//    }
    
    return false;
}

bool
MCObjectLayer::detectsCollision(const CCRect &anFrame, const CCPoint &anOffset)
{
    CCRect frame(anFrame);
    
    frame.origin = ccpAdd(frame.origin, anOffset);
    
    return detectsCollision(frame);
}

void
MCObjectLayer::detectsCollidesWithSemiTransparents(const CCRect &anFrame)
{
    CCObject *obj;
    MCSemiTransparent *semiTransparent;
    bool shouldBeTransparent = false;
    CCARRAY_FOREACH(semiTransparents_, obj) {
        semiTransparent = (MCSemiTransparent *) obj;
        if (semiTransparent->collidesWith(anFrame)) {
            shouldBeTransparent = true;
            break;
        }
    }
    hero_->setOpacity(shouldBeTransparent ? 160 : 255);
}

void
MCObjectLayer::detectsCollidesWithSemiTransparents(const CCRect &anFrame, const CCPoint &anOffset)
{
    CCRect frame(anFrame);
    
    frame.origin = ccpAdd(frame.origin, anOffset);
    
    detectsCollidesWithSemiTransparents(frame);
}

CCPoint
MCObjectLayer::detectsCollidesWithBarriers(const CCRect &anFrame, const CCPoint &anOffset)
{
    CCObject *obj;
    MCBarrier *barrier;
    CCRect frameForCheck(anFrame);
    CCPoint feedbackOffset = CCPointZero;
    
    frameForCheck.origin = ccpAdd(frameForCheck.origin, anOffset);
    CCARRAY_FOREACH(barriers_, obj) {
        barrier = (MCBarrier *) obj;
        if (barrier->MCSemiTransparent::collidesWith(frameForCheck)) { /* 撞上了 */
            /* 尝试消除偏移 */
            /* x轴 */
            frameForCheck.origin = anFrame.origin;
            frameForCheck.origin.x += anOffset.x;
            if (barrier->MCSemiTransparent::collidesWith(frameForCheck)) { /* x bingo */
                feedbackOffset.x = -anOffset.x;
            }
            /* y轴 */
            frameForCheck.origin = anFrame.origin;
            frameForCheck.origin.y += anOffset.y;
            if (barrier->MCSemiTransparent::collidesWith(frameForCheck)) { /* y bingo */
                feedbackOffset.y = -anOffset.y;
            }
            
            break;
        }
    }
    
    return feedbackOffset;
}
bool
MCObjectLayer::detectsCollidesWithBarriers(const CCRect &anFrame)
{
    CCObject *obj;
    MCBarrier *barrier;
    
    CCARRAY_FOREACH(barriers_, obj) {
        barrier = (MCBarrier *) obj;
        if (barrier->MCSemiTransparent::collidesWith(anFrame)) { /* 撞上了 */
            return true;
        }
    }
    
    return false;
}
#endif

#pragma mark -
#pragma mark *** MCGameSceneObjectLayer ***

void
MCGameSceneObjectLayer::controllerDidMove(const CCPoint &delta)
{
    moveTo(delta);
}

void
MCGameSceneObjectLayer::controllerDidRelease()
{
    MCRole *aSelectedRole = selectedRole();
    
    if (aSelectedRole == NULL) {
        return;
    }
    aSelectedRole->getEntity()->stopWalking();
}

/**
 * 行动
 */
void
MCGameSceneObjectLayer::controllerDidActivate()
{
    MCHero *hero = MCHero::sharedHero();
    MCRole *role = hero->roleOfFront();
    if (role) {
        /* 触发对话，对话结束后若有脚本则执行脚本 */
        CCString *dialogue = role->getDefaultDialogue();
        if (dialogue) {
            MCDialog *dialog = MCDialog::sharedDialog(role->getRoleType() == MCRole::MCNPC
                                                      ? MCNPCDialog
                                                      : MCDMDialog);
            dialog->setMessage(dialogue->getCString());
            dialog->setDismissSelector(this,
                                       dismiss_selector(MCGameSceneObjectLayer::dialogDidDismiss),
                                       role);
            dialog->attach(this);
            sceneDelegate_->pauseScene();
        }
    }
}

void
MCGameSceneObjectLayer::dialogDidDismiss(void *anUserdata)
{
    MCRole *role = (MCRole *)anUserdata;
    MCScript *trigger = role->getTrigger();
    sceneDelegate_->resumeScene();
    if (trigger) {
        trigger->run();
    }
}

#pragma mark -
#pragma mark *** MCBattleFieldSceneObjectLayer ***

#pragma mark -
#pragma mark *** MCBattleFieldSceneObjectLayer::控制器 ***

void
MCBattleFieldSceneObjectLayer::controllerDidSelectRole(MCBattleController *aBattleController, MCRole *aSelectedRole)
{
}

/**
 * 取消了选择的人物。
 */
void
MCBattleFieldSceneObjectLayer::controllerDidUnselectRole(MCBattleController *aBattleController, MCRole *aSelectedRole)
{
}

/**
 * 选择全部，若已经全部选择，则全部取消选择。
 */
void
MCBattleFieldSceneObjectLayer::controllerDidSelectAll(MCBattleController *aBattleController, MCTeam *aTeam)
{
}

/**
 * 全部取消选择。
 */
void
MCBattleFieldSceneObjectLayer::controllerDidUnselectAll(MCBattleController *aBattleController, MCTeam *aTeam)
{
}

/**
 * 进入多选模式
 */
void
MCBattleFieldSceneObjectLayer::controllerDidEnterMultiSelectionMode(MCBattleController *aBattleController)
{
}

/**
 * 退出多选模式
 */
void
MCBattleFieldSceneObjectLayer::controllerDidExitMultiSelectionMode(MCBattleController *aBattleController)
{
}

void
MCBattleFieldSceneObjectLayer::controllerDidFocus(MCBattleController *aBattleController, MCRole *aRole)
{
    sceneDelegate_->getScene()->getSceneCamera()->focus(aRole);
}

/**
 * 在选择了人物的情况下，指定移动到某个位置(在地图上的)
 */
void
MCBattleFieldSceneObjectLayer::controllerDidPointTo(MCBattleController *aBattleController, const CCPoint &locationAtMap)
{
    
}

/**
 * 是否允许拖动anItem。按下图标的时候执行。
 */
bool
MCBattleFieldSceneObjectLayer::controllerShouldDragItem(MCBattleController *aBattleController, MCItem *anItem)
{
    return false;
}

/**
 * 将要开始拖动anItem。按下图标后，首次移动anItem的时候执行。
 */
void
MCBattleFieldSceneObjectLayer::controllerWillDragItem(MCBattleController *aBattleController, MCItem *anItem)
{
    
}

/**
 * 拖动完anItem，拖动到了人物aRole上，若aRole等于NULL，则表示没有拖动到任何人物上。放开anItem的时候执行。
 */
void
MCBattleFieldSceneObjectLayer::controllerDidFinishDragItem(MCBattleController *aBattleController, MCItem *anItem, MCRole *aRole)
{
    
}

/**
 * 选择了anItem。按下和放开手指都在anItem的范围内时执行。
 */
void
MCBattleFieldSceneObjectLayer::controllerDidSelectItem(MCBattleController *aBattleController, MCItem *anItem)
{
    
}

/**
 * 拖动地图
 */
void
MCBattleFieldSceneObjectLayer::controllerDidDragMap(MCBattleController *aBattleController, const CCPoint &anOffset)
{
    MCCamera *camera = sceneDelegate_->getScene()->getSceneCamera();
    CCPoint mapCurrentPosition = map_->getPosition();
    CCPoint mapMaybeMoveToPosition = ccpAdd(mapCurrentPosition, anOffset);
    CCPoint delta(anOffset);
    
    /* 检测地图的越界偏移 */
    if (mapMaybeMoveToPosition.x < -(mapWidth_ - winWidth_)) { /* 过左 */
        delta.x -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).x;
    } else if (mapMaybeMoveToPosition.x > 0) { /* 过右 */
        delta.x -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).x;
    }
    if (mapMaybeMoveToPosition.y < -(mapHeight_ - winHeight_)) { /* 过低 */
        delta.y -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).y;
    } else if (mapMaybeMoveToPosition.y > 0) { /* 过高 */
        delta.y -= (ccpSub(mapMaybeMoveToPosition, mapCurrentPosition)).y;
    }
    
    camera->translate(delta);
    camera->locate();
}

void
MCBattleFieldSceneObjectLayer::controllerDidAttach(MCBattleController *aBattleController)
{
    controller_ = aBattleController;
}

MCRole *
MCBattleFieldSceneObjectLayer::selectedRole()
{
    CCArray *selectedRoles = controller_->getSelectedRoles();
    
    return selectedRoles->count() > 0
    ? dynamic_cast<MCRole *>(selectedRoles->objectAtIndex(0))
    : NULL;
}
