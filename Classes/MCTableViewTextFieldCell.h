//
//  MCTableViewTextFieldCell.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-13.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTableViewTextFieldCell__
#define __Military_Confrontation__MCTableViewTextFieldCell__

#include <cocos2d.h>
#include <cocos-ext.h>
USING_NS_CC;
USING_NS_CC_EXT;

class MCTableViewTextFieldCell : public CCTableViewCell {
public:
    bool init(const char *string, const char *fontName, float fontSize);
    
    static MCTableViewTextFieldCell *create(const char *string, const char *fontName, float fontSize);
    
    void selected();
    void unselected();
    
    void setString(const char *var);
    const char *getString();
    
private:
    CCLabelTTF *textField_;
};

#endif /* defined(__Military_Confrontation__MCTableViewTextFieldCell__) */
