//
//  MCEquipmentLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEquipmentLayer__
#define __Military_Confrontation__MCEquipmentLayer__

#include "MCDetailLayer.h"
#include "MCWeaponSelector.h"

class MCEquipmentItem;

class MCEquipmentLayer : public MCDetailLayer, public MCWeaponSelectorDelegate {
public:
    bool init();
    
    void clickFirstItem();
    
    CREATE_FUNC(MCEquipmentLayer);
    
    void weaponSelectorDidFinishSelecting(MCWeaponSelector *aWeaponSelector, MCEquipmentItem *aWeapon);
    void weaponSelectorDidCancelSelecting(MCWeaponSelector *aWeaponSelector);
    
protected:
    void loadData();
    
    void showWeapon();
    void showArmor(MCEquipmentItem *anArmor);
    
    void itemDidClick(CCObject *obj);
    void changeWeapon(CCObject *obj);
    
    void weaponWasSelected(CCObject *obj);
    
private:
    CCLayer *weaponLayer_;
    CCLayer *armorLayer_;
    
    CCLabelTTF *weapon_;
    CCLabelTTF *helmet_;
    CCLabelTTF *armor_;
    CCLabelTTF *shinguard_;
    
    CCLabelTTF *weaponName_;
    CCSprite   *weaponIcon_;
    CCLabelTTF *weaponOre_;
    CCLabelTTF *weaponDexterity_;
    CCLabelTTF *weaponDamage_;
    CCLabelTTF *weaponCriticalHit_;
    CCLabelTTF *weaponDistance_;
    CCLabelTTF *weaponCriticalHitVisible_;
    CCLabelTTF *weaponCriticalHitInvisible_;
    CCLabelTTF *weaponEffect_;
    CCLabelTTF *weaponEffectCheckLabel_;
    CCLabelTTF *weaponEffectCheck_;
    CCLabelTTF *weaponDescription_;
    
    CCLabelTTF *armorName_;
    CCSprite   *armorIcon_;
    CCLabelTTF *armorOre_;
    CCLabelTTF *armorDexterity_;
    CCLabelTTF *armorDefense_;
    CCLabelTTF *armorArmorCheckPenalty_;
    CCLabelTTF *armorDescription_;
};

#endif /* defined(__Military_Confrontation__MCEquipmentLayer__) */
