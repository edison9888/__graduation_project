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

const char *kMCEffectFilepath = "E700.jpkg";

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
        
        effect->init();
        effect->id_ = e_id;
        
        effect->implType_ = effectValueRoot["type"].getInt();
        
        if (effect->implType_ == MCEffect::MCSpriteSheet) {
            std::string path = effectValueRoot["path"].getString();
            const char *cpath = path.c_str();
            const char *basename = __basename(cpath);
            effect->effect_ = CCString::createWithFormat("%s-0", basename);
            effect->animation_ = loadAnimation(cpath);
            effect->animation_->retain();
        } else if (effect->implType_ == MCEffect::MCPList) {
            effect->effect_ = CCString::create(effectValueRoot["path"].getString().c_str());
        }
#warning 尚存effect->implType_等于0的未处理数据
        if (effect->effect_ == NULL) {
            effect->effect_ = CCString::create("(NULL)");
        }
        effect->effect_->retain();
        
        effects_->setObject(effect, MCObjectIdToDickKey(e_id));
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
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames);
    animation->setDelayPerUnit(1.0 / animFrames->count());
    
    return animation;
}
