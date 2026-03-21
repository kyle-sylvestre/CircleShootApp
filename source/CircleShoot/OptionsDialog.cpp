#include "Zuma_Prefix.pch"

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Slider.h>
#include <SexyAppFramework/Checkbox.h>
#include <SexyAppFramework/Font.h>

#include "CircleShootApp.h"
#include "CircleButton.h"
#include "CircleCheckbox.h"
#include "CircleCommon.h"
#include "OptionsDialog.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
OptionsDialog::OptionsDialog(bool inMainMenu) : CircleDialog(Sexy::IMAGE_DIALOG_BACK, Sexy::IMAGE_DIALOG_BUTTON, 0, true,
                                                             inMainMenu ? LS(STRING_ID_MENU) : LS(STRING_ID_OPTIONS), "",
                                                             inMainMenu ? LS(STRING_ID_BACK_TO_GAME) : LS(STRING_ID_DONE),
                                                             3, false)
{
    mInMainMenu = inMainMenu;

    mMusicVolumeSlider = new Slider(Sexy::IMAGE_SLIDER_TRACK, Sexy::IMAGE_SLIDER_THUMB, 0, this);
    mMusicVolumeSlider->SetValue(gSexyAppBase->GetMusicVolume());

    mSfxVolumeSlider = new Slider(Sexy::IMAGE_SLIDER_TRACK, Sexy::IMAGE_SLIDER_THUMB, 1, this);
    mSfxVolumeSlider->SetValue(gSexyAppBase->GetSfxVolume());

    mFullScreenCheckbox = MakeCheckbox(2, this);
    mCustomCursorsCheckbox = MakeCheckbox(3, this);
    m3DAccelCheckbox = MakeCheckbox(4, this);

    mButtonHelp = MakeButton(5, this, LS(STRING_ID_HELP), CircleButton::CB_ClickSound, NULL, 3);
    mButtonUpdates = MakeButton(6, this, LS(STRING_ID_CHECK_FOR_UPDATES), CircleButton::CB_ClickSound, NULL, 3);
    mButtonRegister = MakeButton(7, this, LS(STRING_ID_REGISTER), CircleButton::CB_ClickSound, NULL, 3);
    mButtonBack = MakeButton(8, this, LS(STRING_ID_BACK_TO_MAIN_MENU), CircleButton::CB_ClickSound, NULL, 3);

    mFullScreenCheckbox->mChecked = !gSexyAppBase->mIsWindowed;
    mCustomCursorsCheckbox->mChecked = gSexyAppBase->mCustomCursorsEnabled;
    m3DAccelCheckbox->mChecked = gSexyAppBase->Is3DAccelerated();
    mButtonBack->SetVisible(inMainMenu);

    // if (gSexyAppBase->mDontUpdate)
    if (true) // todo
    {
        mButtonUpdates->SetVisible(false);
    }

    // if (gSexyAppBase->mMaybeNoRegistration)
    if (true)
    {
        mButtonRegister->SetVisible(false);
    }

    mExtraHeight = 200;
    if (mButtonUpdates->mVisible)
    {
        mExtraHeight = 250;
    }

    if (mButtonBack->mVisible)
    {
        mExtraHeight += 50;
    }
}

OptionsDialog::~OptionsDialog()
{
}

void OptionsDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
    Dialog::Resize(theX, theY, theWidth, theHeight);

    int left = GetLeft();
    int top = GetTop();
    int width = GetWidth();

    mMusicVolumeSlider->Resize(left + 130, top - 15, width - 130, 36);
    mSfxVolumeSlider->Layout(LAY_SameLeft | LAY_Below | LAY_SameSize, mMusicVolumeSlider, 0, 0, 0, 0);
    mFullScreenCheckbox->Layout(LAY_Below | LAY_SetLeft, mSfxVolumeSlider, left, 0, 0, 0);
    mCustomCursorsCheckbox->Layout(LAY_SameTop | LAY_Right, mFullScreenCheckbox, 90, 0, 0, 0);
    m3DAccelCheckbox->Layout(LAY_SameLeft | LAY_Below, mFullScreenCheckbox, 0, 0, 0, 0);

    if (mButtonRegister->mVisible)
    {
        mButtonHelp->Layout(LAY_SameLeft | LAY_Below | LAY_SetWidth, m3DAccelCheckbox, 0, 0, width / 2, 0);
        mButtonRegister->Layout(LAY_SameTop | LAY_Right | LAY_SameSize, mButtonHelp, 0, 0, width, 0);
    }
    else
    {
        mButtonHelp->Layout(LAY_SameLeft | LAY_Below | LAY_SetWidth, m3DAccelCheckbox, 0, 0, width, 0);
    }

    if (mButtonUpdates->mVisible)
    {
        mButtonUpdates->Layout(LAY_SameLeft | LAY_Below | LAY_SetWidth, mButtonHelp, 0, 0, width, 0);
        mButtonBack->Layout(LAY_SameLeft | LAY_Below | LAY_SameSize, mButtonUpdates, 0, 0, 0, 0);
    }
    else
    {
        mButtonBack->Layout(LAY_SameLeft | LAY_Below | LAY_SetWidth, mButtonHelp, 0, 0, width, 0);
    }
}

int OptionsDialog::GetPreferredHeight(int theWidth)
{
    return mExtraHeight + CircleDialog::GetPreferredHeight(theWidth);
}

void OptionsDialog::AddedToManager(WidgetManager *theWidgetManager)
{
    Dialog::AddedToManager(theWidgetManager);

    theWidgetManager->AddWidget(mMusicVolumeSlider);
    theWidgetManager->AddWidget(mSfxVolumeSlider);
    theWidgetManager->AddWidget(mFullScreenCheckbox);
    theWidgetManager->AddWidget(mCustomCursorsCheckbox);
    theWidgetManager->AddWidget(m3DAccelCheckbox);
    theWidgetManager->AddWidget(mButtonHelp);
    theWidgetManager->AddWidget(mButtonRegister);
    theWidgetManager->AddWidget(mButtonBack);
    theWidgetManager->AddWidget(mButtonUpdates);
}

void OptionsDialog::RemovedFromManager(WidgetManager *theWidgetManager)
{
    Dialog::RemovedFromManager(theWidgetManager);
    theWidgetManager->RemoveWidget(mMusicVolumeSlider);
    theWidgetManager->RemoveWidget(mSfxVolumeSlider);
    theWidgetManager->RemoveWidget(mFullScreenCheckbox);
    theWidgetManager->RemoveWidget(mCustomCursorsCheckbox);
    theWidgetManager->RemoveWidget(m3DAccelCheckbox);
    theWidgetManager->RemoveWidget(mButtonHelp);
    theWidgetManager->RemoveWidget(mButtonRegister);
    theWidgetManager->RemoveWidget(mButtonUpdates);
    theWidgetManager->RemoveWidget(mButtonBack);
}

void OptionsDialog::Draw(Graphics *g)
{
    CircleDialog::Draw(g);

    g->SetFont(FONT_DIALOG);
    g->SetColor(mColors[COLOR_LINES]);

    int aPosX = mFullScreenCheckbox->mX - mX + 20;

    g->DrawString(LS(STRING_ID_MUSIC_VOLUME), aPosX, mMusicVolumeSlider->mY - mY + FONT_DIALOG->GetAscent() + 5);
    g->DrawString(LS(STRING_ID_SOUND_EFFECTS), aPosX, mSfxVolumeSlider->mY - mY + FONT_DIALOG->GetAscent() + 5);

    DrawCheckboxText(g, LS(STRING_ID_FULLSCREEN), mFullScreenCheckbox);
    DrawCheckboxText(g, LS(STRING_ID_CUSTOM_CURSORS), mCustomCursorsCheckbox);
    DrawCheckboxText(g, Sexy::StrFormat("    %s    ", LS(STRING_ID_3D_HARDWARE_ACCELERATION)), m3DAccelCheckbox);
    //DrawCheckboxText(g, "    3-D Hardware Acceleration          ", m3DAccelCheckbox);
}

void OptionsDialog::ButtonPress(int theId)
{
    CircleDialog::ButtonPress(theId);
}

void OptionsDialog::ButtonDepress(int theId)
{
    CircleDialog::ButtonDepress(theId);

    if (theId == 6)
    {
        GetCircleShootApp()->DoCheckForUpdatesDialog();
    }
    else if (theId == 7)
    {
        GetCircleShootApp()->DoRegisterDialog();
    }
    else if (theId == 8)
    {
        GetCircleShootApp()->ReturnToMainMenu();
    }
    else if (theId == 5)
    {
        GetCircleShootApp()->ShowHelpScreen();
    }
}

void OptionsDialog::SliderVal(int theId, double theVal)
{
    if (theId == 0)
    {
        gSexyAppBase->SetMusicVolume(theVal);
    }
    else if (theId == 1)
    {
        gSexyAppBase->SetSfxVolume(theVal);
    }
}

void OptionsDialog::CheckboxChecked(int theId, bool checked)
{
    if (theId == 2)
    {
        if (gSexyAppBase->mForceFullscreen && !checked)
        {
            gSexyAppBase->DoDialog(6, true, 
                                   LS(STRING_ID_NO_WINDOWED_MODE),
                                   LS(STRING_ID_WINDOWED_MODE_IS_ONLY_AVAILABLE),
                                   LS(DIALOG_BUTTON_OK),
                                   BUTTONS_FOOTER);
            mFullScreenCheckbox->mChecked = true;
        }
    }
    else if (theId == 4 && checked)
    {
        if (gSexyAppBase->Is3DAccelerationSupported())
        {
            if (!gSexyAppBase->Is3DAccelerationRecommended())
            {
                gSexyAppBase->DoDialog(5, true, 
                                       LS(STRING_ID_WARNING),
                                       LS(STRING_ID_YOUR_VIDEO_CARD_MAY_NOT_FULLY_SUPPORT_THIS_FEATURE),
                                       LS(DIALOG_BUTTON_OK),
                                       BUTTONS_FOOTER);
            }
        }
        else
        {
            m3DAccelCheckbox->mChecked = false;
            gSexyAppBase->DoDialog(5, true, 
                                   LS(STRING_ID_NOT_SUPPORTED),
                                   LS(STRING_ID_HARDWARE_ACCELERATION_CANNOT_BE_ENABLED),
                                   LS(DIALOG_BUTTON_OK), 
                                   BUTTONS_FOOTER);
        }
    }
}
