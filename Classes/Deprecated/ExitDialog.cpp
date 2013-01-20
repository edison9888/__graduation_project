#include "ExitDialog.h"

#include "MenuItemLabelColor.h"
#include "CCSpriteColor.h"

#include "CCDebug.h"

bool
ExitDialog::init ()
{
    CCSize winSize;
    CCSize dialogSize;

    if (CCLayerColor::initWithColor (ccc4 (32, 32, 32,  64)))
    {
        winSize = CCDirector::sharedDirector ()->getWinSize ();
        dialogSize.width = winSize.width / 2;
        dialogSize.height = winSize.height / 2;

        CCLabelTTF *confirm = CCLabelTTF::create ("Confirm Exit", "Arial", 32);
        confirm->setColor (ccc3 (214, 214, 214));
        this->addChild (confirm, 1);
        m_pConfirmLocation = ccp (winSize.width / 2,
                                  winSize.height / 2 + confirm->getContentSize ().height / 2);
        confirm->setPosition (m_pConfirmLocation);

        CCLabelTTF *label = CCLabelTTF::create ("YES", "Arial", 32);
        CCMenuItemLabel *yesMenuItem = MenuItemLabelColor::create (label);
        label = CCLabelTTF::create ("NO", "Arial", 32);
        CCMenuItemLabel *noMenuItem = MenuItemLabelColor::create (label);

        CCMenu *menu = CCMenu::create (yesMenuItem, noMenuItem, NULL);
        menu->alignItemsHorizontallyWithPadding (64);
        this->addChild (menu, 1);
        CCPoint point = menu->getPosition ();
        point.y = (winSize.height - dialogSize.height) / 2 + label->getContentSize ().height;
        m_pActionButtonsLocation = point;
        menu->setPosition (point);

        m_miConfirm = confirm;
        m_miYes = yesMenuItem;
        m_miNo = noMenuItem;
        m_mActionButtonsMenu = menu;

        return true;
    }

    return false;
}

void
ExitDialog::showDialog()
{
    CCSize winSize = CCDirector::sharedDirector ()->getWinSize ();
    m_miConfirm->setPosition (ccp (-m_miConfirm->getContentSize ().width / 2,
                                   m_pConfirmLocation.y));
    m_mActionButtonsMenu->setPosition (ccp (winSize.width + m_mActionButtonsMenu->getContentSize ().width / 2,
                                            m_pActionButtonsLocation.y));

    runAction (CCSequence::create (
                   CCFadeIn::create (0.2),
                   CCCallFuncN::create (this,
                                        callfuncN_selector (ExitDialog::showConfirm)),
                   CCCallFuncN::create (this,
                                        callfuncN_selector (ExitDialog::showActionButton)),
                   NULL));
}

void
ExitDialog::hideDialog()
{
    runAction (CCSequence::create (
                   CCCallFuncN::create (this,
                                        callfuncN_selector (ExitDialog::hideConfirm)),
                   CCCallFuncN::create (this,
                                        callfuncN_selector (ExitDialog::hideActionButton)),
                   CCDelayTime::create (0.3),
                   CCFadeOut::create (0.2),
                   NULL));
}

void
ExitDialog::showConfirm (CCNode* pSender)
{
    m_miConfirm->runAction (CCMoveTo::create (0.3, m_pConfirmLocation));
}

void
ExitDialog::hideConfirm (CCNode* pSender)
{
    CCSize winSize = CCDirector::sharedDirector ()->getWinSize ();
    m_miConfirm->runAction (CCMoveTo::create (0.3,
                                              ccp (winSize.width + m_miConfirm->getContentSize ().width / 2,
                                                   m_pConfirmLocation.y)));
}

void
ExitDialog::showActionButton (CCNode* pSender)
{
    m_mActionButtonsMenu->runAction (CCMoveTo::create (0.3, m_pActionButtonsLocation));
}

void
ExitDialog::hideActionButton (CCNode* pSender)
{
    CCSize winSize = CCDirector::sharedDirector ()->getWinSize ();
    m_mActionButtonsMenu->runAction (CCMoveTo::create (0.3,
                                                       ccp (-m_mActionButtonsMenu->getContentSize ().width / 2,
                                                            m_pActionButtonsLocation.y)));
}
