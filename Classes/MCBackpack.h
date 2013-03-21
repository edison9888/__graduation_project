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

extern const mc_size_t kMCItemMax;

class MCBackpackItem {
public:
    MCItem    *item;
    mc_size_t count;
};

class MCBackpack {
private:
    
public:
    static MCBackpack *sharedBackpack();
    
    void saveData();
    
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
    
    inline mc_ssize_t levelUp(const MCBackpackItem &anItem) {
        MCEquipmentItem *item = (MCEquipmentItem *) anItem.item;
        MCOre *currentOre = item->getOre();
        MCOre *nextLevelOre = currentOre->getNextLevel();
        
        if (nextLevelOre) {
            if (nextLevelOre->getPrice() > money_) {
                return kMCNotEnoughMoney;
            }
            money_ -= nextLevelOre->getPrice();
            
            return kMCHandleSucceed;
        }
        
        return kMCFullLevel;
    }
    
    /* 装备 */
    /* 武器 */
    MCDefineLevelUpInterface(dagger_, Dagger); /* 短剑 */
    MCDefineLevelUpInterface(sword_, Sword); /* 长剑 */
    MCDefineLevelUpInterface(greatsword_, Greatsword); /* 巨剑 */
    /* 刀类 */
    MCDefineLevelUpInterface(scimitar_, Scimitar); /* 弯刀 */
    MCDefineLevelUpInterface(broadsword_, Broadsword); /* 大刀 */
    MCDefineLevelUpInterface(machete_, Machete); /* 大砍刀 */
    /* 锤类 */
    MCDefineLevelUpInterface(warhammer_, Warhammer); /* 轻型战锤 */
    MCDefineLevelUpInterface(heavyDutyHammer_, HeavyDutyHammer); /* 重型战锤 */
    /* 斧类 */
    MCDefineLevelUpInterface(handAxe_, HandAxe); /* 手斧 */
    MCDefineLevelUpInterface(warAxe_, WarAxe); /* 战斧 */
    MCDefineLevelUpInterface(greataxe_, Greataxe); /* 巨斧 */
    /* 枪矛类 */
    MCDefineLevelUpInterface(spear_, Spear); /* 长枪 */
    MCDefineLevelUpInterface(lance_, Lance); /* 长矛 */
    MCDefineLevelUpInterface(giantSickle_, GiantSickle); /* 巨镰 */
    /* 弓弩类 */
    MCDefineLevelUpInterface(shortbow_, Shortbow); /* 短弓 */
    MCDefineLevelUpInterface(longbow_, Longbow); /* 长弓 */
    
    /* 防具 */
    MCDefineLevelUpInterface(helmet_, Helmet); /* 头盔 */
    MCDefineLevelUpInterface(armor_, Armor); /* 铠甲 */
    MCDefineLevelUpInterface(shinGuard_, ShinGuard); /* 护胫 */
    
protected:
    void loadData();
    
    void saveEffectiveItems();
    void saveEquipmentItems();
    void loadEffectiveItems();
    void loadEquipmentItems();

private:
    CC_SYNTHESIZE(mc_price_t, money_, Money); /* 身上的金钱 */
    
    /* 道具 */
    /* 数据就按这个顺序储存 */
    /* 药品 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, healthPotion_, HealthPotion);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, physicalPotion_, PhysicalPotion);
    /* 陷阱 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, fireballTrapWide_, FireballTrapWide);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, fireballTrapDamage_, FireballTrapDamage);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, curseTrapWide_, CurseTrapWide);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, curseTrapDamage_, CurseTrapDamage);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, paralysisTrapWide_, ParalysisTrapWide);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, paralysisTrapDamage_, ParalysisTrapDamage);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, fogTrapWide_, FogTrapWide);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, fogTrapDamage_, FogTrapDamage);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, flashTrapWide_, FlashTrapWide);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, flashTrapDamage_, FlashTrapDamage);
    
    /* 装备 */
    /* 数据就按这个顺序储存 */
    /* 武器 */
    /* 剑类 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, dagger_, Dagger); /* 短剑 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, sword_, Sword); /* 长剑 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, greatsword_, Greatsword); /* 巨剑 */
    /* 刀类 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, scimitar_, Scimitar); /* 弯刀 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, broadsword_, Broadsword); /* 大刀 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, machete_, Machete); /* 大砍刀 */
    /* 锤类 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, warhammer_, Warhammer); /* 轻型战锤 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, heavyDutyHammer_, HeavyDutyHammer); /* 重型战锤 */
    /* 斧类 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, handAxe_, HandAxe); /* 手斧 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, warAxe_, WarAxe); /* 战斧 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, greataxe_, Greataxe); /* 巨斧 */
    /* 枪矛类 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, spear_, Spear); /* 长枪 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, lance_, Lance); /* 长矛 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, giantSickle_, GiantSickle); /* 巨镰 */
    /* 弓弩类 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, shortbow_, Shortbow); /* 短弓 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, longbow_, Longbow); /* 长弓 */
    
    /* 防具 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, helmet_, Helmet); /* 头盔 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, armor_, Armor); /* 铠甲 */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(MCBackpackItem, shinGuard_, ShinGuard); /* 护胫 */
};

#endif /* defined(__Military_Confrontation__MCBackpack__) */
