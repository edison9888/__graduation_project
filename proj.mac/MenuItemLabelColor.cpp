#include "MenuItemLabelColor.h"

const ccColor3B Color_blue = ccc3 (54, 128, 241);
const ccColor3B Color_write = ccc3 (241, 241, 241);

MenuItemLabelColor *
MenuItemLabelColor::create(CCNode*label, CCObject* target, SEL_MenuHandler selector)
{
    MenuItemLabelColor *menuItem;

    menuItem = new MenuItemLabelColor;
    menuItem->initWithLabel (label, target, selector);
    menuItem->autorelease ();
    menuItem->setSelectedColor (Color_blue);
    menuItem->setUnselectedColor (Color_write);

    return menuItem;
}

void
MenuItemLabelColor::selected()
{
    CCMenuItemLabel::selected ();
    this->setColor(m_tSelectedColor);
}

void
MenuItemLabelColor::unselected()
{
    CCMenuItemLabel::unselected ();
    this->setColor(m_tUnselectedColor);
}
