#include "Zuma_Prefix.pch"

#include <SexyAppFramework/WidgetManager.h>
#include <SexyAppFramework/Font.h>
#include <SexyAppFramework/Graphics.h>

#include "Board.h"
#include "CircleButton.h"
#include "CircleCommon.h"
#include "CircleDialog.h"
#include "CircleShootApp.h"
#include "HighScoreMgr.h"
#include "StatsDialog.h"
#include "SoundMgr.h"
#include "Res.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

char* itoa(int value, char* buffer, int radix)
{
    if (radix < 2 || radix > 36)
    {
        buffer[0] = '\0';
        return buffer;
    }

    char* ptr = buffer;
    char* ptr1 = buffer;
    char tmp_char;
    int tmp_value;

    bool negative = false;

    if (value < 0 && radix == 10)
    {
        negative = true;
        value = -value;
    }

    do
    {
        tmp_value = value;
        value /= radix;
        int digit = tmp_value - value * radix;

        *ptr++ = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
    }
    while (value);

    if (negative)
        *ptr++ = '-';

    *ptr-- = '\0';

    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return buffer;
}

StatsDialog::StatsDialog(Board *theBoard, bool doCounter) : CircleDialog(Sexy::IMAGE_DIALOG_BACK, Sexy::IMAGE_DIALOG_BUTTON, DialogType_Stats, true,
                                                                         LS(STRING_ID_STATS), "", LS(DIALOG_BUTTON_OK),
                                                                         Dialog::BUTTONS_FOOTER, true)
{
    const char *aNextStageTexts[12];
    const char *aGameOverTexts[8];

    aNextStageTexts[0] = LS(STRING_ID_FILL_THE_ORANGE_GAUGE);
    aNextStageTexts[1] = LS(STRING_ID_SCORE_POINTS_TO_FILL_THE_ORANGE_GAUGE);
    aNextStageTexts[2] = LS(STRING_ID_CLICK_THE_RIGHT_MOUSE_BUTTON_TO_SWAP);
    aNextStageTexts[3] = LS(STRING_ID_SHOOT_BALLS_THROUGH_A_GAP_TO_SCORE);
    aNextStageTexts[4] = LS(STRING_ID_CREATE_COMBOS_OF_2_OR_MORE_EXPLOSIONS);
    aNextStageTexts[5] = LS(STRING_ID_CLOSE_GAPS_BY_PLACING_BALLS_OF_THE_SAME_COLOR);
    aNextStageTexts[6] = LS(STRING_ID_GET_A_COIN_FOR_A_BIG_POINT_BOOST_TO_FINISH);
    aNextStageTexts[7] = LS(STRING_ID_FINISH_A_LEVEL_QUICKLY);
    aNextStageTexts[8] = LS(STRING_ID_GET_AN_EXTRA_FROG);
    aNextStageTexts[9] = LS(STRING_ID_SHOOT_BALLS_THROUGH_TWO_OR_MORE_GAPS);
    aNextStageTexts[10] = LS(STRING_ID_PRESS_SPACE_BAR_TO_PAUSE);
    aNextStageTexts[11] = LS(STRING_ID_PRESS_T_TO_SHOW_TIME);
    aGameOverTexts[0] = LS(STRING_ID_ONCE_YOU_CAN_ACCEPT_THE_UNIVERSE);
    aGameOverTexts[1] = LS(STRING_ID_THERE_IS_ONLY_ONE_CORNER_OF_THE_UNIVERSE);
    aGameOverTexts[2] = LS(STRING_ID_IT_IS_ALWAYS_WISE_TO_LOOK_AHEAD);
    aGameOverTexts[3] = LS(STRING_ID_GOOD_TIMBER_DOES_NOT_GROW_WITH_EASE);
    aGameOverTexts[4] = LS(STRING_ID_THE_FACT_THAT_NO_ONE_UNDERSTANDS);
    aGameOverTexts[5] = LS(STRING_ID_HONEST_CRITICISM_IS_HARD_TO_TAKE);
    aGameOverTexts[6] = LS(STRING_ID_A_GOOD_PLAN_TODAY_IS_BETTER);
    aGameOverTexts[7] = LS(STRING_ID_GOOD_LUCK_IS_OFTEN_WITH_THE_MAN);

    mBoard = theBoard;
    mTimeBonus = 0;
    mTargetTimeBonus = 0;
    mIsGameOver = theBoard->IsGameOver();
    mIsWinning = theBoard->IsWinning();

    if (mIsGameOver)
    {
        mScore = mBoard->mScore;
        if (mIsWinning)
        {
            mDialogHeader = LS(STRING_ID_WINNING_TOTALS);
            mTargetTimeBonus = 50000 * mBoard->mLives;
        }
        else
        {
            mDialogHeader = LS(STRING_ID_GAME_OVER);
        }

        mCurrentTip = aGameOverTexts[rand() % 8];
    }
    else
    {
        mScore = mBoard->mScore - mBoard->mLevelBeginScore;
        if (mBoard->mPracticeBoard.empty() && mBoard->mNextLevelDesc->mInSpace)
        {
            mCurrentTip = LS(STRING_ID_PRESS_THE_S_KEY);
        }
        else
        {
            mCurrentTip = aNextStageTexts[mBoard->mLevel % 12];
        }

        mTargetTimeBonus = mBoard->GetParBonus(mBoard->mLevelStats.mTimePlayed / 100, mBoard->mLevelDesc->mParTime);
        LowTime *aLowTime = mBoard->mApp->mHighScoreMgr->GetLowTime(mBoard->GetUniqueLevelString());
        if (aLowTime != NULL)
        {
            mBestTimeLeft = LS(STRING_ID_BEST_TIME) + Sexy::GetTimeString(aLowTime->mTime);
            mBestTimeRight = aLowTime->mName;
        }
    }

    if (!doCounter)
    {
        mTimeBonus = mTargetTimeBonus;
    }

    mHighScoreChecked = false;
}

StatsDialog::~StatsDialog()
{
}

void StatsDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
    Dialog::Resize(theX, theY, theWidth, theHeight);
}

int StatsDialog::GetPreferredHeight(int theWidth)
{
    return ((!mIsWinning && mIsGameOver) ? 140 : 180) + CircleDialog::GetPreferredHeight(theWidth);
}

void StatsDialog::AddedToManager(WidgetManager *theWidgetManager)
{
    Dialog::AddedToManager(theWidgetManager);
}

void StatsDialog::RemovedFromManager(WidgetManager *theWidgetManager)
{
    Dialog::RemovedFromManager(theWidgetManager);
}

void StatsDialog::Update()
{
    Widget::Update();

    if (mTimeBonus < mTargetTimeBonus)
    {
        if (mUpdateCnt > 100)
        {
            if (mUpdateCnt % 10 == 1)
            {
                mBoard->mSoundMgr->AddSound(
                    Sexy::SOUND_BONUS_EXPLOSION, 0, 0,
                    (float)mTimeBonus / (mIsWinning ? 25000.0f : 1000.0f));
            }

            int aOldTimeBonus = mTimeBonus;
            mTimeBonus = (!mIsWinning ? -2400 : 0) + mTimeBonus + 2500;
            if (mTimeBonus > mTargetTimeBonus)
            {
                mTimeBonus = mTargetTimeBonus;
            }

            mBoard->IncScore(mTimeBonus - aOldTimeBonus, false);

            if (mIsGameOver)
            {
                mScore = mBoard->mScore;
            }
            else
            {
                mScore = mBoard->mScore - mBoard->mLevelBeginScore;
            }

            MarkDirty();
        }
    }
}

void StatsDialog::ButtonDepress(int theId)
{
    if (mTimeBonus < mTargetTimeBonus)
    {
        mBoard->IncScore(mTargetTimeBonus - mTimeBonus, false);
        mTimeBonus = mTargetTimeBonus;
    }

    if (!mHighScoreChecked && mIsWinning)
    {
        mBoard->CheckHighScore();
        mHighScoreChecked = true;
    }

    Dialog::ButtonDepress(theId);
}

void StatsDialog::Draw(Graphics *g)
{
    char buf[50];

    CircleDialog::Draw(g);

    g->SetFont(Sexy::FONT_MAIN12);
    int aDialogTop = GetTop() - mY + 10;
    int aDialogLeft = GetLeft() - mX;

    int aCol1 = aDialogLeft + 10;
    int aCol2 = aDialogLeft + 140;
    int aCol3 = aDialogLeft + 220;
    int aCol4 = aDialogLeft + 320;
    int aLineY;

    // big fish games localization uses much larger GetLineSpacing but same font
    //int aLineOffset = 6 + Sexy::FONT_MAIN12->GetLineSpacing();
    int aLineOffset = 19;
    if (!mIsGameOver)
    {
        g->SetColor(Color(0xFFFF00));
        g->DrawString(LS(STRING_ID_POINTS), aCol1, aDialogTop);
        g->DrawString(LS(STRING_ID_COMBOS), aCol1, aDialogTop + aLineOffset);
        g->DrawString(LS(STRING_ID_COINS), aCol1, aDialogTop + 2 * aLineOffset);
        g->DrawString(LS(STRING_ID_GAPS), aCol1, aDialogTop + 3 * aLineOffset);
        g->DrawString(LS(STRING_ID_MAX_CHAIN), aCol1, aDialogTop + 4 * aLineOffset);
        g->DrawString(LS(STRING_ID_MAX_COMBO), aCol1, aDialogTop + 5 * aLineOffset);

        g->SetColor(Color(0xFFA500));
        g->DrawString(itoa(mScore, buf, 10), aCol2, aDialogTop);
        g->DrawString(itoa(mBoard->mLevelStats.mNumCombos, buf, 10), aCol2, aDialogTop + aLineOffset);
        g->DrawString(itoa(mBoard->mLevelStats.mNumGemsCleared, buf, 10), aCol2, aDialogTop + 2 * aLineOffset);
        g->DrawString(itoa(mBoard->mLevelStats.mNumGaps, buf, 10), aCol2, aDialogTop + 3 * aLineOffset);
        g->DrawString(itoa(mBoard->mLevelStats.mMaxInARow, buf, 10), aCol2, aDialogTop + 4 * aLineOffset);
        g->DrawString(itoa(mBoard->mLevelStats.mMaxCombo + 1, buf, 10), aCol2, aDialogTop + 5 * aLineOffset);

        g->SetColor(Color(0xFFFF00));
        g->DrawString(LS(STRING_ID_YOUR_TIME), aCol3, aDialogTop);
        g->DrawString(LS(STRING_ID_ACE_TIME), aCol3, aDialogTop + aLineOffset);

        g->FillRect(aCol3, aDialogTop + aLineOffset + 4, 160, 1);

        g->SetColor(Color(mTargetTimeBonus > 0 ? 0x2AFF03 : 0xFFA500));
        g->DrawString(Sexy::GetTimeString(mBoard->mLevelStats.mTimePlayed / 100), aCol4, aDialogTop);

        g->SetColor(Color(0xFFA500));
        g->DrawString(Sexy::GetTimeString(mBoard->mLevelDesc->mParTime), aCol4, aDialogTop + aLineOffset);

        if (mTargetTimeBonus <= 0)
        {
            g->SetColor(Color(0x9DCBA2));
            g->DrawString(LS(STRING_ID_NO_TIME_BONUS), aCol3, 4 + aDialogTop + 2 * aLineOffset);
        }
        else if (mTimeBonus > 0)
        {
            // !PORT
            g->SetColor(Color(0x2AFF03));
            g->DrawString(Sexy::StrFormat(LS(STRING_ID_BONUS), mTimeBonus), aCol3, 4 + aDialogTop + 2 * aLineOffset);
        }

        g->SetColor(Color(0xF8F792));
        g->DrawString(mBestTimeLeft, aCol3, aDialogTop + 4 * aLineOffset);
        g->DrawString(mBestTimeRight, aCol3, aDialogTop + 5 * aLineOffset);

        aLineY = aDialogTop + 7 * aLineOffset;
    }
    else
    {
        int aColLeft = aDialogLeft + 130;
        int aColRight = aDialogLeft + 350;

        if (mIsWinning)
        {
            g->SetColor(Color(0xFFFF00));
            g->DrawString(LS(STRING_ID_EXTRA_LIFE_BONUS), aCol1, aDialogTop);
            g->SetColor(Color(0x2AFF03));
            if (mTimeBonus > 0)
            {
                g->DrawString(Sexy::StrFormat("+%d", mTimeBonus), aCol3, aDialogTop);
            }

            aDialogTop += 2 * aLineOffset;
        }

        g->SetColor(Color(0xFFFF00));
        g->DrawString(LS(STRING_ID_TOTAL_TIME), aCol1, aDialogTop);
        g->DrawString(LS(STRING_ID_COMBOS), aCol1, aDialogTop + aLineOffset);
        g->DrawString(LS(STRING_ID_COINS), aCol1, aDialogTop + 2 * aLineOffset);
        g->DrawString(LS(STRING_ID_GAPS), aCol3, aDialogTop);
        g->DrawString(LS(STRING_ID_MAX_CHAIN), aCol3, aDialogTop + aLineOffset);
        g->DrawString(LS(STRING_ID_MAX_COMBO), aCol3, aDialogTop + 2 * aLineOffset);

        g->SetColor(Color(0xFFA500));
        g->DrawString(Sexy::GetTimeString(mBoard->mGameStats.mTimePlayed / 100), aColLeft, aDialogTop);
        g->DrawString(itoa(mBoard->mGameStats.mNumCombos, buf, 10), aColLeft, aDialogTop + aLineOffset);
        g->DrawString(itoa(mBoard->mGameStats.mNumGemsCleared, buf, 10), aColLeft, aDialogTop + 2 * aLineOffset);
        g->DrawString(itoa(mBoard->mGameStats.mNumGaps, buf, 10), aColRight, aDialogTop);
        g->DrawString(itoa(mBoard->mGameStats.mMaxInARow, buf, 10), aColRight, aDialogTop + aLineOffset);
        g->DrawString(itoa(mBoard->mGameStats.mMaxCombo + 1, buf, 10), aColRight, aDialogTop + 2 * aLineOffset);

        aLineY = aDialogTop + 4 * aLineOffset;
    }

    if (!mCurrentTip.empty())
    {
        g->SetColor(Color(0xFFFF00));
        g->DrawString(LS(STRING_ID_TIP), aCol1, aLineY + 4);
        g->SetColor(Color(0xFFFFFF));
        g->SetFont(Sexy::FONT_DIALOG);

        Rect aRc = Rect(
            GetLeft() - mX + 60,
            aLineY - g->GetFont()->GetAscent(),
            GetWidth() - 80,
            200);

        WriteWordWrapped(g, aRc, mCurrentTip, -1, -1);
    }
}

void StatsDialog::FinishScoreInc()
{
    if (mTimeBonus < mTargetTimeBonus)
    {
        mBoard->IncScore(mTargetTimeBonus - mTimeBonus, false);
        mTimeBonus = mTargetTimeBonus;
    }
}
