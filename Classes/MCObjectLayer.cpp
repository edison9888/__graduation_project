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

#include "MCBarrier.h"

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
    float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
    
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
#warning no one
//                break;
                roles->addObject(role);
#warning just one
//                break;
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
    } else if (!scene->getEntranceName().empty()) {
        MCEntrance *entrance = dynamic_cast<MCEntrance *>(scene->getEntrances()
                                                          ->objectForKey(scene->getEntranceName()));
        scene->getEntranceName().clear();
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
    printf("%s(%d): <%.1f %.f1>\n", __FUNCTION__, __LINE__, hero_->getPosition().x, hero_->getPosition().y);
    
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
    MCRole *role;
    MCRoleEntity *entity;
    /* hero */
    hero_->getSpriteSheet()->removeFromParentAndCleanup(false);
    hero_->face(MCFacingDown);
    hero_->stopWalking();
    hero_->stopPathFinding();
    /* 销毁寻路算法实例 */
    hero_->destroyPathFindingAlgorithmInstance();
    /* objects */
    CCARRAY_FOREACH(objects, obj) {
        entity = ((MCRole *) obj)->getEntity();
        entity->getSpriteSheet()->removeFromParentAndCleanup(true);
        entity->face(MCFacingDown);
        entity->stopWalking();
        entity->stopPathFinding();
    }
    /* mercenaries */
    CCARRAY_FOREACH(mercenaries_, obj) {
        role = dynamic_cast<MCRole *>(obj);
        entity = role->getEntity();
        entity->getSpriteSheet()->removeFromParentAndCleanup(true);
        entity->face(MCFacingDown);
        entity->stopWalking();
        entity->stopPathFinding();
        /* 销毁寻路算法实例 */
        entity->destroyPathFindingAlgorithmInstance();
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

/**
 * 返回人物应该移动的值
 */
CCPoint
MCObjectLayer::roleWillMoveBy(MCRole *aRole, const CCPoint &offset)
{
    MCRoleEntity *roleEntity = aRole->getEntity();
    
    /* screen view */
    CCPoint roleCurrentPosition = roleEntity->getPosition();
    CCPoint mapCurrentPosition = map_->getPosition();
    CCPoint delta;
    CCPoint deltaForRole;
    
    /* 障碍物检测用 */
    CCSize spriteSize = roleEntity->getContentSize();
    CCPoint pointForCheck = CCPointZero;
    
    /* 以TMX地图为参照 */
    delta = ccpNormalize(offset);
    delta = ccpMult(delta, 4);
    deltaForRole = CCPoint(delta);

    roleEntity->walk(offset);
    if (deltaForRole.x == 0.0f && deltaForRole.y == 0.0f) {
        return CCPointZero;
    }
    
    CCSize frameSize = roleEntity->getMetadata()->getFrameSize();
    CCRect roleCurrentFrame = CCRectMake(roleCurrentPosition.x - mapCurrentPosition.x,
                                         roleCurrentPosition.y - mapCurrentPosition.y,
                                         frameSize.width,
                                         frameSize.height);
    CCRect roleFrame(roleCurrentFrame);
    roleFrame.origin = ccpAdd(roleFrame.origin, deltaForRole);
    if (roleEntity == hero_) {
        detectsCollidesWithEntrances(roleFrame);
    }
    detectsCollidesWithSemiTransparents(roleFrame);
    
    MCBarrier *barrier = detectsCollidesWithBarriers(roleFrame);
    if (barrier) {
        /* 没解决平滑问题 */
        deltaForRole = CCPointZero;
    }

    return deltaForRole;
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
    obb.
    
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
    MCEntrance *entranceForChangingScene = NULL;
    MCHero *hero = dynamic_cast<MCHero *>(hero_->getPrototype());
    bool atEntrance = false;
    
    CCARRAY_FOREACH(entrances_, obj) {
        entrance = (MCEntrance *) obj;
        if (entrance->contains(anFrame)) {
            /* 全部用push */
            if (hero->atEntrance()) {
                atEntrance = true;
                break;
            }
            entranceForChangingScene = entrance;
            break;
        }
    }
    if (hero->atEntrance() && !atEntrance) {
        hero->setAtEntrance(false);
    }
    if (entranceForChangingScene) {
        sceneDelegate_->gotoScene(entrance->getID(), entrance->getDestination()->getCString());
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

void
MCObjectLayer::detectsCollidesWithSemiTransparents(MCRole *aRole)
{
    MCRoleEntity *roleEntity = aRole->getEntity();
    CCRect roleFrame = roleEntity->getOBB().getAABB();
    
    CCObject *obj;
    MCSemiTransparent *semiTransparent;
    bool shouldBeTransparent = false;
    CCARRAY_FOREACH(semiTransparents_, obj) {
        semiTransparent = (MCSemiTransparent *) obj;
        if (semiTransparent->collidesWith(roleFrame)) {
            shouldBeTransparent = true;
            break;
        }
    }
    roleEntity->setOpacity(shouldBeTransparent ? 160 : 255);
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
            CCRect barrierFrame = barrier->getFrame();
            
            CCPoint intersectsRectOrigin = ccp(MAX(barrierFrame.getMinX(), anFrame.getMinX()),
                                               MAX(barrierFrame.getMinY(), anFrame.getMinY()));
            CCPoint intersectsRectMax = ccp(MIN(barrierFrame.getMaxX(), anFrame.getMaxX()),
                                            MIN(barrierFrame.getMaxY(), anFrame.getMaxY()));
            CCRect intersectsRect = CCRectMake(intersectsRectOrigin.x,
                                               intersectsRectOrigin.y,
                                               intersectsRectMax.x - intersectsRectOrigin.x,
                                               intersectsRectMax.y - intersectsRectOrigin.y);
            CCPoint offset = ccpFromSize(intersectsRect.size);
            
            printf("[<%.1f %.1f>][<%.1f %.1f>] => ",
                  offset.x, offset.y,
                  anOffset.x, anOffset.y);
            
//            feedbackOffset = ccpSub(anOffset, offset);
            feedbackOffset = anOffset;
            
            /* 尝试消除偏移 */
            /* x轴 */
//            frameForCheck.origin = anFrame.origin;
//            frameForCheck.origin.x += anOffset.x;
//            if (barrierFrame.intersectsRect(frameForCheck)) { /* x bingo */
            if (fabsf(anOffset.x) > fabsf(anOffset.y)) { /* x bingo */
                printf("[x] -> ");
//                float dx = fabsf(fabsf(anOffset.x) - fabsf(offset.x));
//                feedbackOffset.x = anOffset.x > 0 ? -intersectsRect.size.width : intersectsRect.size.width;
                feedbackOffset.x = feedbackOffset.x > 0
                                    ? feedbackOffset.x - offset.x
                                    : feedbackOffset.x + offset.x;
            }
//            /* y轴 */
//            frameForCheck.origin = anFrame.origin;
//            frameForCheck.origin.y += anOffset.y;
            else { /* y bingo */
//            if (barrierFrame.intersectsRect(frameForCheck)) { /* y bingo */
                printf("[y] -> ");
//                float dy = fabsf(fabsf(anOffset.y) - fabsf(offset.y));
//                feedbackOffset.y = anOffset.y > 0 ? -intersectsRect.size.height : intersectsRect.size.height;
//                feedbackOffset.y = -feedbackOffset.y;
                feedbackOffset.y = feedbackOffset.y > 0
                                    ? feedbackOffset.y - offset.y
                                    : feedbackOffset.y + offset.y;
            }
            
            return feedbackOffset;
        }
    }
    
    return feedbackOffset;
}

CCSize 
MCObjectLayer::collidedSize(const CCRect &anFrame, const CCPoint &anOffset)
{
    CCObject *obj;
    MCBarrier *barrier;
    CCRect frameForCheck(anFrame);
    CCPoint feedbackOffset = CCPointZero;
    
    frameForCheck.origin = ccpAdd(frameForCheck.origin, anOffset);
    CCARRAY_FOREACH(barriers_, obj) {
        barrier = (MCBarrier *) obj;
        if (barrier->MCSemiTransparent::collidesWith(frameForCheck)) { /* 撞上了 */
            CCRect barrierFrame = barrier->getFrame();
            
            CCPoint intersectsRectOrigin = ccp(MAX(barrierFrame.getMinX(), anFrame.getMinX()),
                                               MAX(barrierFrame.getMinY(), anFrame.getMinY()));
            CCPoint intersectsRectMax = ccp(MIN(barrierFrame.getMaxX(), anFrame.getMaxX()),
                                            MIN(barrierFrame.getMaxY(), anFrame.getMaxY()));
            CCRect intersectsRect = CCRectMake(intersectsRectOrigin.x,
                                               intersectsRectOrigin.y,
                                               intersectsRectMax.x - intersectsRectOrigin.x,
                                               intersectsRectMax.y - intersectsRectOrigin.y);
            return intersectsRect.size;
        }
    }
    
    return CCSizeZero;
}

MCBarrier *
MCObjectLayer::detectsCollidesWithBarriers(const CCRect &anFrame)
{
    CCObject *obj;
    MCBarrier *barrier;
    
    CCARRAY_FOREACH(barriers_, obj) {
        barrier = (MCBarrier *) obj;
        if (barrier->MCSemiTransparent::collidesWith(anFrame)) { /* 撞上了 */
            return barrier;
        }
    }
    
    return NULL;
}
#endif

#pragma mark -
#pragma mark *** MCGameSceneObjectLayer ***

void
MCGameSceneObjectLayer::controllerDidMove(const CCPoint &delta)
{
    MCRole *aSelectedRole = selectedRole();
    
    if (aSelectedRole == NULL) {
        return;
    }
    aSelectedRole->getEntity()->drag(roleWillMoveBy(aSelectedRole, delta));
    getSceneDelegate()->getScene()->getSceneCamera()->focus(aSelectedRole);
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
#pragma mark *** MCBattleFieldSceneObjectLayer::MCJoypadControllerDelegate ***

void
MCBattleFieldSceneObjectLayer::controllerDidMove(const CCPoint &delta)
{
    MCRole *aSelectedRole = selectedRole();
    
    if (aSelectedRole == NULL) {
        return;
    }
    /* 扣体力,一秒钟调度60次，每次调度扣0.1 */
    mc_pp_t pp = aSelectedRole->getPP();
    if (pp > 0.0f) {
        aSelectedRole->getEntity()->move(roleWillMoveBy(aSelectedRole, delta));
        getSceneDelegate()->getScene()->getSceneCamera()->focus(aSelectedRole);
    }
}

void
MCBattleFieldSceneObjectLayer::controllerDidRelease()
{
    MCRole *aSelectedRole = selectedRole();
    
    if (aSelectedRole == NULL) {
        return;
    }
    aSelectedRole->getEntity()->stopWalking();
}

#pragma mark -
#pragma mark *** MCBattleFieldSceneObjectLayer::MCBattleControllerDelegate ***

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
#if MC_SELECT_ALL_SUPPORT == 1
    CCArray *selectedRoles = controller_->getSelectedRoles();
    
    return selectedRoles->count() > 0
            ? dynamic_cast<MCRole *>(selectedRoles->objectAtIndex(0))
            : NULL;
#else
    return controller_->getSelectedRole();
#endif
}
