#ifndef EXITDIALOG_H
#define EXITDIALOG_H

#include <cocos2d.h>
USING_NS_CC;

class ExitDialog : public CCLayerColor
{
public:
    virtual bool init();

    void showDialog();
    void hideDialog();

private:
    void showConfirm();
    void hideConfirm();
    void showActionButton();
    void hideActionButton();

    CREATE_FUNC(ExitDialog);

private:
    CCPoint m_pConfirmLocation;
    CCPoint m_pActionButtonsLocation;

    CC_SYNTHESIZE_READONLY (CCLabelTTF *, m_miConfirm, Confirm)
    CC_SYNTHESIZE_READONLY (CCMenuItem *, m_miYes, Yes)
    CC_SYNTHESIZE_READONLY (CCMenuItem *, m_miNo, No)
    CC_SYNTHESIZE_READONLY (CCMenu *, m_mActionButtonsMenu, ActionButtonsMenu)
};

#endif // EXITDIALOG_H
