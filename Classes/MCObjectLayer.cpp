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

#define kMCDuraitonMap   0.025f

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
//    CC_SAFE_RELEASE(objects_);
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
        
//        objects_ = CCArray::create();
//        objects_->retain();
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
        CCDictionary *dataDict = package->getObjects();
        CCDictElement *elem;
        CCArray *flags;
        bool has;
        
        CCDICT_FOREACH(dataDict, elem) {
            role = dynamic_cast<MCRole *>(elem->getObject());
            /* 初始化NPC数据 */
            has = false;
            if (scenePackageType_ == MCBattleFieldScenePackage) {
                flags = role->getEntityMetadata()->getFlags();
                CCARRAY_FOREACH(flags, obj) {
                    if (dynamic_cast<MCFlag *>(obj)->isOnState()) {
                        has = true;
                        break;
                    }
                }
#warning 判断是否应该添加进去
            } else {
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
    
    /* mercenaries */
    mercenaries_->removeAllObjects();
    if (scenePackageType_ == MCBattleFieldScenePackage) {
        team = MCTeam::sharedTeam()->getRoles();
        CCARRAY_FOREACH(team, obj) {
            MCRole *role = dynamic_cast<MCRole *>(obj);
            if (role != hero) {
                addChild(role->getEntity()->getSpriteSheet());
                mercenaries_->addObject(role);
            }
        }
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
        if (spawnPoint) {
            rolePosition = ccp(spawnPoint->valueForKey("x")->floatValue() / contentScaleFactor,
                               spawnPoint->valueForKey("y")->floatValue() / contentScaleFactor);
            flagManager->setSpawnFlagOff();
        }
    } else if (scene->getEntranceName()) {
        MCEntrance *entrance = dynamic_cast<MCEntrance *>(scene->getEntrances()
                                                                ->objectForKey(scene->getEntranceName()
                                                                                    ->getCString()));
        scene->getEntranceName()->release();
        CCAssert(entrance != NULL, "场景没有入口！");
        MCOBB obb = entrance->getOBB();
        rolePosition = ccpAdd(obb.getOrigin(), map_->getPosition());
        rolePosition.x += obb.extents.width - hero_->getOBB().extents.width;
        rolePosition.y += obb.extents.height - hero_->getOBB().extents.height;
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
    hero_->drag(anOffset);
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
    
    /* tags: #map #offset */
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
    
    /* tags: #collision */
    /* 矩形框检测方案 */
    /* 场景切换检测 */
    MCOBB heroOBB = hero_->getOBB();
    /* recal origin */
    heroOBB.setup(deltaForCheck);
    detectsCollidesWithEntrances(heroOBB);
    detectsCollidesWithSemiTransparents(heroOBB);
    if (detectsCollision(heroOBB)) {
        deltaForMap = CCPointZero;
        deltaForHero = CCPointZero;
    }
    
    CCObject *obj;
    CCAction *scrollAction = CCSequence::create(CCMoveBy::create(kMCDuraitonMap, deltaForMap),
                                                CCCallFunc::create(hero_, callfunc_selector(MCRoleEntity::walkEnded)),
                                                NULL);
    map_->runAction(scrollAction);
    if (deltaForMap.x != 0.0f || deltaForMap.y != 0.0f) {
        MCSceneContext *context = MCSceneContextManager::sharedSceneContextManager()->currentContext();
        CCArray *objects = context->objects_;
        MCRole *role;
        if (objects) {
            CCARRAY_FOREACH(objects, obj) {
                role = (MCRole *) obj;
                role->getEntity()->moveBy(deltaForMap);
            }
        }
    }
    hero_->walkOnScreen(deltaForHero, offset);
    getSceneDelegate()->getScene()->getSceneCamera()->translate(deltaForMap);
}

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

//bool
//MCObjectLayer::detectsCollidesWithObjects(const MCOBB &anOBB)
//{
//    CCObject *obj;
//    MCRole *role;
//    CCARRAY_FOREACH(objects_, obj) {
//        role = (MCRole *) obj;
//        MCOBB obb = role->getEntity()->getOBB();
//        if (obb.collidesWith(anOBB)) {
//            return true;
//        }
//    }
//    
//    return false;
//}
//
//bool
//MCObjectLayer::detectsCollidesWithObjects(const MCOBB &anOBB, const cocos2d::CCPoint &anOffset)
//{
//    MCOBB obb(anOBB);
//    
//    obb.center = ccpAdd(anOBB.center, anOffset);
//    return detectsCollidesWithObjects(obb);
//}

#pragma mark -
#pragma mark *** MCGameSceneObjectLayer ***

void
MCGameSceneObjectLayer::controllerDidMove(MCJoypadControllerDelegate *sender, const CCPoint &delta)
{
    moveTo(delta);
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

bool
MCBattleFieldSceneObjectLayer::detectsCollidesWithMercenaries(const MCOBB &anOBB)
{
    return false;
}

bool
MCBattleFieldSceneObjectLayer::detectsCollidesWithMercenaries(const MCOBB &anOBB, const cocos2d::CCPoint &anOffset)
{
    return false;
}

#pragma mark -
#pragma mark *** MCBattleFieldSceneObjectLayer::控制器 ***

void 
MCBattleFieldSceneObjectLayer::controllerDidSelectRole(MCBattleControllerDelegate *aSender, MCRole *aSelectedRole)
{
    
}

/**
 * 取消了选择的人物。
 */
void 
MCBattleFieldSceneObjectLayer::controllerDidUnselectRole(MCBattleControllerDelegate *aSender, MCRole *aSelectedRole)
{
    
}

/**
 * 选择全部，若已经全部选择，则全部取消选择。
 */
void
MCBattleFieldSceneObjectLayer::controllerDidSelectAll(MCBattleControllerDelegate *aSender, MCTeam *aTeam)
{
    
}

/**
 * 全部取消选择。
 */
void
MCBattleFieldSceneObjectLayer::controllerDidUnselectAll(MCBattleControllerDelegate *aSender, MCTeam *aTeam)
{
    
}

/**
 * 进入多选模式
 */
void
MCBattleFieldSceneObjectLayer::controllerDidEnterMultiSelectionMode(MCBattleControllerDelegate *aSender)
{
}

/**
 * 退出多选模式
 */
void
MCBattleFieldSceneObjectLayer::controllerDidExitMultiSelectionMode(MCBattleControllerDelegate *aSender)
{
}

/**
 * 在选择了人物的情况下，指定移动到某个位置(在地图上的)
 */
void
MCBattleFieldSceneObjectLayer::controllerDidPointTo(MCBattleControllerDelegate *aSender, const CCPoint &locationAtMap)
{
    
}

/**
 * 是否允许拖动anItem。按下图标的时候执行。
 */
bool
MCBattleFieldSceneObjectLayer::controllerShouldDragItem(MCBattleControllerDelegate *aSender, MCItem *anItem)
{
    return false;
}

/**
 * 将要开始拖动anItem。按下图标后，首次移动anItem的时候执行。
 */
void 
MCBattleFieldSceneObjectLayer::controllerWillDragItem(MCBattleControllerDelegate *aSender, MCItem *anItem)
{
    
}

/**
 * 拖动完anItem，拖动到了人物aRole上，若aRole等于NULL，则表示没有拖动到任何人物上。放开anItem的时候执行。
 */
void 
MCBattleFieldSceneObjectLayer::controllerDidFinishDragItem(MCBattleControllerDelegate *aSender, MCItem *anItem, MCRole *aRole)
{
    
}

/**
 * 选择了anItem。按下和放开手指都在anItem的范围内时执行。
 */
void 
MCBattleFieldSceneObjectLayer::controllerDidSelectItem(MCBattleControllerDelegate *aSender, MCItem *anItem)
{
    
}

/**
 * 拖动地图
 */
void
MCBattleFieldSceneObjectLayer::controllerDidDragMap(MCBattleControllerDelegate *aSender, const CCPoint &anOffset)
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
