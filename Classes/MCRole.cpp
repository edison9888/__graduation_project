//
//  MCRole.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCRole.h"
#include "MCAI.h"
#include "MCScript.h"
#include "MCDungeonMaster.h"

#define MCPPPerSecond(sec) ((sec) * 1000.0)

const char *kMCRoleDiedNotification = "kMCRoleDiedNotification";

static const char *__mc_directions[4] = {
    "up",
    "down",
    "left",
    "right"
};

const float kDurationPerFrame = 0.09f;
const char *__default_response = "....";

static const char *
__basename(const char *path)
{
    const char *pointer = path;
    const char *flag = pointer;
    
    while (*pointer != 0) {
        if (*pointer == '/') {
            flag = pointer + 1;
        }
        ++pointer;
    }
    
    return flag;
}

MCRole::MCRole()
{
    entity_ = NULL;
    entityMetadata_ = NULL;
    ai_ = NULL;
    roleType_ = MCRole::MCUnknownRole;
    trigger_ = NULL;
}


MCRole::~MCRole()
{
    CC_SAFE_RELEASE(trigger_);
    CC_SAFE_RELEASE(ai_);
    CC_SAFE_RELEASE(entityMetadata_);
    CC_SAFE_RELEASE(entity_);
}

bool
MCRole::init()
{
    return true;
}

void
MCRole::loadSpriteSheet()
{
    loadSpriteSheet(spriteSheet_->getCString());
}

void
MCRole::loadSpriteSheet(const char *aSpritesheetPath)
{
    char str[64];
    const char *plistFilepath;
    const char *textureFilepath;
    CCSpriteFrameCache *spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteBatchNode* spriteSheet;
    CCArray *animationFrames[4] = {
        CCArray::create(),
        CCArray::create(),
        CCArray::create(),
        CCArray::create()
    };
    CCSpriteFrame *spriteFrame;
    const char *basename = __basename(aSpritesheetPath);
    
    /* 生成路径 */
    /* plist */
    sprintf(str, "%s.plist", aSpritesheetPath);
    plistFilepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(str).c_str();
    /* 把精灵表加载到cache里 */
    spriteFrameCache->addSpriteFramesWithFile(plistFilepath);
    if (entityMetadata_) {
        CC_SAFE_RELEASE(entityMetadata_);
    }
    entityMetadata_ = new MCRoleEntityMetadata;
    
    CCDictionary *plist = CCDictionary::createWithContentsOfFileThreadSafe(str);
    CCDictionary *metadataDict = (CCDictionary*) plist->objectForKey("metadata");
    MCSpriteFrameType spriteFrameType = metadataDict->valueForKey("spriteFrameType")->intValue();
    if (spriteFrameType == MCSpriteFrameSmallSize) {
        entityMetadata_->frameSize_ = kMCSpriteFrameSmallSize;
    } else if (spriteFrameType == MCSpriteFrameMediumSize) {
        entityMetadata_->frameSize_ = kMCSpriteFrameMediumSize;
    } else if (spriteFrameType == MCSpriteFrameLargeSize) {
        entityMetadata_->frameSize_ = kMCSpriteFrameLargeSize;
    }

    /* 纹理文件 */
    sprintf(str, "%s.st", aSpritesheetPath);
    textureFilepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(str).c_str();
    spriteSheet = CCSpriteBatchNode::create(textureFilepath, kDefaultSpriteBatchCapacity);
    spriteSheet->retain();

    /* 添加帧 */
    for (int i = 0; i < sizeof(__mc_directions) / sizeof(const char *); ++i) {
        for (int j = 0; j < 4; ++j) {
            sprintf(str, "%s-%s-%d", basename, __mc_directions[i], j);
            spriteFrame = spriteFrameCache->spriteFrameByName(str);
            animationFrames[i]->addObject(spriteFrame);
        }
    }
    
    /* 创建动画 */
    entityMetadata_->animationGoUp_ = new CCAnimation;
    entityMetadata_->animationGoUp_->initWithSpriteFrames(animationFrames[0], kDurationPerFrame);
    entityMetadata_->animationGoDown_ = new CCAnimation;
    entityMetadata_->animationGoDown_->initWithSpriteFrames(animationFrames[1], kDurationPerFrame);
    entityMetadata_->animationGoLeft_ = new CCAnimation;
    entityMetadata_->animationGoLeft_->initWithSpriteFrames(animationFrames[2], kDurationPerFrame);
    entityMetadata_->animationGoRight_ = new CCAnimation;
    entityMetadata_->animationGoRight_->initWithSpriteFrames(animationFrames[3], kDurationPerFrame);
    
    entityMetadata_->spriteSheet_ = spriteSheet;
    entityMetadata_->spriteSheetPath_ = CCString::create(aSpritesheetPath);
    entityMetadata_->spriteSheetPath_->retain();
    entityMetadata_->facade_ = MCFacingUnknown;
    
    /* 创建AI */
}

/* MCAIDelegate */
/**
 * 某人进入视野
 * 默认看到的都是敌人
 */
void
MCRole::roleDidEnterVision(MCRole *aRole, bool isEnermy)
{
    if (isEnermy) {
        CCLog("敵(%s)、一人見つけた！", aRole->name_->getCString());
        ai_->AIState_ = MCCombatantStatus;
    }
}

/**
 * 某人离开视野
 * 默认离开的都是敌人
 */
void
MCRole::roleDidExitVision(MCRole *aRole, bool isEnermy)
{
    if (isEnermy) {
        CCLog("敵(%s)、一人離れた！", aRole->name_->getCString());
    } else {
        CCLog("%s、一人離れた！", aRole->name_->getCString());
    }
    /* 敌人消失 */
    if (ai_->getEnemiesInVision()->count() < 1) {
        ai_->AIState_ = MCIdleState;
    }
}

/**
 * 被攻击
 */
void
MCRole::roleWasAttacked(const MCEffect &anEffect)
{
    CCLog("攻撃された");
    /* 透支状态时是不会切换的！ */
    if (! exhausted_) {
        ai_->AIState_ = MCCombatantStatus;
    }
}

/**
 * 攻击结束
 */
void
MCRole::attackDidFinish()
{
    ai_->unactivate();
    ai_->AIState_ = MCCombatantStatus;
}

/**
 * 状态切换
 */
void
MCRole::roleDidChangeStateTo(MCAIState anAIState)
{
}

/* MCAIStateMachineDelegate */
/**
 * 空闲状态下回调
 */
void
MCRole::performWhenIdleState()
{
//    CCLog("くだらないなぁ");
    if (pp_ < exhaustion_) {
        exhausted_ = true;
        getEntity()->stopWalking();
        ai_->AIState_ = MCRestingState;
    }
}

/**
 * 战斗状态下回调
 */
void
MCRole::performWhenCombatantStatus()
{
    CCLog("戦え！少年よ！");
    unsigned int count = ai_->getEnemiesInVision()->count();
    if (! exhausted_) { /* 体力透支以上 */
        if (count > 0) {
            CCLog("MCAttackState");
            ai_->AIState_ = MCAttackState;
        } else {
            CCLog("MCIdleState");
            ai_->AIState_ = MCIdleState;
        }
    } else { /* 体力透支，需要休息 */
        CCLog("MCRestingState");
        ai_->AIState_ = MCRestingState;
    }
}

/**
 * 休息状态下回调
 */
void
MCRole::performWhenRestingState()
{
//    CCLog("やすみたいなー");
    double animationInterval = CCDirector::sharedDirector()->getAnimationInterval();
    
    /* 每秒2点体力 */
    pp_ += MCPPPerSecond(2) / animationInterval;
    if (pp_ > maxPP_) {
        pp_ = maxPP_;
        exhausted_ = false;
        ai_->AIState_ = MCIdleState;
    }
}

/**
 * 攻击状态下回调
 */
void
MCRole::performWhenAttackState()
{
    ai_->activate();
        //    CCLog("攻撃せよ！");
    /* 根据仇恨值确定攻击对象 */
    MCRole *target = ai_->roleForMaxAggro();
    if (! target) {
        CCLog("no one");
        return;
    }
    CCLog("will attack %s", target->getName()->getCString());
    /* 交给DM判定攻击 */
    MCDungeonMaster::sharedDungeonMaster()->roleWillAttack(this, target);
    
    attackDidFinish();
}

/**
 * 死亡状态下回调
 */
void
MCRole::performWhenDeathState()
{
    CCLog("死んだ");
    /* 死亡动画神马的还是算了 */
    died();
}

MCRoleEntity *
MCRole::getEntity()
{
    char str[64];
    
    if (entity_ == NULL) {
        sprintf(str, "%s-down-0", __basename(entityMetadata_->spriteSheetPath_->getCString()));
        entity_ = new MCRoleEntity;
        entity_->initWithSpriteFrameName(str);
        entity_->setAnchorPoint(CCPointZero);
        entity_->metadata_ = entityMetadata_;
        entity_->role_ = this;
        entityMetadata_->spriteSheet_->addChild(entity_);
        entityMetadata_->facade_ = MCFacingDown;
    }
    
    return entity_;
}

/**
 * 死亡
 */
void
MCRole::died()
{
    MCRoleEntity *roleEntity = getEntity();
    roleEntity->removeFromParentAndCleanup(true);
    entity_ = NULL;
    CC_SAFE_RELEASE(this);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kMCRoleDiedNotification);
}
