#ifndef MENUITEMLABELCOLOR_H
#define MENUITEMLABELCOLOR_H

#include <cocos2d.h>

USING_NS_CC;

class MenuItemLabelColor : public CCMenuItemLabel
{
public:
    static MenuItemLabelColor *create(CCNode*label, CCObject* target=NULL, SEL_MenuHandler selector=NULL);

    CC_SYNTHESIZE_PASS_BY_REF (ccColor3B, m_tSelectedColor, SelectedColor)
    CC_SYNTHESIZE_PASS_BY_REF (ccColor3B, m_tUnselectedColor, UnselectedColor)

    void selected();
    void unselected();
};

#endif // MENUITEMLABELCOLOR_H
