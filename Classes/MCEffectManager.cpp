//
//  MCEffectManager.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEffectManager.h"
#include "JsonBox.h"

static MCEffectManager *__shared_effect_manager = NULL;

static mc_object_id_t kMCMissEffectId = {'M', 'I', 'S', 'S'};
static mc_object_id_t kMCLaunchingEffectId = {'L', 'N', 'I', 'N'};

static const char *kMCEffectFilepath = "E700.jpkg";

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


MCEffectManager::MCEffectManager()
{
    effects_ = CCDictionary::create();
    effects_->retain();
}

MCEffectManager::~MCEffectManager()
{
    CC_SAFE_RELEASE(effects_);
}

MCEffectManager *
MCEffectManager::sharedEffectManager()
{
    if (__shared_effect_manager == NULL) {
        __shared_effect_manager = new MCEffectManager;
        __shared_effect_manager->loadEffects();
    }
    
    return __shared_effect_manager;
}

MCEffect *
MCEffectManager::effectForObjectId(mc_object_id_t anObjectId)
{
    MCEffect *effect = dynamic_cast<MCEffect *>(protoEffectForObjectId(anObjectId)->copy());
    
    if (effect) {
        effect->autorelease();
    }
    
    return effect;
}

MCEffect *
MCEffectManager::protoEffectForObjectId(mc_object_id_t anObjectId)
{
    return dynamic_cast<MCEffect *>(effects_->objectForKey(MCObjectIdToDickKey(anObjectId)));
}

MCEffect *
MCEffectManager::missEffect()
{
    return effectForObjectId(kMCMissEffectId);
}

MCEffect *
MCEffectManager::launchingEffect()
{
    MCEffect *launchingEffect = effectForObjectId(kMCLaunchingEffectId);
    
    launchingEffect->isReleaseEffect_ = true;
    
    return launchingEffect;
}

void
MCEffectManager::loadEffects()
{
    JsonBox::Value v;
    JsonBox::Object root;
    JsonBox::Object::iterator effectsIterator;
    JsonBox::Object effectValueRoot;
    MCEffect *effect;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(kMCEffectFilepath);
    if (pstrFileContent) {
        v.loadFromString(pstrFileContent->getCString());
    }
#else
    v.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(kMCEffectFilepath).c_str());
#endif
    
    root = v.getObject();
    
    for (effectsIterator = root.begin();
         effectsIterator != root.end();
         ++effectsIterator) {
        const char *c_str_e_id = effectsIterator->first.c_str();
        mc_object_id_t e_id = {
            c_str_e_id[0],
            c_str_e_id[1],
            c_str_e_id[2],
            c_str_e_id[3]
        };
        effectValueRoot = effectsIterator->second.getObject();
        
        effect = new MCEffect;
        
        effect->id_ = e_id;
        
        effect->implType_ = effectValueRoot["type"].getInt();
        effect->duration_ = effectValueRoot["duration"].isDouble()
                                ? (float) effectValueRoot["duration"].getDouble()
                                : (float) effectValueRoot["duration"].getInt();
        
        if (effect->implType_ == MCEffect::MCSpriteSheet) {
            std::string path = effectValueRoot["path"].getString();
            const char *cpath = path.c_str();
            const char *basename = __basename(cpath);
            effect->effect_ = new CCString;
            effect->effect_->initWithFormat("%s-0", basename);
            effect->animation_ = loadAnimation(cpath); /* non autorelease */
            effect->animation_->setDelayPerUnit(effect->duration_ / effect->animation_->getFrames()->count());
        } else if (effect->implType_ == MCEffect::MCPList) {
            effect->effect_ = new CCString(effectValueRoot["path"].getString());
        }
        
        effects_->setObject(effect, MCObjectIdToDickKey(e_id));
        effect->release();
    }
}

CCAnimation *
MCEffectManager::loadAnimation(const char *aPath)
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame *frame;
    char str[100] = {0};
    CCArray *animFrames = CCArray::create();
    const char *basename = __basename(aPath);
    
    sprintf(str, "%s.plist", aPath);
    cache->addSpriteFramesWithFile(str);
    
    for(int i = 0; ; i++) {
        sprintf(str, "%s-%d", basename, i);
        frame = cache->spriteFrameByName( str );
        CC_BREAK_IF(! frame);
        animFrames->addObject(frame);
    }
    
    CCAnimation *animation = new CCAnimation;
    animation->initWithSpriteFrames(animFrames);
    animation->setDelayPerUnit(1.0 / animFrames->count());
    
    return animation;
}
