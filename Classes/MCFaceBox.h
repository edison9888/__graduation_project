//
//  MCFaceBox.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-2.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCFaceBox__
#define __Military_Confrontation__MCFaceBox__

#include <cocos2d.h>
USING_NS_CC;

class MCFaceBox : public CCSprite {
public:
    MCFaceBox();
    
    bool init();
    bool init(CCTexture2D *aFaceTexture, CCTexture2D *aBoxTexture);
    bool init(const char *aFaceFilepath, const char *aBoxFilepath);
    
    CREATE_FUNC(MCFaceBox);
    static MCFaceBox *create(CCTexture2D *aFaceTexture, CCTexture2D *aBoxTexture);
    static MCFaceBox *create(const char *aFaceFilepath, const char *aBoxFilepath);
    
private:
    void alignFace();
    
    CCSprite *face_;
    
    CC_SYNTHESIZE(CCTexture2D *, boxTexture_, BoxTexture);
    CC_SYNTHESIZE(CCTexture2D *, faceTexture_, FaceTexture);
};

#endif /* defined(__Military_Confrontation__MCFaceBox__) */
