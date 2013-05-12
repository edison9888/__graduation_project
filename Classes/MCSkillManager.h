//
//  MCSkillManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCSkillManager__
#define __Military_Confrontation__MCSkillManager__

#include "MCSkill.h"

enum {
    MCSwordSkill = 1,
    MCHammerSkill,
    MCAxeSkill,
    MCSpearSkill,
    MCBowSkill
};
typedef mc_enum_t MCSkillType;

class MCSkillManager {
private:
    MCSkillManager();
    
public:
    ~MCSkillManager();
    static MCSkillManager *sharedSkillManager();
    
    MCSkill *skillForObjectId(mc_object_id_t anObjectId);
    MCSkill *protoSkillForObjectId(mc_object_id_t anObjectId);
    
    CCArray *skillsForSkillType(MCSkillType aSkillType);
    
    void loadData();
    void saveData();
    
    void improveProficiency(MCSkillType aSkillType);
    
private:
    void loadSkills();
    
private:
    CCDictionary *skills_;
    
    /* 技能列表 */
    CCArray *swordSkills_;
    CCArray *hammerSkills_;
    CCArray *axeSkills_;
    CCArray *spearSkills_;
    CCArray *bowSkills_;
    CCArray *otherSkills_; /* 佣兵和敌人的技能 */
    
    /* 熟练度 */
    CC_SYNTHESIZE_READONLY(mc_proficiency_t, swordProficiency_, SwordProficiency);
    CC_SYNTHESIZE_READONLY(mc_proficiency_t, hammerProficiency_, HammerProficiency);
    CC_SYNTHESIZE_READONLY(mc_proficiency_t, axeProficiency_, AxeProficiency);
    CC_SYNTHESIZE_READONLY(mc_proficiency_t, spearProficiency_, SpearProficiency);
    CC_SYNTHESIZE_READONLY(mc_proficiency_t, bowProficiency_, BowProficiency);
};

#endif /* defined(__Military_Confrontation__MCSkillManager__) */
