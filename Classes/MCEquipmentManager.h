//
//  MCEquipmentManager.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEquipmentManager__
#define __Military_Confrontation__MCEquipmentManager__

#include "MCEquipmentItem.h"
#include "MCBackpack.h"
#include "MCInterfaceMacros.h"

enum {
    /* 装备 */
    kMCDagger,
    kMCSword,
    kMCGreatsword,
    kMCWarhammer,
    kMCHeavyDutyHammer,
    kMCHandAxe,
    kMCWarAxe,
    kMCGreataxe,
    kMCSpear,
    kMCLance,
    kMCGiantSickle,
    kMCShortbow,
    kMCLongbow,
    kMCHelmet,
    kMCArmor,
    kMCShinGuard
};
typedef mc_enum_t MCEquipmentIndex;

mc_object_id_t MCEquipmentObjectItem(MCEquipmentIndex anIndex);

class MCEquipmentManager {
private:
    MCEquipmentManager();
    
public:
    ~MCEquipmentManager();
    static MCEquipmentManager *sharedEquipmentManager();
    
    /* 防具接口 */
    inline mc_ac_t getAC() {
        return 10 + helmet_->getAC() + armor_->getAC() + shinGuard_->getAC();
    }
    
    inline void changeWeaponTo(MCEquipmentItem *aWeapon) {
        currentWeapon_ = aWeapon;
    }
    
    mc_ssize_t levelUp(MCEquipmentItem *anEquipment);
    
    void saveData();
    
private:
    void loadData();
    
    void saveEquipmentItems();
    void loadEquipmentItems();
    
    
    /* 数据就按这个顺序储存 */
    /* 武器 */
    /* 剑类 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, dagger_, Dagger); /* 短剑 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, sword_, Sword); /* 长剑 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, greatsword_, Greatsword); /* 巨剑 */
    /* 锤类 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, warhammer_, Warhammer); /* 轻型战锤 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, heavyDutyHammer_, HeavyDutyHammer); /* 重型战锤 */
    /* 斧类 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, handAxe_, HandAxe); /* 手斧 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, warAxe_, WarAxe); /* 战斧 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, greataxe_, Greataxe); /* 巨斧 */
    /* 枪矛类 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, spear_, Spear); /* 长枪 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, lance_, Lance); /* 长矛 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, giantSickle_, GiantSickle); /* 巨镰 */
    /* 弓弩类 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, shortbow_, Shortbow); /* 短弓 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, longbow_, Longbow); /* 长弓 */
    
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, currentWeapon_, CurrentWeapon); /* 装备的武器 */
    CC_SYNTHESIZE_READONLY(CCArray *, weapons_, Weapons); /* 所有武器的列表 */
    
    /* 防具 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, helmet_, Helmet); /* 头盔 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, armor_, Armor); /* 铠甲 */
    CC_SYNTHESIZE_READONLY(MCEquipmentItem *, shinGuard_, ShinGuard); /* 护胫 */
    CC_SYNTHESIZE_READONLY(CCArray *, armors_, Armors); /* 所有防具的列表 */
};

#endif /* defined(__Military_Confrontation__MCEquipmentManager__) */
