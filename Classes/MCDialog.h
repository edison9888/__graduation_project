//
//  MCDialog.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCDialog__
#define __Military_Confrontation__MCDialog__

#include <cocos-ext.h>

#include "MCRole.h"
#include "MCFaceBox.h"

USING_NS_CC_EXT;

enum {
    MCNPCDialog = MCMakeEnum(0), /* NPC说话时的对话框 */
    MCDMDialog  = MCMakeEnum(1)  /* DM说话时的对话框 */
};
typedef mc_enum_t MCDialogType;

class MCDialog : public CCLayer {
protected:
    MCDialog()
    : edge_(NULL)
    , skin_(NULL)
    , dialogue_(NULL)
    , speakerName_(NULL)
    , speakerFace_(NULL)
    , speakerFaceBox_(NULL) {}
public:
    ~MCDialog();
    static MCDialog *sharedDialog(MCDialogType aType);
    
    /* 某人想说几句话 */
    void setMessage(MCRole *aRole);
    
protected:
    CCScale9Sprite *edge_;
    CCScale9Sprite *skin_;
    CCLabelTTF *dialogue_; /* 对白 */
    CCLabelTTF *speakerName_; /* 说话人名字 */
    CCSprite *speakerFace_; /* 说话人头像 */
    CCScale9Sprite *speakerFaceBox_; /* 说话人头像框 */
    
    CC_SYNTHESIZE_READONLY(MCFaceBox *, faceBox_, FaceBox); /* 说话人+相框 */
};

#endif /* defined(__Military_Confrontation__MCDialog__) */
