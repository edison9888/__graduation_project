//
//  MCBattleController.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//
#include "AppMacros.h"
#include "MCTask.h"
#include "MCTaskContext.h"
#include "MCTaskManager.h"
#include "MCBattleController.h"
#include "MCScene.h"
#include "MCMezzanine.h"

#if MC_SELECT_ALL_SUPPORT == 1
static const char *kMCMultiSelectionFilepath = "UI/bc_multi_selection.png";
#endif

static const char *kMCPointToParticleFilepath = "particles/point_to.plist";
static const char *kMCSkillBarItemSelectedEffectFilepath = "particles/skill-selected.plist";

static bool
MCActionBarItemIsPotion(MCActionBarItem *anActionBarItem)
{
    MCTask *task = MCTaskManager::sharedTaskManager()->getCurrentTask();
    CCAssert(task != NULL, "代码逻辑错误");
    MCTaskContext *taskContext = task->getTaskContext();
    MCBackpackItem *backpackItem = anActionBarItem->getBackpackItem();
    
    return backpackItem == taskContext->getHealthPotion()
            || backpackItem == taskContext->getPhysicalPotion();
}

MCBattleController::~MCBattleController()
{
    CC_SAFE_RELEASE(teamLayer_);
    CC_SAFE_RELEASE(actionBar_);
    CC_SAFE_RELEASE(skillBar_);
    CC_SAFE_RELEASE(skillBarItemSelectedEffect_);
}

bool
MCBattleController::init()
{
    if (CCLayer::init()) {
        teamLayer_ = MCTeamLayer::create();
        addChild(teamLayer_);
        
        actionBar_ = MCActionBar::create();
        addChild(actionBar_);
        
        skillBar_ = MCSkillBar::create();
        addChild(skillBar_);
        
#if MC_SELECT_ALL_SUPPORT == 1
        CCMenu *menu;
        CCMenuItemLabel *menuItem;
        CCLabelTTF *label;
        
        menu = CCMenu::create();
        label = CCLabelTTF::create("全选", "Marker Felt", 24);
        menuItem = CCMenuItemLabel::create(label, this, menu_selector(MCBattleController::didSelectAll));
        menu->addChild(menuItem);
        CCSize menuSize = menuItem->getContentSize();
        menu->setPosition(ccp(menuSize.width, menuSize.height));
        
        addChild(menu);
        selectAllMenu_ = menu;
#endif
        
        isJoypadEnabled_ = false;
        isDragging_ = false;
        
        lastTouchedTime_.tv_sec = 0;
        lastTouchedTime_.tv_usec = 0;
        selectedActionBarItem_ = NULL;
        selectedSkillBarItem_ = NULL;
        lastSelectedSkillBarItem_ = NULL;
        
        skillBarItemSelectedEffect_ = new CCParticleSystemQuad;
        skillBarItemSelectedEffect_->initWithFile(kMCSkillBarItemSelectedEffectFilepath);
        
        return true;
    }
    
    return false;
}

#if MC_SELECT_ALL_SUPPORT == 1
CCArray *
MCBattleController::getSelectedRoles()
{
    return teamLayer_->getSelectedRoles();
}
#else
MCRole *
MCBattleController::getSelectedRole()
{
    return teamLayer_->getSelectedRole();
}
#endif

/* 选中敌人 */
void
MCBattleController::selectTarget(MCRole *aRole)
{
    /* 选中敌人！干之！ */
    MCRole *selectedRole = getSelectedRole();
    
    /* 检测是否选中技能 */
    MCSkill *skill = lastSelectedSkillBarItem_ ? lastSelectedSkillBarItem_->getSkill() : NULL;
    
    /* 选中了技能，但是不能使用的话，不做处理 */
    if (skill) {
        /* 技能攻击 */
        if (skill->canRoleUse(selectedRole)) {
            selectedRole->attackTargetWithSkill(aRole, skill);
        }
    } else {
        /* 普通攻击 */
        selectedRole->attackTarget(aRole);
    }
}

bool
MCBattleController::isItemTouchedForTouch(CCTouch *aTouch)
{
    return teamLayer_->roleBaseInfoForTouch(aTouch)
            || actionBar_->itemForTouch(aTouch)
            || skillBar_->itemForTouch(aTouch);
}

void
MCBattleController::onEnter()
{
    CCLayer::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                  callfuncO_selector(MCBattleController::skillBarVisibleDidChange),
                                                                  kMCSkillBarVisibleDidChangeNotification,
                                                                  NULL);
}

void
MCBattleController::onExit()
{
    CCLayer::onExit();
}

void
MCBattleController::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    
    MCRoleBaseInfo *info = teamLayer_->roleBaseInfoForTouch(touch);
    if (info) {
        info->setTouched(true);
    }
    
    /* action bar item操作 */
    selectedActionBarItem_ = actionBar_->itemForTouch(touch);
    if (selectedActionBarItem_ && selectedActionBarItem_->getBackpackItem()->count > 0) {
        selectedActionBarItem_->touchedPoint = touch->getLocation();
        selectedActionBarItem_->setOpacity(kMCDraggingActionBarItemOpacity);
    }
    /* skill bar item操作 */
    if (selectedActionBarItem_ == NULL) { /* action bar item都被选中了，再检测skill bar item也没意义 */
        selectedSkillBarItem_ = skillBar_->itemForTouch(touch);
    } else {
        selectedSkillBarItem_ = NULL;
    }
    
    CCLayer::ccTouchesBegan(pTouches, pEvent);
}

void
MCBattleController::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
#if MC_SELECT_ALL_SUPPORT == 1
    CCArray *selectedRoles = teamLayer_->getSelectedRoles();
#else
    MCRole *selectedRole = teamLayer_->getSelectedRole();
#endif
    
    if (selectedActionBarItem_) {
        for (CCSetIterator iterator = pTouches->begin();
             iterator != pTouches->end();
             ++iterator) {
            CCTouch *touch = dynamic_cast<CCTouch *>(*iterator);
            if (selectedActionBarItem_->touchedPoint.equals(touch->getPreviousLocation())) {
                CCPoint touchedLocation = touch->getLocation();
                CCPoint offset = ccpSub(touch->getLocation(), touch->getPreviousLocation());
                selectedActionBarItem_->setPosition(ccpAdd(selectedActionBarItem_->getPosition(), offset));
                selectedActionBarItem_->touchedPoint = touch->getLocation();
                isDragging_ = true;
                if (selectedActionBarItem_->isTrap()
#if MC_SELECT_ALL_SUPPORT == 1
                    && selectedRoles->count() > 0) {
#else
                    && selectedRole) {
#endif
                    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
                    MCScene *scene = sceneContext->getScene();
                    MCMezzanine *mezzanine = scene->mezzanine();
                    MCTrap *trap = dynamic_cast<MCTrap *>((selectedActionBarItem_->getBackpackItem()->item));
                    trap->setPosition(touchedLocation);
                    selectedActionBarItem_->setVisible(false);
                    mezzanine->setDraggingTrap(trap);
                }
                break;
            }
        }
        teamLayer_->acceptActionBarItem(selectedActionBarItem_);
    } else if (selectedSkillBarItem_) {
        bool out = true;
        for (CCSetIterator iterator = pTouches->begin();
             iterator != pTouches->end();
             ++iterator) {
            CCTouch *touch = dynamic_cast<CCTouch *>(*iterator);
            out = selectedSkillBarItem_ != skillBar_->itemForTouch(touch);
        }
        if (out) {
            selectedSkillBarItem_ = NULL;
        }
#if MC_SELECT_ALL_SUPPORT == 1
    } else if (selectedRoles->count() == 0) { /* 拖动地图模式 */
#else
    } else if (selectedRole == NULL) { /* 拖动地图模式 */
#endif
        CCTouch *touch = (CCTouch *) pTouches->anyObject();
        CCPoint offset = ccpSub(touch->getLocation(), touch->getPreviousLocation());
        delegate_->controllerDidDragMap(this, offset);
    }
    
    CCLayer::ccTouchesMoved(pTouches, pEvent);
}

void
MCBattleController::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    bool findPath = true;
    
    struct cc_timeval touchedTime;
    CCTime::gettimeofdayCocos2d(&touchedTime, NULL);
    
    /* 队伍选择 */
    /* tags: #select,#team */
    MCRoleBaseInfo *roleBaseInfo = teamLayer_->roleBaseInfoForTouch(touch);
    if (roleBaseInfo && roleBaseInfo->getTouched()) {
        MCRole *selectedRole = teamLayer_->getSelectedRole();
        bool isSelected = roleBaseInfo->isSelected();
        MCRole *role = roleBaseInfo->getRole();
        double elapsed = 0;
        
        findPath = false;
        if (lastTouchedTime_.tv_sec != 0) {
            elapsed = CCTime::timersubCocos2d(&lastTouchedTime_, &touchedTime);
        }
        
        if (selectedRole) {
            teamLayer_->unselectRole(selectedRole);
            skillBar_->showSkillsForRole(NULL);
            skillBarItemSelectedEffect_->setVisible(false);
            delegate_->controllerDidUnselectRole(this, role);
        }
        /* 两次点击小于0.5秒则为聚焦人物，并且选中之 */
        if (elapsed < 500) {
            lastTouchedTime_.tv_sec = 0;
            teamLayer_->selectRole(role);
            skillBar_->showSkillsForRole(role);
            skillBarItemSelectedEffect_->setVisible(true);
            delegate_->controllerDidSelectRole(this, role);
            delegate_->controllerDidFocus(this, role);
        } else if (isSelected) {
            teamLayer_->unselectRole(role);
            skillBar_->showSkillsForRole(NULL);
            skillBarItemSelectedEffect_->setVisible(false);
            delegate_->controllerDidUnselectRole(this, role);
        } else {
            teamLayer_->selectRole(role);
            skillBar_->showSkillsForRole(role);
            skillBarItemSelectedEffect_->setVisible(true);
            delegate_->controllerDidSelectRole(this, role);
        }
#if MC_SELECT_ALL_SUPPORT == 1
        if (teamLayer_->getSelectedRoles()->count() == teamLayer_->size()) { /* 全部选中了 */
            CCMenuItemLabel *menuItem = (CCMenuItemLabel *)selectAllMenu_->getChildren()->objectAtIndex(0);
            CCLabelTTF *label = (CCLabelTTF *) menuItem->getLabel();
            label->setString("取消");
        }
#endif
        roleBaseInfo->setTouched(false);
    }
    lastTouchedTime_ = touchedTime;
    
    /* tags: #item,#use */
#if MC_SELECT_ALL_SUPPORT == 1
    CCArray *selectedRoles = teamLayer_->getSelectedRoles();
    MCRole *selectedRole = selectedRoles->count() > 0 ? dynamic_cast<MCRole *>(selectedRoles->objectAtIndex(0)) : NULL;
#else
    MCRole *selectedRole = teamLayer_->getSelectedRole();
#endif
    for (CCSetIterator iterator = pTouches->begin();
         iterator != pTouches->end();
         ++iterator) {
        CCTouch *touch = dynamic_cast<CCTouch *>(*iterator);
        MCRoleBaseInfo *selectedRoleBaseInfo;
        MCActionBarItem *touchedActionBarItem = actionBar_->itemForTouch(touch);
        
        if (selectedActionBarItem_ != NULL) {
            bool isTrap = selectedActionBarItem_->isTrap();
            
            if (selectedActionBarItem_ == touchedActionBarItem) {
                findPath = false;
                if (selectedActionBarItem_->getBackpackItem()->count > 0) {
                    if (selectedActionBarItem_->getItemPosition().equals(selectedActionBarItem_->getPosition())) {
                        if (isTrap && selectedRole) {
                            MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
                            MCScene *scene = sceneContext->getScene();
                            MCMezzanine *mezzanine = scene->mezzanine();
                            MCTrap *trap = dynamic_cast<MCTrap *>((selectedActionBarItem_->getBackpackItem()->item));
                            /* 在选择的人物的自身位置安装陷阱 */
                            trap = dynamic_cast<MCTrap *>(trap->copy());
                            trap->autorelease();
                            mezzanine->installTrap(trap, selectedRole->getEntity()->getPosition());
                            --selectedActionBarItem_->getBackpackItem()->count;
                            selectedActionBarItem_->updateCount();
                        } else {
                            /* 直接点击使用道具 */
                            if (MCActionBarItemIsPotion(selectedActionBarItem_)) { /* 是药品才能使用 */
                                teamLayer_->selectedRolesUseActionBarItem(selectedActionBarItem_);
                                selectedActionBarItem_->updateCount();
                            }
                        }
                    } else if (isTrap && selectedRole) {
                        /* 到目标位置安装陷阱 */
                        MCTrap *trap = dynamic_cast<MCTrap *>((selectedActionBarItem_->getBackpackItem()->item));
                        CCPoint destinationPosition = touch->getLocation();
                        /* 在选择的人物的自身位置安装陷阱 */
                        MCRoleEntity *roleEntity = selectedRole->getEntity();
                        trap = dynamic_cast<MCTrap *>(trap->copy());
                        trap->setPosition(ccpSub(destinationPosition,
                                                 MCSceneContextManager::sharedSceneContextManager()
                                                 ->currentContext()
                                                 ->getScene()
                                                 ->getMapOffset()));
                        trap->setUserObject(selectedActionBarItem_);
                        roleEntity->findPath(destinationPosition,
                                             this,
                                             callfuncO_selector(MCBattleController::installTrap),
                                             trap);
                        
                    } else if ((selectedRoleBaseInfo = teamLayer_->collidesWithActionBarItem(selectedActionBarItem_)) != NULL) {
                        /* 拖动使用道具 */
                        selectedRoleBaseInfo->setOpacity(255);
                        selectedRoleBaseInfo->useActionBarItem(selectedActionBarItem_);
                        /* 物品效果 */
                        dynamic_cast<MCEffectiveItem *>(selectedActionBarItem_->getBackpackItem()->item)->effect->attach(this,
                                                                                                                         selectedRoleBaseInfo->getRole());
                        selectedActionBarItem_->updateCount();
                    }
                }
            } 
        }
        
        MCSkillBarItem *touchedSkillBarItem = skillBar_->itemForTouch(touch);
        if (selectedSkillBarItem_ != NULL
            && selectedSkillBarItem_ == touchedSkillBarItem) {
            
            findPath = false;
            if (touchedSkillBarItem->isSelected()) {
                skillBarItemSelectedEffect_->removeFromParentAndCleanup(false);
                touchedSkillBarItem->unselected();
                touchedSkillBarItem = NULL;
            } else {
                if (! skillBarItemSelectedEffect_->getParent()) {
                    addChild(skillBarItemSelectedEffect_);
                }
                skillBarItemSelectedEffect_->setPosition(touchedSkillBarItem->getPosition());
                touchedSkillBarItem->selected();
            }
            if (lastSelectedSkillBarItem_) {
                lastSelectedSkillBarItem_->unselected();
            }
            lastSelectedSkillBarItem_ = touchedSkillBarItem;
            selectedSkillBarItem_ = NULL;
        }
    }
    
    if (selectedActionBarItem_) {
        if (selectedActionBarItem_->isTrap()) {
            MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
            MCScene *scene = sceneContext->getScene();
            MCMezzanine *mezzanine = scene->mezzanine();
            selectedActionBarItem_->setVisible(true);
            mezzanine->setDraggingTrap(NULL);
        }
        selectedActionBarItem_->resetPosition();
        if (selectedActionBarItem_->getBackpackItem()->count > 0) {
            selectedActionBarItem_->setOpacity(kMCNormalActionBarItemOpacity);
        } else {
            selectedActionBarItem_->setOpacity(kMCInvalidActionBarItemOpacity);
        }
        selectedActionBarItem_ = NULL;
    }
    
    /* 行动 */
    /* 行走 */
    if (!isJoypadEnabled_
        && !isDragging_) {
#if MC_SELECT_ALL_SUPPORT == 1
        CCArray *selectedRoles = teamLayer_->getSelectedRoles();
        CCObject *obj;
        
        /* 随机生成几个位置 */
        
        if (findPath && selectedRoles->count() > 0) {
            CCARRAY_FOREACH(selectedRoles, obj) {
                MCRole *role = dynamic_cast<MCRole *>(obj);
#else
                MCRole *role = teamLayer_->getSelectedRole();
                if (findPath && role) {
#endif
                    CCPoint location = touch->getLocation();
                    /* 来个粒子效果 */
                    CCParticleSystemQuad *pointTo = CCParticleSystemQuad::create(kMCPointToParticleFilepath);
                    pointTo->setAutoRemoveOnFinish(true);
                    pointTo->setPosition(location);
                    addChild(pointTo);
                    role->getEntity()->findPath(location);
                }
#if MC_SELECT_ALL_SUPPORT == 1
            }
#endif
        }
    
    isDragging_ = false;
    
    CCLayer::ccTouchesEnded(pTouches, pEvent);
}

void
MCBattleController::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    if (selectedActionBarItem_) {
        if (selectedActionBarItem_->isTrap()) {
            MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
            MCScene *scene = sceneContext->getScene();
            MCMezzanine *mezzanine = scene->mezzanine();
            selectedActionBarItem_->setVisible(true);
            mezzanine->setDraggingTrap(NULL);
        }
        selectedActionBarItem_->resetPosition();
        selectedActionBarItem_->setOpacity(255);
        selectedActionBarItem_ = NULL;
    }
    
    isDragging_ = false;
    CCLayer::ccTouchesCancelled(pTouches, pEvent);
}

#if MC_SELECT_ALL_SUPPORT == 1
void
MCBattleController::didSelectAll(CCObject *aSender)
{
    CCMenuItemLabel *menuItem = (CCMenuItemLabel *)selectAllMenu_->getChildren()->objectAtIndex(0);
    CCLabelTTF *label = (CCLabelTTF *) menuItem->getLabel();
    CCArray *roles = teamLayer_->getSelectedRoles();
    CCObject *obj;
    if (roles->count() == teamLayer_->size()) {
        /* tags: #select,#all */
        /* 全选的话，镜头将移到主角身上 */
        /* 已经选择全部了，则全部取消选择 */
        CCARRAY_FOREACH(roles, obj) {
            delegate_->controllerDidUnselectRole(this, dynamic_cast<MCRole *>(obj));
        }
        teamLayer_->unselectAll();
        label->setString("全选");
        delegate_->controllerDidUnselectAll(this, MCTeam::sharedTeam());
    } else {
        CCARRAY_FOREACH(roles, obj) {
            delegate_->controllerDidSelectRole(this, dynamic_cast<MCRole *>(obj));
        }
        teamLayer_->selectAll();
        label->setString("取消");
        delegate_->controllerDidSelectAll(this, MCTeam::sharedTeam());
    }
}
#endif
    
void
MCBattleController::skillBarVisibleDidChange(CCObject *anObject)
{
    /* 由于是改变前的状态，所以隐藏了即为未隐藏 */
    skillBarItemSelectedEffect_->setVisible(skillBar_->isHidden());
}
    
void
MCBattleController::installTrap(CCObject *anObject)
{
    MCSceneContext *sceneContext = MCSceneContextManager::sharedSceneContextManager()->currentContext();
    MCScene *scene = sceneContext->getScene();
    MCMezzanine *mezzanine = scene->mezzanine();
    MCTrap *trap = dynamic_cast<MCTrap *>(anObject);
    MCActionBarItem *selectedActionBarItem = dynamic_cast<MCActionBarItem *>(trap->getUserObject());
    
    --selectedActionBarItem->getBackpackItem()->count;
    selectedActionBarItem->updateCount();
    
    mezzanine->installTrap(trap);
    trap->release(); /* non-autorelease */
}
