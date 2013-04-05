//
//  MCTrap.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTrap__
#define __Military_Confrontation__MCTrap__

#include "MCEffectiveItem.h"

enum {
    MCUnknownTrap   = 0,
    MCFireballTrap  = MCMakeEnum(0), /* 火球陷阱 */
    MCCurseTrap     = MCMakeEnum(1), /* 诅咒陷阱 */
    MCParalysisTrap = MCMakeEnum(2), /* 麻痹陷阱 */
    MCFogTrap       = MCMakeEnum(3), /* 迷雾陷阱 */
    MCFlashTrap     = MCMakeEnum(4)  /* 闪光陷阱 */
};
typedef mc_enum_t MCTrapType;

extern const char *kMCUnknownTrap;
extern const char *kMCFireballTrap;
extern const char *kMCCurseTrap;
extern const char *kMCParalysisTrap;
extern const char *kMCFogTrap;
extern const char *kMCFlashTrap;

const char *MCTrapGetNameWithTrapType(MCTrapType aTrapType);

class MCTrap : public MCEffectiveItem {
public:
    bool init(mc_object_id_t anObjectId);
    
    static MCTrap *create(mc_object_id_t anObjectId);
    
    CCObject *copy();
    
    CC_SYNTHESIZE(MCTrapType, trapType_, TrapType);
};

#endif /* defined(__Military_Confrontation__MCTrap__) */
