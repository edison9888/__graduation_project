//
//  MCRole.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-22.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCRole.h"

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
}


MCRole::~MCRole()
{
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
    CCDictionary *plist;
    
    /* 生成路径 */
    /* plist */
    sprintf(str, "%s.plist", aSpritesheetPath);
    plistFilepath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(str);
    /* 把精灵表加载到cache里 */
    spriteFrameCache->addSpriteFramesWithFile(plistFilepath);
    plist = CCDictionary::createWithContentsOfFileThreadSafe(plistFilepath);
    if (entityMetadata_) {
        CC_SAFE_RELEASE(entityMetadata_);
    }
    entityMetadata_ = new MCRoleEntityMetadata;

    /* 纹理文件 */
    sprintf(str, "%s.st", aSpritesheetPath);
    textureFilepath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(str);
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
 * 某人进入视野
 * 默认看到的都是敌人
 */
void
MCRole::onSeeSomeone(MCRole *aRole, bool isEnermy)
{
}

/**
 * 某人离开视野
 * 默认离开的都是敌人
 */
void
MCRole::onSomeoneDidLeave(MCRole *aRole, bool isEnermy)
{
}

/**
 * 被攻击
 */
void
MCRole::wasAttacked(const MCEffect &anEffect)
{
    hp_ -= anEffect.hp;
    
    if (hp_ < 0) {
        this->died();
    } else {
        pp_ -= anEffect.pp;
        roleState_ |= anEffect.positive_state;
        roleState_ ^= anEffect.negative_state;
    }
}

/**
 * 死亡
 */
void
MCRole::died()
{
    entity_->removeFromParentAndCleanup(true);
    entity_ = NULL;
    CC_SAFE_RELEASE(this);
}
