#include "Zuma_Prefix.pch"

#include <SexyAppFramework/MTRand.h>
#include <SexyAppFramework/MusicInterface.h>
#include <SexyAppFramework/ResourceManager.h>
#include <SexyAppFramework/MemoryImage.h>
#include <SexyAppFramework/Dialog.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/ImageFont.h>
#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Checkbox.h>
#include <SexyAppFramework/DialogButton.h>
#include <SexyAppFramework/Rect.h>
#include <SexyAppFramework/graphics/GLInterface.h>
#include <SexyAppFramework/widget/Slider.h>
#include <algorithm>

#include "CircleButton.h"
#include "CurveMgr.h"
#include "SoundMgr.h"
#include "TransitionMgr.h"
#include "Gun.h"
#include "Board.h"
#include "DataSync.h"
#include "CircleCommon.h"
#include "CircleShootApp.h"
#include "LoadingScreen.h"
#include "MainMenu.h"
#include "CreateUserDialog.h"
#include "OptionsDialog.h"
#include "StatsDialog.h"
#include "UserDialog.h"
#include "AdventureScreen.h"
#include "PracticeScreen.h"
#include "MoreGamesScreen.h"
#include "CreditsScreen.h"
#include "HelpScreen.h"
#include "LevelParser.h"
#include "ProfileMgr.h"
#include "HighScoreMgr.h"
#include "WidgetMover.h"
#include "WorkerThread.h"
#include "Res.h"

using namespace Sexy;

CircleShootApp::CircleShootApp()
{
    mTitle = "Zuma Deluxe " + mProductVersion;
    SetString("RegistryKey", "PopCap\\Zuma");

    mAutoEnable3D = true;

    mLevelParser = new LevelParser();
    mProfileMgr = new ProfileMgr();
    mHighScoreMgr = new HighScoreMgr();
    mWidgetMover = new WidgetMover();
    mWorkerThread = new WorkerThread();

    mWidth = CIRCLE_WINDOW_WIDTH;
    mHeight = CIRCLE_WINDOW_HEIGHT;

    mAdventureScreen = NULL;
    mBoard = NULL;
    mMainMenu = NULL;
    mPracticeScreen = NULL;
    mProfile = NULL;
    mHelpScreen = NULL;
    mMoreGamesScreen = NULL;
    mLoadingScreen = NULL;
    mCreditsScreen = NULL;

    mSongId = 0;
    mLastSong = -1;
    mUnk28 = 0;
    mMusicVolume = 0.6;
    mSfxVolume = 0.6;
    mDoPlayCount = false;
    mPlayCount = 0;
    mUnk29 = 0;
    mMaxExecutions = 0;
    mMaxPlays = 0;
    mMaxTime = 0;
    mController = NULL;
    mControllerIndex = -1;
    mControllerWidget = NULL;
}

CircleShootApp::~CircleShootApp()
{
    // Sexy::SpriteMgrDeleteGlobals()
    // Sexy::BallDeleteGlobals()
    // Sexy::GunDeleteGlobals()

    delete mLevelParser;
    delete mProfileMgr;
    delete mHighScoreMgr;
    delete mWidgetMover;
    delete mWorkerThread;
    if (mController)
    {
        SDL_GameControllerClose(mController);
        mController = NULL;
        mControllerIndex = -1;
        mControllerWidget = NULL;
    }

    // mResourceManager->DeleteResources("");
}

void CircleShootApp::Init()
{
    SexyAppBase::Init();
    if (mShutdown)
        return;
    
    mMuteOnLostFocus = true;

    SetupLocalizations();
    LoadProperties();
    mMaxExecutions = GetInteger("MaxExecutions", 0);
    mMaxPlays = GetInteger("MaxPlays", 0);
    mMaxTime = GetInteger("MaxTime", 60);

    mLastSongSwitchTime = SDL_GetTicks() - 100000;
    gMainThreadId = std::this_thread::get_id();

    gThreadRand.SRand(Sexy::Rand());
    gAppRand.SRand(Sexy::Rand());

    this->mProfileMgr->Load();
    this->mHighScoreMgr->Load();

    std::string profile;

    if (this->RegistryReadString("CurUser", &profile))
    {
        mProfile = mProfileMgr->GetProfile(profile);
    }
    
    if (!mProfile)
    {
        mProfile = mProfileMgr->GetAnyProfile();
    }

    if (!mLevelParser->ParseLevelFile("levels/levels.xml"))
    {
        Popup(mLevelParser->GetErrorText());
        exit(0);
    }

    if (mLevelParser->mLevels.size() == 0)
    {
        Popup("No levels defined in levels.xml");
        exit(0);
    }

    // mResourceManager->SetAllowMissingProgramImages(true); // hack?

    LoadResourceManifest();
    if (!mResourceManager->LoadResources("Init"))
    {
        ShowResourceError(true);
    }

    ImageFont *fontMain10 = reinterpret_cast<ImageFont *>(mResourceManager->GetFont("FONT_MAIN10"));
    if (fontMain10)
    {
        ImageFont *fontDialog = reinterpret_cast<ImageFont *>(fontMain10->Duplicate());
        fontDialog->AddTag("Shadow1");
        mResourceManager->ReplaceFont("FONT_DIALOG", fontDialog);

        ImageFont *fontMain10Outline = reinterpret_cast<ImageFont *>(fontMain10->Duplicate());
        fontMain10Outline->RemoveTag("Outline");
        mResourceManager->ReplaceFont("FONT_MAIN10OUTLINE", fontMain10Outline);

        ImageFont *fontMain10Outline2 = reinterpret_cast<ImageFont *>(fontMain10->Duplicate());
        fontMain10Outline2->RemoveTag("Outline2");
        mResourceManager->ReplaceFont("FONT_MAIN10OUTLINE2", fontMain10Outline2);

        ImageFont *fontMain10Outline3 = reinterpret_cast<ImageFont *>(fontMain10->Duplicate());
        fontMain10Outline3->RemoveTag("Outline3");
        mResourceManager->ReplaceFont("FONT_MAIN10OUTLINE3", fontMain10Outline3);
    }

    if (!ExtractInitResources(mResourceManager))
    {
        ShowResourceError(true);
    }

    SetCursorImage(0, Sexy::IMAGE_CURSOR_POINTER);
    SetCursorImage(1, Sexy::IMAGE_CURSOR_HAND);
    SetCursorImage(2, Sexy::IMAGE_CURSOR_DRAGGING);
    SetCursorImage(3, Sexy::IMAGE_CURSOR_TEXT);

    std::string music_it = Sexy::GetResourcePath("music/zuma.it");
    std::string music_mo3 = Sexy::GetResourcePath("music/zuma.mo3");
    if (Sexy::FileExists(music_it))
    {
        mMusicInterface->LoadMusic(0, music_it);
        mMusicInterface->LoadMusic(1, music_it);
    }
    else
    {
        mMusicInterface->LoadMusic(0, music_mo3);
        mMusicInterface->LoadMusic(1, music_mo3);
    }
    PlaySong(24, false, 0.01);
    ShowLoadingScreen();
}

void CircleShootApp::Shutdown()
{
    if (!this->mShutdown)
    {
        CleanupWidgets();

        if (this->mMaxTime <= 0)
        {
            if (this->mMaxPlays > 0)
            {
                // todo
            }
        }
        else
        {
            // todo
        }

        SexyAppBase::Shutdown();
    }
}

void CircleShootApp::UpdateFrames()
{
    SexyAppBase::UpdateFrames();
    mWidgetMover->Update();

    if (mBoard && mController)
    {
        int dir = 0;
        if (SDL_GameControllerGetButton(mController, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
            dir = 1;
        else if (SDL_GameControllerGetButton(mController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
            dir = -1;

        if (dir)
        {
            const float TURN_RATE = SEXY_PI / (100.0f * 1.5f);
            float rot = TURN_RATE * dir;
            mBoard->mGun->SetAngle(mBoard->mGun->GetAngle() + rot);
            mBoard->DoAccuracy(mBoard->mAccuracyCount > 0);
            mBoard->mRecalcGuide = true;
        }
    }
}

void CircleShootApp::ButtonDepress(int theId)
{
    CheckYesNoButton(theId);
}

Dialog *CircleShootApp::NewDialog(int theDialogId,
                                  bool isModal,
                                  const SexyString &theDialogHeader,
                                  const SexyString &theDialogLines,
                                  const SexyString &theDialogFooter,
                                  int theButtonMode)
{
    CircleDialog *aDialog = new CircleDialog(Sexy::IMAGE_DIALOG_BACK,
                                             Sexy::IMAGE_DIALOG_BUTTON,
                                             theDialogId,
                                             isModal,
                                             theDialogHeader,
                                             theDialogLines,
                                             theDialogFooter,
                                             theButtonMode,
                                             false);

    Sexy::SetupDialog(aDialog, 348);
    return aDialog;
}

bool CircleShootApp::KillDialog(int theDialogId)
{
    Widget *aDialog = GetDialog(theDialogId);
    if (aDialog)
    {
        mWidgetMover->RemoveWidget(aDialog);
    }

    if (!SexyAppBase::KillDialog(theDialogId))
    {
        return false;
    }

    if (mDialogMap.empty())
    {
        mWidgetManager->SetFocus(mBoard);
    }

    return true;
}

void CircleShootApp::GotFocus()
{
    if (mBoard)
    {
        mBoard->Pause(false);
    }
}

void CircleShootApp::LostFocus()
{
    if (mBoard)
    {
        mBoard->Pause(true);
#if defined(__ANDROID__) || defined(__SWITCH__)
        mBoard->SaveGame(GetSaveGameName(mBoard->IsPracticeMode(), mProfile->mId));
#endif
    }
}

void CircleShootApp::MakeBoard()
{
    mDidNextTempleDialog = false;
    CleanupWidgets();

    mBoard = new Board(this);

    mBoard->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mBoard);
    mWidgetManager->SetFocus(mBoard);
    mDoPlayCount = false;
}

void CircleShootApp::CleanupWidgets()
{
    if (mAdventureScreen)
    {
        mWidgetMover->SafeDeleteWidget(mAdventureScreen);
        mAdventureScreen = NULL;
    }

    if (mBoard)
    {
        if (mDoPlayCount)
        {
            mPlayCount++;
            mDoPlayCount = false;
        }

        mBoard->WaitForLoadingThread();
        if (mBoard->NeedSaveGame())
        {
            mBoard->SaveGame(GetSaveGameName(mBoard->IsPracticeMode(), mProfile->mId));
        }

        mWidgetMover->SafeDeleteWidget(mBoard);
        mBoard = NULL;
    }

    if (mLoadingScreen)
    {
        mWidgetMover->SafeDeleteWidget(mLoadingScreen);
        mLoadingScreen = NULL;
    }

    if (mMainMenu)
    {
        mWidgetMover->SafeDeleteWidget(mMainMenu);
        mMainMenu = NULL;
    }

    if (mPracticeScreen)
    {
        mWidgetMover->SafeDeleteWidget(mPracticeScreen);
        mPracticeScreen = NULL;
    }

    if (mMoreGamesScreen)
    {
        mWidgetMover->SafeDeleteWidget(mMoreGamesScreen);
        mMoreGamesScreen = NULL;
    }

    if (mHelpScreen)
    {
        mWidgetMover->SafeDeleteWidget(mHelpScreen);
        mHelpScreen = NULL;
    }

    if (mCreditsScreen)
    {
        mWidgetMover->SafeDeleteWidget(mCreditsScreen);
        mCreditsScreen = NULL;
    }
}

void CircleShootApp::ShowLoadingScreen()
{
    CleanupWidgets();

    mLoadingScreen = new LoadingScreen();

    mLoadingScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mLoadingScreen);
    mWidgetManager->SetFocus(mLoadingScreen);
}

void CircleShootApp::ShowHelpScreen()
{
    mHelpScreen = new HelpScreen();
    mHelpScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mHelpScreen);
    mWidgetManager->SetFocus(mHelpScreen);
    if (mBoard)
    {
        mBoard->Pause(true, true);
    }
    ClearUpdateBacklog();
}

void CircleShootApp::StartAdventureGame(int theStage)
{
    if (mDidNextTempleDialog && theStage == mProfile->mMaxStage && CheckSaveGame(false))
    {
        StartSavedGame(true);
    }
    else
    {
        MakeBoard();
        mBoard->SetStartLevel(mLevelParser->GetLevelNumByStage(theStage));
        mBoard->Reset(true);
        if (mProfile->mShowHelpScreen)
        {
            ShowHelpScreen();
        }

        PlaySong(0, 1, 0.01);
    }
}

void CircleShootApp::StartPracticeGame(const std::string &theLevelName, int theStartLevel, bool endless)
{
    MakeBoard();
    mBoard->SetPracticeBoard(theLevelName);
    mBoard->SetStartLevel(theStartLevel);
    mBoard->SetIsEndless(endless);
    mBoard->Reset(true);
    if (mProfile->mShowHelpScreen)
    {
        ShowHelpScreen();
    }

    PlaySong(0, true, 0.01);
}

bool CircleShootApp::CheckSaveGame(bool showConfirm)
{
    std::string aSaveGameName = GetSaveGameName(mIsPractice, mProfile->mId);
    if (!ReadBufferFromFile(aSaveGameName, &mSaveGameBuffer))
        return false;

    DataReader aReader;
    aReader.OpenMemory(mSaveGameBuffer.GetDataPtr(), mSaveGameBuffer.GetDataLen(), false);
    if (aReader.ReadLong() != gSaveGameVersion)
    {
        EraseFile(aSaveGameName);
        mSaveGameBuffer.Clear();
        return false;
    }

    std::string aVerboseLevelString;
    std::string aLevelDisplayName;
    bool aIsPractice = aReader.ReadBool();
    aReader.ReadString(aVerboseLevelString);
    aReader.ReadString(aLevelDisplayName);
    int aScore = aReader.ReadLong();
    if (showConfirm)
    {
        if (aScore > 0)
        {
            DoConfirmContinueDialog(aVerboseLevelString, aLevelDisplayName, aScore);
        }
        else
        {
            EraseFile(aSaveGameName);
            mSaveGameBuffer.Clear();
        }
    }
    else
    {
        EraseFile(aSaveGameName);
    }

    return true;
}

void CircleShootApp::StartSavedGame(bool showConfirm)
{
    MakeBoard();
    mBoard->SetStartLevel(0);
    mBoard->LoadGame(mSaveGameBuffer);
    mSaveGameBuffer.Clear();
    if (!showConfirm)
    {
        if (mProfile->mShowHelpScreen)
        {
            ShowHelpScreen();
        }
        else
        {
            DoGetReadyDialog();
        }
    }
    PlaySong(0, true, 0.01);
}

void CircleShootApp::SaveProfile()
{
    if (mProfile)
    {
        mProfile->SaveDetails();
    }
}

void CircleShootApp::LoadingThreadProc()
{
    const char *resourceGroups[5];
    resourceGroups[0] = "Register";
    resourceGroups[1] = "LoadingThread";
    resourceGroups[2] = "MainMenu";
    resourceGroups[3] = "AdventureScreen";
    resourceGroups[4] = "GauntletScreen";

    // check for funky zylom MenuMain resource group
    {
        std::string file = Sexy::GetResourcePath("properties/resources.xml");
        FILE *f = fopen(file.c_str(), "rb");
        if (f)
        {
            const char *group_name = "MenuMain";
            char buf[32 * 1024] = {};
            fread(buf, 1, sizeof(buf), f);
            buf[SDL_arraysize(buf) - 1] = '\0';
            const char *p = strstr(buf, group_name);
            if (p)
            {
                resourceGroups[2] = group_name;
            }
            fclose(f);
        }
    }
    
    mUnk28 = 0;
    mNumLoadingThreadTasks = 0;
    int i;

    for (i = 0; i < 5; i++)
    {
        mNumLoadingThreadTasks +=
            mResourceManager->GetNumResources(resourceGroups[i]);
    }
    ++mNumLoadingThreadTasks;

    i = 0;
    for (;;)
    {
        mResourceManager->StartLoadResources(resourceGroups[i]);

        while (mResourceManager->LoadNextResource())
            ++mCompletedLoadingThreadTasks;

        if (mResourceManager->HadError() ||
            i == 0 && !Sexy::ExtractResourcesByName(mResourceManager,
                                                    resourceGroups[0]))
        {
            break;
        }

        ++mUnk28;
        ++i;

        if (i == 5)
        {
            Sexy::SharedImageRef checkBoxLine = mResourceManager->GetImage("IMAGE_DIALOG_CHECKBOXLINE");
            if (checkBoxLine.mSharedImage != NULL)
            {
                Sexy::MemoryImage *image = new MemoryImage(this);
                image->Create(20, checkBoxLine->mHeight);

                Sexy::Graphics g(image);

                for (int j = 0; j != 20; ++j)
                    g.DrawImage(checkBoxLine, j, 0);

                mResourceManager->ReplaceImage("IMAGE_DIALOG_CHECKBOXLINE", image);
            }

            Sexy::SharedImageRef ballShadow = mResourceManager->GetImage("IMAGE_BALL_SHADOW");
            if (ballShadow.mSharedImage != NULL)
            {
                Sexy::MemoryImage *image = new MemoryImage(this);
                image->Create(ballShadow->mWidth, ballShadow->mHeight);

                Sexy::Graphics g(image);
                g.SetColorizeImages(true);
                g.SetColor(Sexy::Color(0, 0, 0, 96));
                g.DrawImage(ballShadow, 0, 0);

                mResourceManager->ReplaceImage("IMAGE_BALL_SHADOW", image);
            }

            Sexy::Font *main8Font = mResourceManager->GetFont("FONT_MAIN8");
            if (main8Font != NULL)
            {
                ImageFont *imFont = reinterpret_cast<Sexy::ImageFont *>(main8Font->Duplicate());
                imFont->AddTag("Outline");
                mResourceManager->ReplaceFont("FONT_MAIN8OUTLINE", imFont);
            }

            int v10 = 1;
            while (Sexy::ExtractResourcesByName(mResourceManager, resourceGroups[v10]))
            {
                if (++v10 >= 5)
                {
                    ++mCompletedLoadingThreadTasks;
                    return;
                }
            }

            break;
        }
    }

    ShowResourceError();
    mLoadingFailed = true;
}

void CircleShootApp::LoadingThreadCompleted()
{
    // !PORT
    //if (ShouldCheckForUpdate())
    //{
    //    DoConfirmCheckForUpdatesDialog();
    //}
}

void CircleShootApp::FinishStatsDialog(bool confirm)
{
    KillDialog(DialogType_Stats);
    if (mBoard == NULL)
        return;

    if (!mBoard->IsGameOver())
    {
        mBoard->Pause(false, true);
    }
    else if (mBoard->IsPracticeMode())
    {
        ShowPracticeScreen(false);
    }
    else if (!mBoard->IsWinning())
    {
        ShowAdventureScreen(false, false);
    }
    else if (mBoard->GetCurrentStage() < 12)
    {
        ShowAdventureScreen(false, true);
    }
    else
    {
        ShowCreditsScreen(true);
    }
}

void CircleShootApp::FinishConfirmQuitDialog(bool confirm)
{
    KillDialog(DialogType_ConfirmQuit);

    if (confirm)
    {
        Shutdown();
    }
}

void CircleShootApp::DoStatsDialog(bool slide, bool doCounter)
{
    if (mBoard == NULL)
        return;

    mBoard->Pause(true, true);

    StatsDialog *aDialog = new StatsDialog(mBoard, doCounter);
    SetupDialog(aDialog, 460);
    AddDialog(DialogType_Stats, aDialog);

    if (slide)
    {
        mWidgetMover->MoveWidget(
            aDialog,
            aDialog->mX,
            aDialog->mHeight + 480,
            aDialog->mX,
            aDialog->mY,
            false);
    }
}

void CircleShootApp::DoNextTempleDialog()
{
    mDidNextTempleDialog = true;
    DoDialog(DialogType_NextTemple, true, LS(STRING_ID_ENTER_NEXT_TEMPLE), LS(STRING_ID_YOU_ARE_NOW_GOING_TO_ENTER_THE_NEXT_TEMPLE), "", Dialog::BUTTONS_OK_CANCEL);
}

void CircleShootApp::FinishNextTempleDialog(bool save)
{
    KillDialog(DialogType_NextTemple);

    if (save && CheckSaveGame(false))
    {
        StartSavedGame(true);
    }
    else if (mAdventureScreen)
    {
        mWidgetManager->SetFocus(mAdventureScreen);
    }
}

void CircleShootApp::DoUserDialog()
{
    KillDialog(DialogType_User);
    UserDialog *aDialog = new UserDialog();
    SetupDialog(aDialog, 400);
    AddDialog(DialogType_User, aDialog);
}

void CircleShootApp::DoCreateUserDialog()
{
    KillDialog(DialogType_CreateUser);
    CreateUserDialog *aDialog = new CreateUserDialog(false);
    SetupDialog(aDialog, 400);
    AddDialog(DialogType_CreateUser, aDialog);
}

void CircleShootApp::DoRenameUserDialog(const std::string &theName)
{
    KillDialog(DialogType_RenameUser);
    CreateUserDialog *aDialog = new CreateUserDialog(true);
    aDialog->SetName(theName);
    SetupDialog(aDialog, 400);
    AddDialog(DialogType_RenameUser, aDialog);
}

void CircleShootApp::DoConfirmDeleteUserDialog(const std::string &theName)
{
    KillDialog(DialogType_ConfirmDeleteUser);

    std::string aText = Sexy::StrFormat(LS(STRING_ID_THIS_WILL_PERMANENTLY_REMOVE_PLAYER_ROSTER), theName.c_str());
    DoDialog(DialogType_ConfirmDeleteUser, true, LS(STRING_ID_ARE_YOU_SURE_QM), aText, "", Dialog::BUTTONS_YES_NO);
}

void CircleShootApp::FinishUserDialog(bool confirm)
{
    UserDialog *aDialog = (UserDialog *)GetDialog(DialogType_User);
    if (!aDialog)
        return;

    if (confirm)
    {
        std::string aName = aDialog->GetSelName();
        UserProfile *aProfile = mProfileMgr->GetProfile(aName);

        if (aProfile)
        {
            mProfile = aProfile;
            mWidgetManager->MarkAllDirty();
            if (mMainMenu)
            {
                mMainMenu->SyncProfile();
            }
        }
    }

    KillDialog(DialogType_User);
}

void CircleShootApp::FinishCreateUserDialog(bool confirm)
{
    CreateUserDialog *aDialog = (CreateUserDialog *)GetDialog(DialogType_CreateUser);
    if (!aDialog)
        return;

    std::string aName = aDialog->GetName();
    if (confirm && aName.empty())
    {
        DoDialog(DialogType_NameEntry, true, 
                 LS(STRING_ID_ENTER_YOUR_NAME),
                 LS(STRING_ID_ENTER_YOUR_NAME_TO_CREATE_A_NEW_USER_PROFILE),
                 LS(DIALOG_BUTTON_OK),
                 Dialog::BUTTONS_FOOTER);
        return;
    }

    if (mProfile)
    {
        if (!confirm)
        {
            KillDialog(DialogType_CreateUser);
            return;
        }
    }
    else if (!confirm || aName.empty())
    {
        DoDialog(DialogType_NameEntry, true, 
                 LS(STRING_ID_ENTER_YOUR_NAME),
                 LS(STRING_ID_ENTER_YOUR_NAME_TO_CREATE_A_NEW_USER_PROFILE),
                 LS(DIALOG_BUTTON_OK),
                 Dialog::BUTTONS_FOOTER);
        return;
    }

    UserProfile *aProfile = mProfileMgr->AddProfile(aName);
    if (aProfile)
    {
        mProfileMgr->Save();
        mProfile = aProfile;
        KillDialog(DialogType_User);
        KillDialog(DialogType_CreateUser);
        mWidgetManager->MarkAllDirty();
        if (mMainMenu)
        {
            mMainMenu->SyncProfile();
        }
    }
    else
    {
        // yup, it's 10 here
        DoDialog(DialogType_NameEntry, true, 
                 LS(STRING_ID_NAME_CONFLICT),
                 LS(STRING_ID_THE_NAME_YOU_ENTERED_IS_ALREADY_BEING_USED),
                 LS(DIALOG_BUTTON_OK),
                 Dialog::BUTTONS_FOOTER);
    }
}

void CircleShootApp::FinishRenameUserDialog(bool confirm)
{
    if (!confirm)
    {
        KillDialog(DialogType_RenameUser);
        return;
    }

    UserDialog *aUserDialog = (UserDialog *)GetDialog(DialogType_User);
    CreateUserDialog *aCreateDialog = (CreateUserDialog *)GetDialog(DialogType_RenameUser);
    if (!aUserDialog || !aCreateDialog)
        return;

    std::string aSelName = aUserDialog->GetSelName();
    std::string aNewName = aCreateDialog->GetName();

    if (!aNewName.empty())
    {
        if (mProfileMgr->RenameProfile(aSelName, aNewName))
        {
            mProfileMgr->Save();
            if (strcasecmp(aSelName.c_str(), aNewName.c_str()) != 0)
            {
                mProfile = mProfileMgr->GetProfile(aNewName);
            }
            aUserDialog->FinishRenameUser(aNewName);
            mWidgetManager->MarkAllDirty();
            KillDialog(DialogType_RenameUser);
        }
        else
        {
            DoDialog(DialogType_NameConflict, true, 
                     LS(STRING_ID_NAME_CONFLICT),
                     LS(STRING_ID_THE_NAME_YOU_ENTERED_IS_ALREADY_BEING_USED),
                     LS(DIALOG_BUTTON_OK),
                     Dialog::BUTTONS_FOOTER);
        }
    }
}

void CircleShootApp::FinishConfirmDeleteUserDialog(bool confirm)
{
    KillDialog(DialogType_ConfirmDeleteUser);
    if (!confirm)
        return;

    UserDialog *aDialog = (UserDialog *)GetDialog(DialogType_User);
    if (!aDialog)
        return;

    std::string aCurProfileName = mProfile ? mProfile->mName : "";
    std::string aSelName = aDialog->GetSelName();

    if (aSelName == aCurProfileName)
    {
        mProfile = NULL;
    }

    mProfileMgr->DeleteProfile(aSelName);
    aDialog->FinishDeleteUser();

    if (!mProfile)
    {
        std::string aNewName = aDialog->GetSelName();
        mProfile = mProfileMgr->GetProfile(aNewName);
        if (!mProfile)
        {
            mProfile = mProfileMgr->GetAnyProfile();
        }
    }

    mProfileMgr->Save();

    if (!mProfile)
    {
        DoCreateUserDialog();
    }

    mWidgetManager->MarkAllDirty();

    if (mMainMenu)
    {
        mMainMenu->SyncProfile();
    }
}

void CircleShootApp::DoRegisterDialog()
{
}

void CircleShootApp::DoCheckForUpdatesDialog()
{
}

void CircleShootApp::DoConfirmMainMenuDialog()
{
    if (mBoard && mBoard->NeedSaveGame())
    {
        Dialog *aDialog = DoDialog(DialogType_ConfirmMainMenu, true, LS(STRING_ID_LEAVE_GAME_QM), LS(STRING_ID_DO_YOU_WANT_TO_CONTINUE_QM), "", Dialog::BUTTONS_OK_CANCEL);
        aDialog->mYesButton->mLabel = LS(STRING_ID_LEAVE);

        OptionsDialog *aDialogOptions = (OptionsDialog *)GetDialog(DialogType_Options);
        if (aDialogOptions)
        {
            aDialog->Resize(aDialog->mX, aDialogOptions->mY + aDialogOptions->mHeight - aDialog->mHeight, aDialog->mWidth, aDialog->mHeight);
        }
    }
    else
    {
        ShowMainMenu();
    }
}

void CircleShootApp::FinishUpdateDialogs(int theDialogId, bool confirm)
{
}

void CircleShootApp::DoConfirmQuitDialog()
{
    // HACK: black window left around after quitting android
    // disable this and just rely on the user swiping the app away instead
#if !defined(__ANDROID__)
    DoDialog(DialogType_ConfirmQuit, true, LS(STRING_ID_QUIT_ZUMA_QM), LS(STRING_ID_QUIT_THE_GAME_QM), "", Dialog::BUTTONS_YES_NO);
#endif
}

void CircleShootApp::SwitchSong(int id)
{
    if (this->mLastSong == id ||
        (SDL_GetTicks() - this->mLastSongSwitchTime) >= 5000)
    {
        PlaySong(id, true, 0.01);
    }
}

void CircleShootApp::DoOptionsDialog()
{
    if (mBoard != NULL)
    {
        mBoard->Pause(true, true);
    }

    Dialog *dialog = new OptionsDialog(mBoard != NULL);
    SetupDialog(dialog, 400);
    AddDialog(DialogType_Options, dialog);
}

void CircleShootApp::DoConfirmContinueDialog(const std::string &theVerboseLevelString, const std::string &theDisplayName, int theScore)
{
    std::string aScore = Sexy::StrFormat(LS(STRING_ID_SCORE), theVerboseLevelString.c_str(), theDisplayName.c_str(), theScore);
    std::string aText = Sexy::StrFormat("%s\r\n"
                        "\r\n"
                        "%s"
                        "\r\n"
                        "\r\n", 
                        LS(STRING_ID_YOUR_GAME_WAS_SAVED_WHEN_YOU_QUIT),
                        LS(STRING_ID_DO_YOU_WANT_TO_CONTINUE_YOUR_LAST_GAME_QM)) +
                        aScore;

    Dialog *aDialog = NewDialog(DialogType_ConfirmContinue, true, LS(STRING_ID_CONTINUE_QM), aText, "", Dialog::BUTTONS_OK_CANCEL);

    aDialog->mYesButton->mLabel = LS(STRING_ID_SM_CONTINUE);
    aDialog->mNoButton->mLabel = LS(STRING_ID_NEW_GAME);

    AddDialog(DialogType_ConfirmContinue, aDialog);
}

void CircleShootApp::DoGetReadyDialog()
{
    if (mDialogMap.empty())
    {
        DoDialog(DialogType_GetReady, true, LS(STRING_ID_GET_READY), "", LS(STRING_ID_GO), Dialog::BUTTONS_FOOTER);
        if (mBoard)
        {
            mBoard->Pause(true, true);
        }

        mBoard->SetShowBallsDuringPause(true);
        mBoard->SetFullPauseFade();
    }
}

void CircleShootApp::PlaySong(int id, bool fade, double fadeSpeed)
{
    if (this->mLastSong != id)
    {
        int prevSong = mSongId;

        if (fade)
        {
            mSongId = (mSongId + 1) % 2;
            mMusicInterface->FadeOut(prevSong, true, fadeSpeed);
            mMusicInterface->FadeIn(mSongId, id, fadeSpeed * 0.5);
        }
        else
        {
            mMusicInterface->PlayMusic(mSongId, id);
        }

        mLastSong = id;
        mLastSongSwitchTime = SDL_GetTicks();
    }
}

void CircleShootApp::FinishGetReadyDialog()
{
    KillDialog(DialogType_GetReady);
    if (mBoard)
    {
        mBoard->Pause(false, true);
    }
}

void CircleShootApp::FinishConfirmContinueDialog(bool startGame)
{
    KillDialog(DialogType_ConfirmContinue);

    std::string aSaveGameName = GetSaveGameName(mIsPractice, mProfile->mId);
    EraseFile(aSaveGameName);

    if (startGame)
    {
        StartSavedGame(false);
    }
    else if (mIsPractice)
    {
        ShowPracticeScreen(false);
    }
    else
    {
        ShowAdventureScreen(false, false);
    }
}

void CircleShootApp::FinishOptionsDialog(bool saveSettings)
{
    Sexy::OptionsDialog *dialog = (OptionsDialog *)GetDialog(DialogType_Options);
    if (dialog != NULL)
    {
        if (saveSettings)
        {
            bool fullscreen = dialog->mFullScreenCheckbox->IsChecked();
            bool acceleration = dialog->m3DAccelCheckbox->IsChecked();
            SwitchScreenMode(fullscreen != true, acceleration);
            bool cursorsEnabled = dialog->mCustomCursorsCheckbox->IsChecked();
            EnableCustomCursors(cursorsEnabled);
            ClearUpdateBacklog();
        }

        KillDialog(0);
        if (mBoard)
        {
            mBoard->Pause(false, true);
        }
    }
}

void CircleShootApp::FinishConfirmMainMenuDialog(bool mainMenu)
{
    KillDialog(DialogType_ConfirmMainMenu);
    if (mainMenu)
    {
        ShowMainMenu();
    }
}

bool CircleShootApp::CheckYesNoButton(int theButton)
{
    if ((theButton % 10000 - 2000) < 1000)
    {
        switch (theButton)
        {
        case 2000:
            FinishOptionsDialog(true);
            return true;
        case 2001:
            // DoCheckForUpdatesDialog();
            return true;
        case 2002:
        case 12002:
        case 22002:
            // FinishUpdateDialogs(theButton - 2000, true);
            return true;
        case 2007:
            FinishUserDialog(true);
            return true;
        case 2008:
            FinishCreateUserDialog(true);
            return true;
        case 2009:
            FinishRenameUserDialog(true);
            return true;
        case 2012:
            FinishConfirmDeleteUserDialog(true);
            return true;
        case 2013:
            FinishConfirmContinueDialog(true);
            return true;
        case 2014:
            FinishStatsDialog(true);
            return true;
        case 2015:
            FinishGetReadyDialog();
            return true;
        case 2016:
            FinishNextTempleDialog(true);
            return true;
        case 2017:
            // FinishRegisterDialog(true);
            return true;
        case 2020:
            FinishConfirmMainMenuDialog(true);
            return true;
        case 2021:
            FinishConfirmQuitDialog(true);
            return true;
        case 2022:
            // FinishNeedRegisterDialog(true);
            return true;
        default:
            KillDialog(theButton - 2000);
            return true;
        }
    }
    else if ((theButton % 10000 - 3000) < 1000)
    {
        switch (theButton)
        {
        case 3002:
        case 13002:
        case 23002:
            // FinishUpdateDialogs(theButton - 3000, true);
            return true;
        case 3007:
            FinishUserDialog(false);
            return true;
        case 3008:
            FinishCreateUserDialog(false);
            return true;
        case 3009:
            FinishRenameUserDialog(false);
            return true;
        case 3012:
            FinishConfirmDeleteUserDialog(false);
            return true;
        case 3013:
            FinishConfirmContinueDialog(false);
            return true;
        case 3014:
            FinishStatsDialog(false);
            return true;
        case 3016:
            FinishNextTempleDialog(false);
            return true;
        case 3017:
            // FinishRegisterDialog(false);
            return true;
        case 3020:
            FinishConfirmMainMenuDialog(false);
            return true;
        case 3021:
            FinishConfirmQuitDialog(false);
            return true;
        case 3022:
            // FinishNeedRegisterDialog(false);
            return true;
        default:
            KillDialog(theButton - 3000);
            return true;
        }
    }

    return false;
}

void CircleShootApp::ShowMainMenu()
{
    if (mPracticeScreen != NULL)
    {
        mWidgetMover->MoveWidget(mPracticeScreen, mPracticeScreen->mX, mPracticeScreen->mY, -mPracticeScreen->mWidth, mPracticeScreen->mY, true);
        mPracticeScreen = NULL;
    }

    if (mAdventureScreen != NULL)
    {
        mWidgetMover->MoveWidget(mAdventureScreen, mAdventureScreen->mX, mAdventureScreen->mY, -mAdventureScreen->mWidth, mAdventureScreen->mY, true);
        mAdventureScreen = NULL;
    }

    if (mMoreGamesScreen != NULL)
    {
        mWidgetMover->MoveWidget(mMoreGamesScreen, mMoreGamesScreen->mX, mMoreGamesScreen->mY, -mMoreGamesScreen->mWidth, mMoreGamesScreen->mY, true);
        mMoreGamesScreen = NULL;
    }

    FinishOptionsDialog(true);
    CleanupWidgets();

    mMainMenu = new MainMenu();
    mMainMenu->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mMainMenu);
    mWidgetManager->BringToBack(mMainMenu);
    mWidgetManager->SetFocus(mMainMenu);

    if (!mProfile)
    {
        DoCreateUserDialog();
    }

    PlaySong(28, true, 0.01);
    ClearUpdateBacklog();
}

void CircleShootApp::ShowAdventureScreen(bool fromMenu, bool revealTemple)
{
    mIsPractice = false;
    if (fromMenu && CheckSaveGame(true))
        return;

    MainMenu *aMainMenu = mMainMenu;
    mWidgetMover->DelayDeleteWidget(mMainMenu);
    mMainMenu = NULL;

    CleanupWidgets();

    mAdventureScreen = new AdventureScreen();
    mAdventureScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mAdventureScreen);
    mWidgetManager->SetFocus(mAdventureScreen);

    if (revealTemple)
    {
        mAdventureScreen->RevealTemple(75, mProfile->mMaxStage / 3 + 1);
        mAdventureScreen->SetStartNextTempleOnRevel(true);
    }

    if (aMainMenu)
    {
        mWidgetMover->MoveWidget(mAdventureScreen, -mAdventureScreen->mWidth, 0, 0, 0, false);
    }

    PlaySong(32, true, 0.01);
    ClearUpdateBacklog();
}

void CircleShootApp::ShowPracticeScreen(bool fromMenu)
{
    mIsPractice = true;
    if (fromMenu && CheckSaveGame(true))
        return;

    MainMenu *aMainMenu = mMainMenu;
    mWidgetMover->DelayDeleteWidget(mMainMenu);
    mMainMenu = NULL;

    CleanupWidgets();

    mPracticeScreen = new PracticeScreen();
    mPracticeScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mPracticeScreen);
    mWidgetManager->SetFocus(mPracticeScreen);

    if (aMainMenu)
    {
        mWidgetMover->MoveWidget(mPracticeScreen, -mPracticeScreen->mWidth, 0, 0, 0, false);
    }

    PlaySong(34, true, 0.01);
    ClearUpdateBacklog();
}

void CircleShootApp::ShowCreditsScreen(bool happyEnd)
{
    CleanupWidgets();
    mCreditsScreen = new CreditsScreen(happyEnd);
    mCreditsScreen->Resize(0, 0, mWidth, mHeight);
    mWidgetManager->AddWidget(mCreditsScreen);
    mWidgetManager->SetFocus(mCreditsScreen);
    PlaySong(0, true, 0.01);
}

void CircleShootApp::ShowMoreGamesScreen()
{
}

void CircleShootApp::EndHelpScreen()
{
    if (mHelpScreen)
    {
        mWidgetMover->SafeDeleteWidget(mHelpScreen);
        mHelpScreen = NULL;
    }

    if (mBoard)
    {
        mWidgetManager->SetFocus(mBoard);

        if (mBoard)
        {
            mBoard->Pause(false, true);
        }

        if (mBoard->IsSavedGame())
        {
            DoGetReadyDialog();
        }
    }
}

void CircleShootApp::ReturnToMainMenu()
{
    DoConfirmMainMenuDialog();
}
bool CircleShootApp::ChangeDirHook(const char *theIntendedPath)
{
	return true;
}
void CircleShootApp::CloseRequestAsync()
{
	Shutdown();
}
SDL_Point CircleShootApp::Translate(int x, int y)
{
    const Rect &r = mGLInterface->mPresentationRect;
    SDL_Point result = {};
    result.x = ((float)x / mWidth) * r.mWidth + r.mX;
    result.y = ((float)y / mHeight) * r.mHeight + r.mY;
    return result;
}
void CircleShootApp::MoveToControllerWidget()
{
    static int lastx, lasty;
    int x = mControllerWidget->mX + mControllerWidget->mWidth / 2.0f;
    int y = mControllerWidget->mY + mControllerWidget->mHeight / 2.0f;
    if (lastx != x || lasty != y)
    {
        lastx = x;
        lasty = y;
        SDL_Point pt = {};

        // do mouse leave on widget
        pt = Translate(0, 0);
        SDL_Event ev = {};
        ev.motion.x = pt.x;
        ev.motion.y = pt.y;
        ev.type = SDL_MOUSEMOTION;
        SDL_PushEvent(&ev);

        // do mouse enter on widget
        pt = Translate(x, y);
        ev.type = SDL_MOUSEMOTION;
        ev.motion.x = pt.x;
        ev.motion.y = pt.y;
        SDL_PushEvent(&ev);

        // make sure window draws the moved cursor
        mWidgetManager->MarkAllDirty();
    }
}
void CircleShootApp::DrawAboveWidgets(Graphics *g)
{
    if (mController)
    {
        // clear controller widget if it's no longer clickable
        bool found = false;
        int aFlags = mWidgetManager->GetWidgetFlags();
        Rect aRect = { 0, 0, mWidth, mHeight };
        auto widgets = GetClickableWidgets(aRect, aFlags);
        for (auto widget : widgets)
        {
            if (mControllerWidget == widget)
            {
                found = true;
            }
        }

        if (!found)
        {
            if (widgets.size())
            {
                bool handled = false;
                if (mPracticeScreen)
                {
                    Widget *pb = mPracticeScreen->mPracticeButton;
                    if (mControllerWidget == mPracticeScreen->mSurvivalButton)
                    {
                        handled = true;
                        mControllerWidget = mPracticeScreen->mPracticeButton;
                    }
                    else if (mControllerWidget == mPracticeScreen->mPracticeButton)
                    {
                        handled = true;
                        mControllerWidget = mPracticeScreen->mSurvivalButton;
                    }
                }

                if (!handled)
                {
                    SDL_Event ev = {};
                    ev.type = SDL_CONTROLLERBUTTONDOWN;
                    ev.cbutton.button = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
                    SDL_PushEvent(&ev);
                    mControllerWidget = NULL;
                }
            }
            else
            {
                mControllerWidget = NULL;
            }
        }

        // draw image above all widgets
        if (mControllerWidget)
        {
            MoveToControllerWidget();
            g->Translate(mControllerWidget->mX, mControllerWidget->mY);
            Image *aImage = gSexyAppBase->mCursorImages[CURSOR_HAND];
            g->DrawImage(aImage, 0, 0);
        }
    }
}
std::vector<Widget *> CircleShootApp::GetClickableWidgets(Rect theRect, int theFlags)
{
    // prevent board menu button from being clickable
    std::vector<Widget *> result;
    auto widgets = mWidgetManager->GetClickableWidgets(theRect, theFlags);
    for (auto widget : widgets)
    {
        if (!mBoard || widget != mBoard->mMenuButton)
        {
            result.push_back(widget);
        }
    }
    return result;
}
Widget *CircleShootApp::Move(Widget *start, Direction dir)
{
    Widget *result = NULL;

    struct Cell
    {   
        Widget *widget = NULL;
        bool found = false;
        Cell(Widget *w) { widget = w; }
        Cell() = default;
    };
    const int NX = 9;
    const int NY = 7;
    struct CellGrid
    {
        Cell cells[NY][NX];
    };
    CellGrid grid = {};

    struct CellPos
    {
        int x;
        int y;
    };
    CellPos default_pos = {};

    const auto GetCellPos = [&](Widget *w) -> CellPos
    {
        CellPos result = { -1, -1 };
        for (int y = 0; y < NY; y++)
        {
            for (int x = 0; x < NX; x++)
            {
                if (grid.cells[y][x].widget == w && w != NULL)
                {
                    result.x = x;
                    result.y = y;
                    return result;
                }
            }
        }
        return result;
    };
    const auto GetCell = [&](CellPos pos) -> Cell *
    {
        Cell *result = NULL;
        if (pos.x >= 0 && pos.x < NX &&
            pos.y >= 0 && pos.y < NY)
        {
            result = &grid.cells[pos.y][pos.x];
        }
        return result;
    };
    
    int aFlags = mWidgetManager->GetWidgetFlags();
    Rect aRect = { 0, 0, mWidth, mHeight };
    auto widgets = GetClickableWidgets(aRect, aFlags);

    const auto HasWidget = [&](Widget *w)
    {
        bool result = false;
        for (Widget *iter : widgets)
        {
            if (iter == w)
            {
                result = true;
            }
        }
        return result;
    };

    OptionsDialog *options = (OptionsDialog *)GetDialog(DialogType_Options);
    UserDialog *user = (UserDialog *)GetDialog(DialogType_User);
    if (mPracticeScreen)
    {
        Widget *nb = mPracticeScreen->mNextButton;
        Widget *bb = mPracticeScreen->mBackButton;
        Widget *gb = mPracticeScreen->mGauntPlayButton;
        Widget *mmb = mPracticeScreen->mMainMenuButton;
        Widget *pb = mPracticeScreen->mPracticeButton;
        Widget *sb = mPracticeScreen->mSurvivalButton;
        Widget *sb0 = mPracticeScreen->mDoorInfo[0].mSpoof;
        Widget *sb1 = mPracticeScreen->mDoorInfo[1].mSpoof;
        Widget *sb2 = mPracticeScreen->mDoorInfo[2].mSpoof;
        Widget *sb3 = mPracticeScreen->mDoorInfo[3].mSpoof;
        
        //              SUNGOD
        //              JAGUAR
        //              EAGLE
        //              RABBIT
        //                              SURVIVAL
        // MM   BACK    PLAY    NEXT    PRACTICE
        grid = {
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL,  sb3, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL,  sb2, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL,  sb1, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL,  sb0, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL,   sb, NULL, NULL, NULL, NULL,
             mmb,   bb,   gb,   nb,   pb, NULL, NULL, NULL, NULL,
        };
        default_pos = GetCellPos(mmb);
    }
    else if (user && HasWidget(user->mRenameButton))
    {
        Widget *renb = user->mRenameButton;
        Widget *delb = user->mDeleteButton;
        Widget *okkb = user->mYesButton;
        Widget *cnlb = user->mNoButton;
        
        grid = {
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            renb, delb, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            okkb, cnlb, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        };
        default_pos = GetCellPos(renb);
    }
    else if (options && HasWidget(options->mFullScreenCheckbox))
    {
        // MUSIC
        // SOUND
        // FULL     CURSOR
        // 3D       3D
        // DONE     DONE     
        //Widget *fb = options->mMusicVolumeSlider;
        //Widget *fb = options->mSfxVolumeSlider;
        Widget *musc = options->mSpoofMusic;
        Widget *sond = options->mSpoofSound;
        Widget *full = options->mFullScreenCheckbox;
        Widget *curs = options->mCustomCursorsCheckbox;
        Widget *accl = options->m3DAccelCheckbox;
        Widget *bhlp = options->mButtonHelp;
        Widget *main = options->mButtonBack;
        Widget *game = options->mYesButton;
        
        double delta = (dir == Direction_Left) ? -0.1 :
                       (dir == Direction_Right) ? 0.1 : 0;
        if (start == sond && delta != 0.0)
        {
            double val = std::clamp(gSexyAppBase->GetSfxVolume() + delta, 0.0, 1.0);
            gSexyAppBase->SetSfxVolume(val);
            options->mSfxVolumeSlider->SetValue(val);
            return start;
        }
        if (start == musc && delta != 0.0)
        {
            double val = std::clamp(gSexyAppBase->GetMusicVolume() + delta, 0.0, 1.0);
            gSexyAppBase->SetMusicVolume(val);
            options->mMusicVolumeSlider->SetValue(val);
            return start;
        }

        if (!HasWidget(game)) game = NULL;
        if (!HasWidget(main)) main = NULL;

        grid = {
            musc, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            sond, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            full, curs, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            accl, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            bhlp, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            main, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            game, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        };
        default_pos = GetCellPos((game) ? game : main);
    }
    else if (mAdventureScreen)
    {
        Widget *main = mAdventureScreen->mMainMenuButton;
        Widget *play = mAdventureScreen->mPlayButton;
        Widget *ent0 = mAdventureScreen->mDoorInfo[0].mSpoof;
        Widget *ent1 = mAdventureScreen->mDoorInfo[1].mSpoof;
        Widget *ent2 = mAdventureScreen->mDoorInfo[2].mSpoof;
        Widget *ent3 = mAdventureScreen->mDoorInfo[3].mSpoof;
        Widget *ent4 = mAdventureScreen->mDoorInfo[4].mSpoof;
        Widget *ent5 = mAdventureScreen->mDoorInfo[5].mSpoof;
        Widget *ent6 = mAdventureScreen->mDoorInfo[6].mSpoof;
        Widget *ent7 = mAdventureScreen->mDoorInfo[7].mSpoof;
        Widget *ent8 = mAdventureScreen->mDoorInfo[8].mSpoof;
        Widget *ent9 = mAdventureScreen->mDoorInfo[9].mSpoof;
        Widget *ent10 = mAdventureScreen->mDoorInfo[10].mSpoof;
        Widget *ent11 = mAdventureScreen->mDoorInfo[11].mSpoof;

        if (start == main && dir == Direction_Right)
        {
            ent3 = ent4 = ent5 = NULL;
        }
        else if (start == play && dir == Direction_Left)
        {
            ent6 = ent7 = ent8 = NULL;
        }

        grid = {
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, ent9,ent10,ent11, NULL, NULL, NULL,
            ent3, ent4, ent5, NULL, NULL, NULL, ent6, ent7, ent8,
            NULL, NULL, NULL, ent0, ent1, ent2, NULL, NULL, NULL,
            main, NULL, NULL, NULL, NULL, NULL, NULL, NULL, play,
        };
        default_pos = GetCellPos(main);
    }
    else if (mMainMenu && HasWidget(mMainMenu->mNotYouLink))
    {
        Widget *noty = mMainMenu->mNotYouLink;
        Widget *play = mMainMenu->mArcadeButton;
        Widget *gaun = mMainMenu->mGauntletButton;
        Widget *opts = mMainMenu->mOptionsButton;
        Widget *more = mMainMenu->mMoreGamesButton;
        Widget *quit = mMainMenu->mQuitButton;

        grid = {
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            noty, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, play, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, gaun, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, opts, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, quit, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        };
        default_pos = GetCellPos(play);
    }
    else
    {
        const auto Comparator = [](const Widget *a, const Widget *b) -> bool
        {
            return a->mX < b->mX;
        };
        
        std::sort(widgets.begin(), widgets.end(), Comparator);
        
        size_t i = 0;
        for (int x = 0; x < NX; x++)
        {
            if (i < widgets.size())
            {
                grid.cells[0][x].widget = widgets[i];
                i += 1;
            }
        }

        if (widgets.size())
        {
            default_pos = GetCellPos(widgets.front());
        }
    }

    // find 
    CellPos iter = GetCellPos(start);
    if (iter.x != -1 && iter.y != -1)
    {
        for (int num = 0; num < 9; num++)
        {
            if (dir == Direction_Up)
                iter.y -= 1;
            else if (dir == Direction_Down)
                iter.y += 1;
            else if (dir == Direction_Left)
                iter.x -= 1;
            else if (dir == Direction_Right)
                iter.x += 1;

            for (int j = 0; j < 9; j++)
            {
                int jj[] = { j, -j };
                for (int k : jj)
                {
                    CellPos iterk = iter;
                    if (dir == Direction_Up || dir == Direction_Down)
                        iterk.x += k;
                    else if (dir == Direction_Left || dir == Direction_Right)
                        iterk.y += k;
                    
                    Cell *next = GetCell(iterk);
                    if (next && next->widget && !next->widget->mDisabled)
                    {
                        result = next->widget;
                        break;
                    }
                }
                if (result) break;
            }
            if (result) break;
        }
    }
    else
    {
        Cell *next = GetCell(default_pos);
        if (next)
        {
            result = next->widget;
        }
    }
    
    return result;
}
void CircleShootApp::HandleEvent(SDL_Event *ev)
{
    static int i = -1;
    static int x = 0;
    static int y = 0;
    
    if (ev->type == SDL_CONTROLLERDEVICEADDED)
    {
        if (mController == NULL)
        {
            SDL_GameController *gc = SDL_GameControllerOpen(ev->cdevice.which);
            SDL_Log("SDL_CONTROLLERDEVICEADDED SDL_GameControllerOpen %d: %s",
                    ev->cdevice.which, (gc == NULL) ? SDL_GetError() : "Success");
            if (gc)
            {
                mController = gc;
                mControllerIndex = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gc));
            }
        }
    }
    else if (ev->type == SDL_CONTROLLERDEVICEREMOVED)
    {
        SDL_Log("SDL_CONTROLLERDEVICEREMOVED %d REMOVED %d", ev->cdevice.which, mControllerIndex);
        if (ev->cdevice.which == mControllerIndex)
        {
            SDL_GameControllerClose(mController);
            mController = NULL;
            mControllerIndex = -1;
            mControllerWidget = NULL;
        }
    }
    
    if (mBoard &&
        mBoard->mPauseCount == 0 &&
        mBoard->mDialogCount == 0 &&
        mBoard->mTransitionMgr->mState != TransitionState_TempleComplete)
    {
        if (ev->type == SDL_CONTROLLERAXISMOTION)
        {
            Sint16 x = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_LEFTX);
            Sint16 y = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_LEFTY);
            const int JOYSTICK_DEAD_ZONE = 8000;
            float dist = sqrtf(y*y + x*x);
            if (dist > JOYSTICK_DEAD_ZONE)
            {
                float angle = atan2f(-y, x) + (SEXY_PI / 2.0f);
                mBoard->mGun->SetAngle(angle);
                mBoard->DoAccuracy(mBoard->mAccuracyCount > 0);
                mBoard->mRecalcGuide = true;
            }
        }
        else if (ev->type == SDL_CONTROLLERBUTTONDOWN)
        {
            if (ev->cbutton.button == SDL_CONTROLLER_BUTTON_START)
            {
                if (mBoard->mPauseCount == 0)
                {
                    DoOptionsDialog();
                }
            }
            else if (ev->cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                if (mBoard->mTransitionMgr->mState == TransitionState_LevelBegin)
                {
                    mBoard->mSoundMgr->KillAllSounds();
                    mBoard->StartLevel();
                }
                else
                {
                    if (mBoard->mGameState == GameState_Playing)
                    {
                        bool aCanFire = true;
                        for (int i = 0; i < mBoard->mNumCurves; i++)
                        {
                            if (!mBoard->mCurveMgr[i]->CanFire())
                                aCanFire = false;
                        }

                        if (aCanFire)
                        {
                            if (mBoard->mGun->StartFire())
                            {
                                PlaySample(Sexy::SOUND_BALLFIRE);
                            }
                        }
                    }
                }
            }
            else if (ev->cbutton.button == SDL_CONTROLLER_BUTTON_B)
            {
                mBoard->mGun->SwapBullets();
            }
            else if (ev->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN ||
                     ev->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
            {
                mBoard->mGun->SetAngle(mBoard->mGun->GetAngle() + SEXY_PI);
                mBoard->DoAccuracy(mBoard->mAccuracyCount > 0);
                mBoard->mRecalcGuide = true;
            }
        }
    }
    else
    {
        bool aTempleCompleteClickable = false;
        if (mBoard &&
            mBoard->mTransitionMgr->mState == TransitionState_TempleComplete &&
            mBoard->mTransitionMgr->mStateCount < mBoard->mTransitionMgr->mResetFrame)
        {
            aTempleCompleteClickable = true;
        }
        
        if (ev->type == SDL_CONTROLLERBUTTONDOWN)
        {
            Direction dir = (ev->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) ? Direction_Left :
                            (ev->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ? Direction_Right :
                            (ev->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) ? Direction_Up :
                            (ev->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) ? Direction_Down : Direction_Invalid;

            if (dir != Direction_Invalid)
            {
                Widget *next = CircleShootApp::Move(mControllerWidget, dir);
                if (next)
                {
                    mControllerWidget = next;
                    MoveToControllerWidget();
                }
            }
            else if (ev->cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                if (aTempleCompleteClickable)
                {
                    mBoard->mTransitionMgr->FinishAllTextBlurbs();
                    mBoard->mTransitionMgr->mResetFrame = mBoard->mTransitionMgr->mStateCount;
                }
                else if (mControllerWidget)
                {
                    int x = mControllerWidget->mX + mControllerWidget->mWidth / 2.0f;
                    int y = mControllerWidget->mY + mControllerWidget->mHeight / 2.0f;
                    SDL_Point pt = Translate(x, y);

                    SDL_Event ev = {};
                    ev.type = SDL_MOUSEBUTTONDOWN;
                    ev.button.x = pt.x;
                    ev.button.y = pt.y;
                    SDL_PushEvent(&ev);

                    ev.type = SDL_MOUSEBUTTONUP;
                    ev.button.x = pt.x;
                    ev.button.y = pt.y;
                    SDL_PushEvent(&ev);
                }
            }
        }
    }
}
