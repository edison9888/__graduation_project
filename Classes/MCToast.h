//
//  MCToast.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-6.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCToast__
#define __Military_Confrontation__MCToast__

#include "MCDialog.h"

enum {
    MCLengthShort = 1, /* 1秒 */
    MCLengthLong  = 4  /* 4秒 */
};
typedef float MCToastLength;

class MCToast : public MCDialog {
private:
    bool init();
public:
    
    static MCToast *make(CCNode *aParent, const char *aMessage, MCToastLength aToastLength = MCLengthShort);
    
    void show();
    void cancel();
    
protected:
    void hide(CCObject *obj);
    void destroy(CCObject *obj);
    
    CC_SYNTHESIZE_READONLY(CCNode *, parent_, Parent);
    CC_SYNTHESIZE(CCString *, message_, Message);
    CC_SYNTHESIZE(MCToastLength, length_, Length);
};

#endif /* defined(__Military_Confrontation__MCToast__) */
