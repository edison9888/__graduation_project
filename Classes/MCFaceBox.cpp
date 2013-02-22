//
//  MCFaceBox.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCFaceBox.h"

MCFaceBox::MCFaceBox()
{
    face_ = CCSprite::create();
    faceTexture_ = NULL;
    boxTexture_ = NULL;
}

bool
MCFaceBox::init()
{
    if (CCSprite::init()) {
        return true;
    }
    
    return false;
}

bool
MCFaceBox::init(CCTexture2D *aFaceTexture, CCTexture2D *aBoxTexture)
{
    if (CCSprite::init()) {
        CCRect rect = CCRectZero;
        faceTexture_ = aFaceTexture;
        boxTexture_ = aBoxTexture;
        
        setTexture(aBoxTexture);
        rect.size = aBoxTexture->getContentSize();
        setTextureRect(rect);
        
        face_->setTexture(aFaceTexture);
        rect.size = aFaceTexture->getContentSize();
        face_->setTextureRect(rect);
        if (! face_->getParent()) {
            addChild(face_);
            alignFace();
        }
        
        return true;
    }
    
    return false;
}

bool
MCFaceBox::init(const char *aFaceFilepath, const char *aBoxFilepath)
{
    
    do {
        CCImage *aFaceImage = new CCImage;
        CC_BREAK_IF(! aFaceImage);
        CCImage *aBoxImage = new CCImage;
        CC_BREAK_IF(! aBoxImage);
        CCTexture2D *aFaceTexture = new CCTexture2D;
        CC_BREAK_IF(! aFaceTexture);
        CCTexture2D *aBoxTexture = new CCTexture2D;
        CC_BREAK_IF(! aBoxTexture);
        
        CC_BREAK_IF(! aFaceImage->initWithImageFile(aFaceFilepath));
        CC_BREAK_IF(! aBoxImage->initWithImageFile(aBoxFilepath));
        
        CC_BREAK_IF(! aFaceTexture->initWithImage(aFaceImage));
        CC_BREAK_IF(! aBoxTexture->initWithImage(aBoxImage));
        
        return init(aFaceTexture, aBoxTexture);
    } while (0);
    
    return false;
}

MCFaceBox *
MCFaceBox::create(CCTexture2D *aFace, CCTexture2D *aBox)
{
    MCFaceBox *faceBox = new MCFaceBox;
    
    if (faceBox && faceBox->init(aFace, aBox)) {
        faceBox->autorelease();
    } else {
        CC_SAFE_DELETE(faceBox);
        faceBox = NULL;
    }
    
    return faceBox;
}

MCFaceBox *
MCFaceBox::create(const char *aFaceFilepath, const char *aBoxFilepath)
{
    MCFaceBox *faceBox = new MCFaceBox;
    
    if (faceBox && faceBox->init(aFaceFilepath, aBoxFilepath)) {
        faceBox->autorelease();
    } else {
        CC_SAFE_DELETE(faceBox);
        faceBox = NULL;
    }
    
    return faceBox;
}

void
MCFaceBox::alignFace()
{
    CCSize size = getContentSize();
    
    face_->setAnchorPoint(ccp(0.5, 0.5));
    face_->setPosition(ccp(size.width / 2, size.height / 2));
}
