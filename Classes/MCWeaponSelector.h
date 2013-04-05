//
//  MCWeaponSelector.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-3.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCWeaponSelector__
#define __Military_Confrontation__MCWeaponSelector__

#include <cocos2d.h>
USING_NS_CC;

class MCWeaponSelectorDelegate;
class MCEquipmentItem;

class MCWeaponSelector : public CCLayerColor {
public:
    bool init();
    
    void open();
    void close();
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    static MCWeaponSelector *weaponSelector(CCNode *aParent, MCWeaponSelectorDelegate *aDelegate);
    
    CREATE_FUNC(MCWeaponSelector);
    
private:
    void showSelector(CCNode* pSender);
    void hideSelector(CCNode* pSender);
    void destroy();
    
    void weaponDidFinishSelecting(CCObject *obj);
    void cancelButtonDidClick(CCObject *obj);
    
private:
    CCMenu *cancel_;
    CCMenu *weaponsMenu_;
    
    CC_SYNTHESIZE(MCWeaponSelectorDelegate *, delegate_, Delegate);
};

class MCWeaponSelectorDelegate {
public:
    virtual void weaponSelectorDidFinishSelecting(MCWeaponSelector *aWeaponSelector, MCEquipmentItem *aWeapon) {};
    virtual void weaponSelectorDidCancelSelecting(MCWeaponSelector *aWeaponSelector) {};
};

#endif /* defined(__Military_Confrontation__MCWeaponSelector__) */
