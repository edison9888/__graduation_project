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
#include <cocos-ext.h>

USING_NS_CC_EXT;

class MCScene;
class MCEquipmentItem;
class MCTableViewTextFieldCell;

class MCEnhancingEquipmentUI : public CCLayer, public MCConfirmDelegate, public CCTableViewDataSource, public CCTableViewDelegate {
public:
    bool init();
    
    void initPosition();
    void attach(MCScene *aScene);
    void detach();
    
    void confirmDidClickYesButton(MCConfirm *aConfirm);
    
    /* CCScrollViewDelegate */
    void scrollViewDidScroll(CCScrollView *view) {}
    void scrollViewDidZoom(CCScrollView *view) {}
    
    /* CCTableViewDataSource */
    CCSize cellSizeForTable(CCTableView *table);
    CCTableViewCell *tableCellAtIndex(CCTableView *table, unsigned int idx);
    unsigned int numberOfCellsInTableView(CCTableView *table);
    
    /* CCTableViewDelegate */
    void tableCellTouched(CCTableView *table, CCTableViewCell *cell);
    
    CREATE_FUNC(MCEnhancingEquipmentUI);
    
protected:
    void equipmentType_clicked(CCObject *obj);
    
    void levelUp_click(CCObject *aSender);
    
    void backward(CCObject *aSender); /* 关闭.... */
    
    void destroy();
    
    void loadWeapon(MCEquipmentItem *aWeapon);
    void loadArmor(MCEquipmentItem *anArmor);
    
    CCTableView *tableView_; /* 装备列表 */
    CCSize tableViewSize_;
    MCTableViewTextFieldCell *selectedCell_;
    
    CCLayer *infoLayer_; /* 显示装备信息的层 */
    CCArray *equipments_;
    CCMenuItemLabel *lastSelectedEquipmentTypeMenuItem_;
    MCEquipmentItem *lastSelectedEquipmentItem_;
    
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
