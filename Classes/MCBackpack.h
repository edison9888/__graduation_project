//
//  MCBackpack.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCBackpack__
#define __Military_Confrontation__MCBackpack__

#include "MCItem.h"
#include "MCOreManager.h"
#include "MCEffectiveItem.h"
#include "MCEquipmentItem.h"
#include "MCInterfaceMacros.h"

extern const char *kMCHealthPotionIcon;
extern const char *kMCPhysicalPotionIcon;
extern const char *kMCFireballWideIcon;
extern const char *kMCFireballDamageIcon;
extern const char *kMCCurseWideIcon;
extern const char *kMCCurseDamageIcon;
extern const char *kMCParalysisWideIcon;
extern const char *kMCParalysisDamageIcon;
extern const char *kMCFogWideIcon;
extern const char *kMCFogDamageIcon;
extern const char *kMCFlashWideIcon;
extern const char *kMCFlashDamageIcon;

extern const mc_size_t kMCItemMax;
enum {
    /* 道具 */
    kMCHealthPotion,
    kMCPhysicalPotion,
    kMCFireballWide,
    kMCFireballDamage,
    kMCCurseWide,
    kMCCurseDamage,
    kMCParalysisWide,
    kMCParalysisDamage,
    kMCFogWide,
    kMCFogDamage,
    kMCFlashWide,
    kMCFlashDamage
};
typedef mc_enum_t MCEffectiveItemIndex;

mc_object_id_t MCEffectiveItemObjectItem(MCEffectiveItemIndex anIndex);

class MCBackpackItem {
public:
    MCBackpackItem()
    : item(NULL)
    , count(0) {}
    
    /**
     * 只有物品执行此方法才有效
     */
    inline bool use() {
        if (count > 0) {
            count -= 1;
            return true;
        }
        return false;
    }
    
    MCItem    *item;
    mc_size_t count;
};

class MCBackpack {
private:
    MCBackpack();
    
public:
    ~MCBackpack();
    static MCBackpack *sharedBackpack();
    
    /**
     * 返回是否花费成功
     */
    bool spend(mc_price_t money);
    
    /* 道具 */
    /* 药品 */
    MCDefineInterface(healthPotion_, HealthPotion);
    MCDefineInterface(physicalPotion_, PhysicalPotion);
    /* 陷阱 */
    MCDefineInterface(fireballTrapWide_, FireballTrapWide);
    MCDefineInterface(fireballTrapDamage_, FireballTrapDamage);
    MCDefineInterface(curseTrapWide_, CurseTrapWide);
    MCDefineInterface(curseTrapDamage_, CurseTrapDamage);
    MCDefineInterface(paralysisTrapWide_, ParalysisTrapWide);
    MCDefineInterface(paralysisTrapDamage_, ParalysisTrapDamage);
    MCDefineInterface(fogTrapWide_, FogTrapWide);
    MCDefineInterface(fogTrapDamage_, FogTrapDamage);
    MCDefineInterface(flashTrapWide_, FlashTrapWide);
    MCDefineInterface(flashTrapDamage_, FlashTrapDamage);
    
    void saveData();
    
private:
    void loadData();
    
    void saveEffectiveItems();
    void loadEffectiveItems();
    
    void loadIcons();

private:
    CC_SYNTHESIZE(mc_price_t, money_, Money); /* 身上的金钱 */
    
    /* 道具 */
    /* 数据就按这个顺序储存 */
    /* 药品 */
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, healthPotion_, HealthPotion);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, physicalPotion_, PhysicalPotion);
    /* 陷阱 */
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, fireballTrapWide_, FireballTrapWide);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, fireballTrapDamage_, FireballTrapDamage);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, curseTrapWide_, CurseTrapWide);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, curseTrapDamage_, CurseTrapDamage);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, paralysisTrapWide_, ParalysisTrapWide);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, paralysisTrapDamage_, ParalysisTrapDamage);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, fogTrapWide_, FogTrapWide);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, fogTrapDamage_, FogTrapDamage);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, flashTrapWide_, FlashTrapWide);
    CC_SYNTHESIZE_READONLY(MCBackpackItem *, flashTrapDamage_, FlashTrapDamage);
};

#endif /* defined(__Military_Confrontation__MCBackpack__) */
