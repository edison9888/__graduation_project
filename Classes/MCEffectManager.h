//
//  MCEffectManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEffectManager__
#define __Military_Confrontation__MCEffectManager__

#include "MCEffect.h"

class MCEffectManager {
private:
    MCEffectManager();
    
public:
    ~MCEffectManager();
    
    static MCEffectManager *sharedEffectManager();
    
    MCEffect *effectForObjectId(mc_object_id_t anObjectId);
    MCEffect *protoEffectForObjectId(mc_object_id_t anObjectId);
    
private:
    void loadEffects();
    
    CCAnimation *loadAnimation(const char *aPath);
    
    void loadPList(const char *aPListFilepath);
    
    CCDictionary *effects_;
};

#endif /* defined(__Military_Confrontation__MCEffectManager__) */
