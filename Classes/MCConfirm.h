//
//  MCConfirm.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCConfirm__
#define __Military_Confrontation__MCConfirm__

#include <cocos2d.h>
USING_NS_CC;

class MCConfirmDelegate;

class MCConfirm : public CCLayerColor {
public:
    ~MCConfirm();
    bool init();
    
    void open();
    void close();
    
    inline const char *getConfirm() {
        return confirm_->getString();
    }
    
    inline void setConfirm(const char *aConfirm) {
        confirm_->setString(aConfirm);
    }
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    static MCConfirm *confirm(CCNode *aParent, MCConfirmDelegate *aDelegate, const char *aMessage);
    
    static bool hasParent();
    
    CREATE_FUNC(MCConfirm);
    
private:
    void showConfirm(CCNode* pSender);
    void hideConfirm(CCNode* pSender);
    void showActionButton(CCNode* pSender);
    void hideActionButton(CCNode* pSender);
    void destroy();
    
    void yesButtonDidClick(CCObject *obj);
    void noButtonDidClick(CCObject *obj);
    
private:
    CCPoint confirmLocation_;
    CCPoint actionButtonsLocation_;
    
    CCLabelTTF *confirm_;
    CCMenuItem *yes_;
    CCMenuItem *no_;
    CCMenu *actionButtonsMenu_;
    
    CC_PROPERTY(MCConfirmDelegate *, delegate_, Delegate);
};

class MCConfirmDelegate {
public:
    virtual void confirmDidClickYesButton(MCConfirm *aConfirm) {};
    virtual void confirmDidClickNoButton(MCConfirm *aConfirm) {};
};

#endif /* defined(__Military_Confrontation__MCConfirm__) */
