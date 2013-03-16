//
//  MCActionMenu.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-13.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCActionMenu__
#define __Military_Confrontation__MCActionMenu__

#include <cocos2d.h>
USING_NS_CC;

#include "MCType.h"

enum {
    MCFloatMenu = MCMakeEnum(0), /* 浮动菜单 */
    MCFixedMenu = MCMakeEnum(1)  /* 固定菜单 */
};
typedef mc_enum_t MCActionMenuType;

class MCGameScene;
class MCActionMenu;

class MCMenuItemAction : public CCObject {
    friend class MCActionMenu;
    
    CC_SYNTHESIZE(mc_dict_key_t, id_, ID);
    CC_SYNTHESIZE(CCString *, label_, Label);
    CC_SYNTHESIZE(CCObject *, target_, Target);
    CC_SYNTHESIZE(SEL_MenuHandler, menuHandler_, MenuHandler);
};

class MCActionMenu : public CCMenu {
public:
    MCActionMenu();
    ~MCActionMenu();
    
    bool init(MCActionMenuType anActionMenuType);
    
    static MCActionMenu *create(MCActionMenuType anActionMenuType);
    
    void openMenu(CCObject *aMenuItem);
    void closeMenu(CCObject *aMenuItem);
    
    void onEnter();
    void onExit();
    
    /**
     * 依附在场景中
     */
    void attach(MCGameScene *aGameScene);
    
    /**
     * 从被依附的场景中脱离
     */
    void detach();
    
    void addActionGroup(mc_dict_key_t aGroupId, CCArray *actions);
    
    void removeActionGroup(mc_dict_key_t aGroupId);
    
    void setActiveActionGroup(mc_dict_key_t aGroupId);
    
//    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    CC_SYNTHESIZE_READONLY(MCActionMenuType, actionMenuType_, ActionMenuType);
    CC_SYNTHESIZE_READONLY(MCGameScene *, gameScene_, GameScene);
    
protected:
    void showActions();
    void hideActions();
    
private:
    CCMenuItemImage *actionItem_;
    CCMenuItemLabel *closeItem_;
    
    CCDictionary *actionGroups_;
    CCArray *activeActionGroup_;
    float activeActionGroupRadius_;
    CCDictionary *actionItemHolder_;
};

class MCActionMenuDelegate {
public:
    virtual MCActionMenu *actionMenu() = 0;
    virtual void actionMenuDidOpen(MCActionMenu *anActionMenu) { CC_UNUSED_PARAM(anActionMenu); }
    virtual void actionMenuDidClosd(MCActionMenu *anActionMenu) { CC_UNUSED_PARAM(anActionMenu); }
};

#endif /* defined(__Military_Confrontation__MCActionMenu__) */
