//
//  MCTableViewTextFieldCell.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-4-13.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCTableViewTextFieldCell.h"

static const ccColor3B kMCUnselectedColor = ccc3(255, 255, 255);
static const ccColor3B kMCSelectedColor = ccc3(240, 196, 64);

bool
MCTableViewTextFieldCell::init(const char *string, const char *fontName, float fontSize)
{
    textField_ = CCLabelTTF::create(string, fontName, fontSize);
    textField_->setAnchorPoint(CCPointZero);
    textField_->setPosition(CCPointZero);
    addChild(textField_);
    
    return true;
}

MCTableViewTextFieldCell *
MCTableViewTextFieldCell::create(const char *string, const char *fontName, float fontSize)
{
    MCTableViewTextFieldCell *tableViewTextFieldCell = new MCTableViewTextFieldCell;
    
    if (tableViewTextFieldCell && tableViewTextFieldCell->init(string, fontName, fontSize)) {
        tableViewTextFieldCell->autorelease();
    } else {
        delete tableViewTextFieldCell;
        tableViewTextFieldCell = NULL;
    }
    
    return tableViewTextFieldCell;
}

void
MCTableViewTextFieldCell::selected()
{
    textField_->setColor(kMCSelectedColor);
}

void
MCTableViewTextFieldCell::unselected()
{
    textField_->setColor(kMCUnselectedColor);
}

void
MCTableViewTextFieldCell::setString(const char *var)
{
    textField_->setString(var);
}

const char *
MCTableViewTextFieldCell::getString()
{
    return textField_->getString();
}
