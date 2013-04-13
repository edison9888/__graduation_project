//
//  MCEnhancingEquipmentUI.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-12.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCEnhancingEquipmentUI__
#define __Military_Confrontation__MCEnhancingEquipmentUI__

#include "MCType.h"
#include "MCConfirm.h"

class MCScene;
class MCEquipmentItem;

class MCEnhancingEquipmentUI : public CCLayer, public MCConfirmDelegate {
public:
    bool init();
    
    void initPosition();
    void attach(MCScene *aScene);
    void detach();
    
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    
    CREATE_FUNC(MCEnhancingEquipmentUI);
    
protected:
    void equipmentType_clicked(CCObject *obj);
    void item_clicked(CCObject *obj);
    
    void levelUp_click(CCObject *aSender);
    
    void backward(CCObject *aSender); /* 关闭.... */
    
    void destroy();
    
    void loadWeapon(MCEquipmentItem *aWeapon);
    void loadArmor(MCEquipmentItem *anArmor);
    
    CCLayer *infoLayer_; /* 显示装备信息的层 */
    CCMenu *currentMenu_;
    CCMenuItemLabel *lastSelectedEquipmentTypeMenuItem_;
    CCMenuItemLabel *lastSelectedEquipmentMenuItem_;
    
    CCLayer *weaponLayer_;
    CCLayer *armorLayer_;
    CCMenu *levelUpButton_;
    
    CCLabelTTF *money_;
    
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
    CCLabelTTF *weaponPriceLabel_;
    CCLabelTTF *weaponPrice_;
    CCLabelTTF *weaponDescription_;
    
    CCLabelTTF *armorName_;
    CCSprite   *armorIcon_;
    CCLabelTTF *armorOre_;
    CCLabelTTF *armorDexterity_;
    CCLabelTTF *armorDefense_;
    CCLabelTTF *armorArmorCheckPenalty_;
    CCLabelTTF *armorPriceLabel_;
    CCLabelTTF *armorPrice_;
    CCLabelTTF *armorDescription_;
    
};

#endif /* defined(__Military_Confrontation__MCEnhancingEquipmentUI__) */
