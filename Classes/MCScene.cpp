//
//  MCScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-15.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCSceneController.h"
#include "MCScene.h"
#include "MCCamera.h"
#include "MCControllerLayer.h"
#include "MCEntrance.h"
#include "MCAStar.h"
#include "MCTeam.h"
#include "MCFlagManager.h"
#include "MCObjectLayer.h"
#include "MCTaskManager.h"
#include "MCScript.h"
#include "MCMezzanine.h"
#include "MCTaskContext.h"
#include "MCTaskTarget.h"
#include "MCSimpleAudioEngine.h"

MCSceneContext::MCSceneContext()
{
    objects_ = CCArray::create();
    objects_->retain();
    inited_ = false;
}

MCSceneContext::~MCSceneContext()
{
    CC_SAFE_RELEASE(objects_);
}

/* 敌方人物挂了 */
void
MCSceneContext::enemyWasDied(MCRole *aRole)
{
    MCTask *currentTask = MCTaskManager::sharedTaskManager()->getCurrentTask();
    MCTaskContext *taskContext = currentTask->getTaskContext();
    CCArray *targets = taskContext->getTask()->getTargets();
    CCObject *obj;
    MCTaskTarget *target;
    mc_object_id_t enemyId = aRole->getID();
    bool done = true;
    bool isTarget = false;
    
    objects_->removeObject(aRole);
    CCARRAY_FOREACH(targets, obj) {
        target = dynamic_cast<MCTaskTarget *>(obj);
        if (MCObjectIdIsEqualsTo(target->objectID, enemyId)) {
            isTarget = true;
            if (target->remaining > 0) {
                --target->remaining;
            }
            /* 检查任务是否完成 */
            if (target->remaining != 0) {
                done = false;
            }
        }
    }
    
    if (done && isTarget) {
        /* 任务完成! */
        CCSize winSize = CCDirectorGetWindowsSize();
        CCLabelTTF *missionCompleted = CCLabelTTF::create("任务完成", "", 56 / CC_CONTENT_SCALE_FACTOR());
        scene_->mezzanine()->addChild(missionCompleted);
        missionCompleted->setAnchorPoint(ccp(0.5, 1));
        missionCompleted->setPosition(ccp(winSize.width / 2, 0));
        missionCompleted->runAction(CCSequence::create(CCMoveBy::create(0.4, ccp(0, winSize.height / 2)),
                                                       CCDelayTime::create(0.5),
                                                       CCMoveBy::create(0.6, ccp(0, winSize.height)),
                                                       CCCallFuncO::create(this,
                                                                           callfuncO_selector(MCSceneContext::missionCompleted),
                                                                           currentTask),
                                                       NULL));
    }
}

void
MCSceneContext::missionCompleted(CCObject *anTaskObject)
{
    MCSceneManager::sharedSceneManager()->autoreleaseTaskSceneWithObjectId(scene_->getScenePackage()->getID());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCTaskDidFinishNotification, anTaskObject);
}

#pragma mark *** MCSceneContextManager ***

static const char *kMCDetailButtonPressedFilepath = "UI/d_details_pressed.png";
static const char *kMCDetailButtonFilepath = "UI/d_details.png";

static MCSceneContextManager *__shared_scene_context_manager = NULL;

MCSceneContextManager::MCSceneContextManager()
{
    contextStack_ = CCArray::create();
    contextStack_->retain();
}

MCSceneContextManager::~MCSceneContextManager()
{
    CC_SAFE_RELEASE(contextStack_);
}

MCSceneContextManager *
MCSceneContextManager::sharedSceneContextManager()
{
    if (__shared_scene_context_manager == NULL) {
        __shared_scene_context_manager = new MCSceneContextManager;
    }
    
    return __shared_scene_context_manager;
}

void
MCSceneContextManager::pushContext(MCSceneContext *aContext)
{
    contextStack_->addObject(aContext);
}

void MCSceneContextManager::popContext()
{
    contextStack_->removeLastObject();
}

MCSceneContext *
MCSceneContextManager::currentContext()
{
    return (MCSceneContext *) contextStack_->lastObject();
}

#pragma mark -
#pragma mark *** MCScene ***

MCScene::~MCScene()
{
    CC_SAFE_RELEASE(context_);
    CC_SAFE_RELEASE(trigger_);
    CC_SAFE_DELETE(sceneCamera_);
    CC_SAFE_RELEASE(aStar_);
}

/* 从场景包加载场景初始化 */
bool
MCScene::initWithScenePackage(MCScenePackage *aPackage)
{
    if (aPackage && CCScene::init()) {
        CCSize winSize = CCDirectorGetWindowsSize();
        scenePackage_ = aPackage;
        background_ = MCBackgroundLayer::create(aPackage->getTMXTiledMapPath()->getCString(),
                                                aPackage->getBackgroundMusicPath()->getCString());
        addChild(background_);
        CCSize sceneSize = background_->getSceneSize();
        defaultLocation_ = ccp(sceneSize.width > winSize.width
                               ? 0
                               : (winSize.width - sceneSize.width) / 2,
                               sceneSize.height > winSize.height
                               ? 0
                               : (winSize.height - sceneSize.height) / 2);
        
        CCString *triggerFilepath = aPackage->getTriggerFilepath();
        if (triggerFilepath) {
            trigger_ = MCScriptMaker::createScript(triggerFilepath->getCString());
            trigger_->retain();
        }
        
        sceneCamera_ = new MCCamera;
        sceneCamera_->setSceneDelegate(this);
        
        MCScenePackageType scenePackageType = aPackage->getScenePackageType();
        if (scenePackageType == MCBattleFieldScenePackage
            && !MCFlagManager::sharedFlagManager()->isTaskStarted()) {
            scenePackageType = MCGameScenePackage;
        }
        objects_ = MCObjectLayer::create(scenePackageType);
        objects_->setSceneDelegate(this);
        objects_->setTMXTiledMap(background_->getMap());
        objects_->loadEntrancesFromScenePackage(aPackage);
        addChild(objects_);
        
        aStar_ = new MCAStar;
        aStar_->init(background_->getMap());
        aStar_->setBarriers(objects_->getbarriers());
        
        /* 顺便也会添加夹层世界 */
        installController();
        
        /* entrances */
        entrances_ = aPackage->getScenes();
        
        /* detail menu */
        CCMenu *detailMenu = CCMenu::create();
        addChild(detailMenu);
        CCMenuItemImage *detailMenuItem = CCMenuItemImage::create(kMCDetailButtonFilepath,
                                                                  kMCDetailButtonPressedFilepath);
        detailMenuItem->setTarget(this, menu_selector(MCScene::showPlayerInfo));
        detailMenu->addChild(detailMenuItem);
        detailMenu->alignItemsHorizontally();
        CCSize buttonSize = detailMenuItem->getContentSize();
        detailMenu->setPosition(ccp(winSize.width - buttonSize.width,
                                    winSize.height - buttonSize.height));
        playerInfoMenu_ = detailMenu;
        
        playerInfo_ = MCPlayerInfo::create();
        addChild(playerInfo_);
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                      callfuncO_selector(MCScene::playerInfoDidHide),
                                                                      kMCPlayerInfoDidHideNotification,
                                                                      NULL);
        
#if MC_DEBUG_NON_VISUAL_OBJECTS == 1
        viewport_ = MCViewportLayer::create();
        addChild(viewport_);
        viewport_->map = background_->getMap();
#endif
        
        return true;
    }
    
    return false;
}

/* 从场景包加载场景 */
MCScene *
MCScene::createWithScenePackage(MCScenePackage *aPackage)
{
    MCScene *scene = new MCScene;
    
    if (scene && scene->initWithScenePackage(aPackage)) {
        scene->autorelease();
    } else {
        CC_SAFE_DELETE(scene);
        scene = NULL;
    }
    
    return scene;
}

CCPoint
MCScene::getMapOffset() const
{
    return background_->getMap()->getPosition();
}

CCTMXTiledMap *
MCScene::getMap() const
{
    return background_->getMap();
}

/**
 * 获取场景大小
 */
CCSize &
MCScene::getSceneSize() const
{
    return objects_->mapSize_;
}

void
MCScene::onEnter()
{
    if (context_ == NULL) {
        context_ = new MCSceneContext;
        context_->scene_ = this;
    }
    MCSceneContextManager::sharedSceneContextManager()->pushContext(context_);
    
    CCScene::onEnter();
    
    if (trigger_) {
        trigger_->run();
    }
    
    /* 已加载玩对象了现在 */
    background_->loadEnemies(objects_->objects());
    background_->loadTeam(MCTeam::sharedTeam());
    
#if MC_DEBUG_NON_VISUAL_OBJECTS == 1
    viewport_->objects = objects_->objects();
    viewport_->loadBarriers(objects_->barriers_);
    viewport_->loadSemis(objects_->semiTransparents_);
    viewport_->loadEntrances(objects_->entrances_);
    if (mezzanine_) {
        viewport_->loadMezzanine(mezzanine_);
    }
#endif
    
    /* 设置地图位置 */
    sceneCamera_->restore();
    sceneCamera_->locate();
    sceneCamera_->focusHero();
    
    MCSimpleAudioEngine::sharedSimpleAudioEngine()->playMusic();
}

void
MCScene::onExit()
{
    CCScene::onExit();
    
    MCSceneContextManager::sharedSceneContextManager()->popContext();
    
    MCTaskManager *taskManager = MCTaskManager::sharedTaskManager();
    
    /* 没有接受任务则会销毁场景 */
    if (!taskManager->isTaskAccepted()
        && !taskManager->isTaskActiviting()) {
        MCSceneManager::sharedSceneManager()->autoreleaseSceneWithObjectId(scenePackage_->getID());
    } else {
        MCSceneManager::sharedSceneManager()->autoreleaseTaskSceneWithObjectId(scenePackage_->getID());
    }
}

/**
 * 移动到场景
 * aSceneId(in): 场景ID
 * anEntranceName(in): 场景入口名
 */
void
MCScene::gotoScene(mc_object_id_t aSceneId, const char *anEntranceName, bool isInternal)
{
    MCSceneController *sceneController = MCSceneController::sharedSceneController();
    /* 一切replace */
    sceneController->pushExpectedScene(aSceneId,
                                       anEntranceName,
                                       MCReplaceScene);
    sceneController->requestChangingScene();
}

void
MCScene::moveSceneToLocation(const CCPoint &aLocation, bool adjusted)
{
    CCPoint offset = ccpSub(aLocation, getMapOffset());
    objects_->setSceneOffset(offset);
}

bool
MCScene::hasEntrance(const char *anEntranceName)
{
    return entrances_->objectForKey(anEntranceName) ? true : false;
}

MCScene *
MCScene::getScene()
{
    return this;
}

void
MCScene::showPlayerInfo()
{
    pauseInput();
    playerInfo_->show();
}

void
MCScene::detectsCollidesWithSemiTransparents(MCRole *aRole)
{
    objects_->detectsCollidesWithSemiTransparents(aRole);
}

void
MCScene::showAbortTaskConfirm(const char *aMessage)
{
    if (abortTaskConfirm_) {
        return;
    }
    pauseInput();
    abortTaskConfirm_ = MCConfirm::confirm(this, this, aMessage);
}

void
MCScene::confirmDidClickYesButton(MCConfirm *aConfirm)
{
    MCTaskManager *taskManager = MCTaskManager::sharedTaskManager();
    
    taskManager->abortCurrentTask();
    resumeInput();
    abortTaskConfirm_ = NULL;
    MCSceneController::sharedSceneController()->requestChangingScene();
}

void
MCScene::confirmDidClickNoButton(MCConfirm *aConfirm)
{
    MCHero::sharedHero()->getEntity()->stopWalking();
    resumeInput();
    abortTaskConfirm_ = NULL;
}

void
MCScene::playerInfoDidHide()
{
    resumeInput();
}
