#include "SexyAppFramework/SexyAppBase.h"
#include "CircleCommon.h"
#include "Localization.h"

using namespace Sexy;

static const char *STRING_IDS[STRING_ID_count] = {
    "STRING_ID_CONTINUE",
    "STRING_ID_BONUS",
    "STRING_ID_SURVIVAL",
    "STRING_ID_PAUSED",
    "STRING_ID_LEVEL",
    "STRING_ID_ADVENTURE",
    "STRING_ID_COMBO",
    "STRING_ID_TRIPLE_GAP_BONUS",
    "STRING_ID_DOUBLE_GAP_BONUS",
    "STRING_ID_GAP_BONUS",
    "STRING_ID_CHAIN_BONUS",
    "STRING_ID_SLOWDOWN_BALL",
    "STRING_ID_BACKWARDS_BALL",
    "STRING_ID_ACCURACY_BALL",
    "STRING_ID_RANDOM",
    "STRING_ID_RABBIT",
    "STRING_ID_EAGLE",
    "STRING_ID_JAGUAR",
    "STRING_ID_SUNGOD",
    "STRING_ID_RAB",
    "STRING_ID_EAG",
    "STRING_ID_JAG",
    "STRING_ID_SUN",
    "STRING_ID_OPEN_THE_FOLLOWING_URL_IN_YOUR_BROWSER",
    "STRING_ID_FOR_YOUR_CONVENIENCE",
    "STRING_ID_OPEN_BROWSER",
    "STRING_ID_OK",
    "STRING_ID_OPENING_BROWSER",
    "STRING_ID_GET_READY",
    "STRING_ID_GO",
    "STRING_ID_UPDATES",
    "STRING_ID_DO_YOU_WANT_TO_CHECK_FOR_UPDATES_TO_ZUMA",
    "STRING_ID_ENTER_YOUR_NAME",
    "STRING_ID_ENTER_YOUR_NAME_TO_CREATE_A_NEW_USER_PROFILE",
    "STRING_ID_NAME_CONFLICT",
    "STRING_ID_THE_NAME_YOU_ENTERED_IS_ALREADY_BEING_USED",
    "STRING_ID_ARE_YOU_SURE_?",
    "STRING_ID_THIS_WILL_PERMANENTLY_REMOVE_PLAYER_ROSTER",
    "STRING_ID_CONTINUE_?",
    "STRING_ID_YOUR_GAME_WAS_SAVED_WHEN_YOU_QUIT",
    "STRING_ID_DO_YOU_WANT_TO_CONTINUE_YOUR_LAST_GAME_?",
    "STRING_ID_SM_CONTINUE",
    "STRING_ID_NEW_GAME",
    "STRING_ID_ENTER_NEXT_TEMPLE",
    "STRING_ID_YOU_ARE_NOW_GOING_TO_ENTER_THE_NEXT_TEMPLE",
    "STRING_ID_INVALID_CODE",
    "STRING_ID_LICENSE_CODE_NOT_VALID",
    "STRING_ID_YOU_HAVE_ALREADY_REGISTERED_ZUMA",
    "STRING_ID_ALREADY_REGISTERED",
    "STRING_ID_QUIT_ZUMA_?",
    "STRING_ID_QUIT_THE_GAME_?",
    "STRING_ID_LEAVE_GAME_?",
    "STRING_ID_DO_YOU_WANT_TO_CONTINUE_?",
    "STRING_ID_LEAVE",
    "STRING_ID_PLEASE_REGISTER",
    "STRING_ID_TRIAL_VERSION_EXPIRED",
    "STRING_ID_REGISTER",
    "STRING_ID_QUIT",
    "STRING_ID_TEMPLE_OF_ZUKULKAN",
    "STRING_ID_QUETZAL_QUATL",
    "STRING_ID_POPO_POYOLLI",
    "STRING_ID_SECRET_SHRINE_OF_ZUMA",
    "STRING_ID_DO_YOU_DARE_ENTER_THE_ANCIENT_RUINS",
    "STRING_ID_THE_2ND_LEVEL_OF_THE_TEMPLE_AWAITS",
    "STRING_ID_WILL_YOU_DESCEND_TO_THE_THIRD_LEVEL",
    "STRING_ID_YOU_HAVE_UNCOVERED_A_HIDDEN_DOOR",
    "STRING_ID_THE_GOLDEN_SECRETS_OF_ZUMA_AWAIT_YOU",
    "STRING_ID_DO_YOU_DARE_ENTER_THE_LAST_CHAMBER",
    "STRING_ID_ANOTHER_LOST_TEMPLE",
    "STRING_ID_CAN_YOU_FULFILL_THE_PROPHECY_?",
    "STRING_ID_THE_LAST_STAGE_?",
    "STRING_ID_RUINED_TEMPLE_RISES",
    "STRING_ID_THE_FINAL_SECRET",
    "STRING_ID_ENTER_THE_RUINS",
    "STRING_ID_UNCOVER_THE_SECRET_TEMPLES",
    "STRING_ID_CLICK_ON_A_DOORWAY",
    "STRING_ID_PLEASE_ENTER_YOUR_NAME",
    "STRING_ID_WHATEVER",
    "STRING_ID_CREDITS",
    "STRING_ID_GAME_DESIGN",
    "STRING_ID_PROGRAMMING",
    "STRING_ID_ART",
    "STRING_ID_SOUND_FX",
    "STRING_ID_MUSIC",
    "STRING_ID_LEVEL_DESIGN",
    "STRING_ID_QA",
    "STRING_ID_POPCAP_FRAMEWORK",
    "STRING_ID_BIZ_DEV",
    "STRING_ID_SPECIAL_THANKS_TO",
    "STRING_ID_SHOOT_BALLS_BY_CLICKING_ON_SCREEN",
    "STRING_ID_THE_GEM_IN_THE_BACK_OF_THE_FROG",
    "STRING_ID_CLICK",
    "STRING_ID_CLEAR_BALLS_BY",
    "STRING_ID_LIKE_COLORED_BALLS_ATTRACT",
    "STRING_ID_DONT_LET_THE_BALLS_REACH_THE_SKULL",
    "STRING_ID_SHOOT_COINS_FOR_EXTRA_POINTS",
    "STRING_ID_RIGHT_MOUSE_BUTTON_WILL_SWAP_THE_BALLS",
    "STRING_ID_DONT_SHOW_THIS_SCREEN_AT_STARTUP",
    "STRING_ID_YOU_HAVE_1_MINUTE_LEFT_IN_THIS_TRIAL",
    "STRING_ID_YOU_HAVE",
    "STRING_ID_MINUTES_LEFT_IN_THIS_TRIAL",
    "STRING_ID_YOU_HAVE_1_FREE_PLAY_REMAINING",
    "STRING_ID_YOU_HAVE_2",
    "STRING_ID_FREE_PLAYS_REMAINING",
    "STRING_ID_CLICK_HERE_TO_PLAY",
    "STRING_ID_CLICK_HERE_TO_REGISTER",
    "STRING_ID_THANKS_FOR_REGISTERING",
    "STRING_ID_IF_THIS_IS_NOT_YOU",
    "STRING_ID_WELCOME_TO_ZUMA",
    "STRING_ID_WELCOME_TO_ZUMA1",
    "STRING_ID_BACK_TO_MAIN_MENU",
    "STRING_ID_MENU",
    "STRING_ID_OPTIONS",
    "STRING_ID_BACK_TO_GAME",
    "STRING_ID_DONE",
    "STRING_ID_HELP",
    "STRING_ID_CHECK_FOR_UPDATES",
    "STRING_ID_MUSIC_VOLUME",
    "STRING_ID_SOUND_EFFECTS",
    "STRING_ID_FULLSCREEN",
    "STRING_ID_CUSTOM_CURSORS",
    "STRING_ID_3D_HARDWARE_ACCELERATION",
    "STRING_ID_NO_WINDOWED_MODE",
    "STRING_ID_WINDOWED_MODE_IS_ONLY_AVAILABLE",
    "STRING_ID_NOT_SUPPORTED",
    "STRING_ID_HARDWARE_ACCELERATION_CANNOT_BE_ENABLED",
    "STRING_ID_WARNING",
    "STRING_ID_YOUR_VIDEO_CARD_MAY_NOT_FULLY_SUPPORT_THIS_FEATURE",
    "STRING_ID_LOCKED",
    "STRING_ID_PRACTICE",
    "STRING_ID_PRACTICE_A_SINGLE_BOARD_UNTIL_MASTERED",
    "STRING_ID_YOU_MUST_FIRST_UNLOCK_THIS_LEVEL_IN_ADVENTURE_MODE",
    "STRING_ID_PLAY_RANDOM_LEVEL_UNLOCKED_IN_ADVENTURE_MODE",
    "STRING_ID_PLAY_THROUGH_ALL_THE_LEVELS_IN_RANDOM_ORDER",
    "STRING_ID_HOW_LONG_CAN_YOU_SURVIVE",
    "STRING_ID_REGISTER_ZUMA",
    "STRING_ID_OBTAIN_A_REGISTRATION_CODE_ONLINE",
    "STRING_ID_CANCEL",
    "STRING_ID_CLICK_HERE_TO_GET_REGISTRATION_CODE",
    "STRING_ID_YOUR_NAME",
    "STRING_ID_YOUR_REGISTRATION_CODE",
    "STRING_ID_FILL_THE_ORANGE_GAUGE",
    "STRING_ID_SCORE_POINTS_TO_FILL_THE_ORANGE_GAUGE",
    "STRING_ID_CLICK_THE_RIGHT_MOUSE_BUTTON_TO_SWAP",
    "STRING_ID_SHOOT_BALLS_THROUGH_A_GAP_TO_SCORE",
    "STRING_ID_CREATE_COMBOS_OF_2_OR_MORE_EXPLOSIONS",
    "STRING_ID_CLOSE_GAPS_BY_PLACING_BALLS_OF_THE_SAME_COLOR",
    "STRING_ID_GET_A_COIN_FOR_A_BIG_POINT_BOOST_TO_FINISH",
    "STRING_ID_FINISH_A_LEVEL_QUICKLY",
    "STRING_ID_GET_AN_EXTRA_FROG",
    "STRING_ID_SHOOT_BALLS_THROUGH_TWO_OR_MORE_GAPS",
    "STRING_ID_PRESS_SPACE_BAR_TO_PAUSE",
    "STRING_ID_PRESS_T_TO_SHOW_TIME",
    "STRING_ID_ONCE_YOU_CAN_ACCEPT_THE_UNIVERSE",
    "STRING_ID_THERE_IS_ONLY_ONE_CORNER_OF_THE_UNIVERSE",
    "STRING_ID_IT_IS_ALWAYS_WISE_TO_LOOK_AHEAD",
    "STRING_ID_GOOD_TIMBER_DOES_NOT_GROW_WITH_EASE",
    "STRING_ID_THE_FACT_THAT_NO_ONE_UNDERSTANDS",
    "STRING_ID_HONEST_CRITICISM_IS_HARD_TO_TAKE",
    "STRING_ID_A_GOOD_PLAN_TODAY_IS_BETTER",
    "STRING_ID_GOOD_LUCK_IS_OFTEN_WITH_THE_MAN",
    "STRING_ID_WINNING_TOTALS",
    "STRING_ID_GAME_OVER",
    "STRING_ID_PRESS_THE_S_KEY",
    "STRING_ID_BEST_TIME",
    "STRING_ID_EXTRA_LIFE_BONUS",
    "STRING_ID_TOTAL_TIME",
    "STRING_ID_COMBOS",
    "STRING_ID_COINS",
    "STRING_ID_GAPS",
    "STRING_ID_MAX_CHAIN",
    "STRING_ID_MAX_COMBO",
    "STRING_ID_POINTS",
    "STRING_ID_YOUR_TIME",
    "STRING_ID_ACE_TIME",
    "STRING_ID_NO_TIME_BONUS",
    "STRING_ID_TIP",
    "STRING_ID_WHO_ARE_YOU_?",
    "STRING_ID_RENAME",
    "STRING_ID_DELETE",
    "STRING_ID_CREATE_A_NEW_USER",
    "STRING_ID_TITLE",
    "STRING_ID_SCORE",
    "STRING_ID_RENAME_USER",
    "STRING_ID_NEW_USER",
    "STRING_ID_STATS",
    "STRING_ID_LAST_LIFE",
    "STRING_ID_LIVES_LEFT",
    "STRING_ID_STAGE",
    "STRING_ID_COMPLETE",
    "STRING_ID_INTERLOPER_OF_PUNY",
    "STRING_ID_I_SEE_WHAT",
    "STRING_ID_NOT_POSSIBLE_TO_BELIEVE",
    "STRING_ID_FINAL_TEMPLE_STRUCK",
    "STRING_ID_WE_COME_FROM_STAR",
    "STRING_ID_EXCELLENT_WORK_FROG_ONE",
    "STRING_ID_RIBBIT",
    "STRING_ID_RIBBIT_?",
    "STRING_ID_RIBBIT_!",
    "STRING_ID_I_BEG_TO_DIFFER",
    "STRING_ID_NEVER_GIVE_UP",
    "STRING_ID_FULFILL_THE_PROPHECY",
    "STRING_ID_IN_SPACE",
    "STRING_ID_WERD",
    "STRING_ID_CONGRATULATIONS",
    "DIALOG_BUTTON_YES",
    "DIALOG_BUTTON_NO",
    "DIALOG_BUTTON_OK",
    "DIALOG_BUTTON_CANCEL",
    "UPDATE_CHECK_TITLE",
    "UPDATE_CHECK_BODY",
    "UP_TO_DATE_TITLE",
    "UP_TO_DATE_BODY",
    "NEW_VERSION_TITLE",
    "NEW_VERSION_BODY",
    "STRING_ID_MORE_GAMES_URL",
    "STRING_ID_LVL",
};
static const char *STRINGS[STRING_ID_count];
extern Sexy::SexyAppBase *Sexy::gSexyAppBase;

static const char *GetLocalizationIdString(int string_id)
{
    const char *result = "???";
    if (string_id >= 0 && string_id < STRING_ID_count)
    {
        result = STRING_IDS[string_id];
    }
    return result;
}
void SetupLocalizations()
{
    // initialize english strings
    const auto Set = [](int string_id, const char *string)
    {
        const char *str_string_id = GetLocalizationIdString(string_id);
        Sexy::gSexyAppBase->SetString(str_string_id, string);
    };
    Set(STRING_ID_CONTINUE, "CONTINUE");
    Set(STRING_ID_BONUS, "BONUS");
    Set(STRING_ID_SURVIVAL, "Survival");
    Set(STRING_ID_PAUSED, "PAUSED");
    Set(STRING_ID_LEVEL, "LeVeL");
    Set(STRING_ID_ADVENTURE, "");
    Set(STRING_ID_COMBO, "COMBO");
    Set(STRING_ID_TRIPLE_GAP_BONUS, "TRIPLE GAP BONUS");
    Set(STRING_ID_DOUBLE_GAP_BONUS, "DOUBLE GAP BONUS");
    Set(STRING_ID_GAP_BONUS, "GAP BONUS");
    Set(STRING_ID_CHAIN_BONUS, "CHAIN BONUS");
    Set(STRING_ID_SLOWDOWN_BALL, "SLOWDOWN Ball");
    Set(STRING_ID_BACKWARDS_BALL, "BACKWARDS Ball");
    Set(STRING_ID_ACCURACY_BALL, "ACCURACY Ball");
    Set(STRING_ID_RANDOM, "Random");
    Set(STRING_ID_RABBIT, "RABBIT");
    Set(STRING_ID_EAGLE, "EAGLE");
    Set(STRING_ID_JAGUAR, "JAGUAR");
    Set(STRING_ID_SUNGOD, "SUNGOD");
    Set(STRING_ID_RAB, "RAB");
    Set(STRING_ID_EAG, "EAG");
    Set(STRING_ID_JAG, "JAG");
    Set(STRING_ID_SUN, "SUN");
    Set(STRING_ID_OPEN_THE_FOLLOWING_URL_IN_YOUR_BROWSER, "");
    Set(STRING_ID_FOR_YOUR_CONVENIENCE, "");
    Set(STRING_ID_OPEN_BROWSER, "");
    Set(STRING_ID_OK, "");
    Set(STRING_ID_OPENING_BROWSER, "");
    Set(STRING_ID_GET_READY, "Get Ready!");
    Set(STRING_ID_GO, "GO");
    Set(STRING_ID_UPDATES, "");
    Set(STRING_ID_DO_YOU_WANT_TO_CHECK_FOR_UPDATES_TO_ZUMA, "");
    Set(STRING_ID_ENTER_YOUR_NAME, "Enter Your Name");
    Set(STRING_ID_ENTER_YOUR_NAME_TO_CREATE_A_NEW_USER_PROFILE, "Enter your name to create a new user profile for storing high score data and games in progress.");
    Set(STRING_ID_NAME_CONFLICT, "Name Conflict");
    Set(STRING_ID_THE_NAME_YOU_ENTERED_IS_ALREADY_BEING_USED, "The name you entered is already being used.  Please enter a unique player name.");
    Set(STRING_ID_ARE_YOU_SURE_QM, "Are You Sure?");
    Set(STRING_ID_THIS_WILL_PERMANENTLY_REMOVE_PLAYER_ROSTER, "This will permanently remove '%s' from the player roster!");
    Set(STRING_ID_CONTINUE_QM, "CONTINUE?");
    Set(STRING_ID_YOUR_GAME_WAS_SAVED_WHEN_YOU_QUIT, "Your game was saved when you quit.  If you do not continue now, the game will be lost.");
    Set(STRING_ID_DO_YOU_WANT_TO_CONTINUE_YOUR_LAST_GAME_QM, "Do you want to continue your last game?");
    Set(STRING_ID_SM_CONTINUE, "Continue");
    Set(STRING_ID_NEW_GAME, "New Game");
    Set(STRING_ID_ENTER_NEXT_TEMPLE, "Enter Next Temple");
    Set(STRING_ID_YOU_ARE_NOW_GOING_TO_ENTER_THE_NEXT_TEMPLE, "You are now going to enter the next temple.\nGet Ready!");
    Set(STRING_ID_INVALID_CODE, "");
    Set(STRING_ID_LICENSE_CODE_NOT_VALID, "");
    Set(STRING_ID_YOU_HAVE_ALREADY_REGISTERED_ZUMA, "");
    Set(STRING_ID_ALREADY_REGISTERED, "");
    Set(STRING_ID_QUIT_ZUMA_QM, "Quit Zuma?");
    Set(STRING_ID_QUIT_THE_GAME_QM, "Are you sure you want to\nquit the game?");
    Set(STRING_ID_LEAVE_GAME_QM, "Leave Game?");
    Set(STRING_ID_DO_YOU_WANT_TO_CONTINUE_QM, "Your game session will be saved upon leaving. Do you want to continue?");
    Set(STRING_ID_LEAVE, "Leave");
    Set(STRING_ID_PLEASE_REGISTER, "");
    Set(STRING_ID_TRIAL_VERSION_EXPIRED, "");
    Set(STRING_ID_REGISTER, "Register");
    Set(STRING_ID_QUIT, "");
    Set(STRING_ID_TEMPLE_OF_ZUKULKAN, "TEMPLE OF ZUKULKAN");
    Set(STRING_ID_QUETZAL_QUATL, "QUETZAL QUATL");
    Set(STRING_ID_POPO_POYOLLI, "POPO POYOLLI");
    Set(STRING_ID_SECRET_SHRINE_OF_ZUMA, "SECRET SHRINE OF ZUMA");
    Set(STRING_ID_DO_YOU_DARE_ENTER_THE_ANCIENT_RUINS, "DO YOU DARE ENTER THE ANCIENT RUINS?");
    Set(STRING_ID_THE_2ND_LEVEL_OF_THE_TEMPLE_AWAITS, "THE 2ND LEVEL OF THE TEMPLE AWAITS.");
    Set(STRING_ID_WILL_YOU_DESCEND_TO_THE_THIRD_LEVEL, "WILL YOU DESCEND TO THE THIRD LEVEL?");
    Set(STRING_ID_YOU_HAVE_UNCOVERED_A_HIDDEN_DOOR, "YOU HAVE UNCOVERED A HIDDEN DOOR!");
    Set(STRING_ID_THE_GOLDEN_SECRETS_OF_ZUMA_AWAIT_YOU, "THE GOLDEN SECRETS OF ZUMA AWAIT YOU.");
    Set(STRING_ID_DO_YOU_DARE_ENTER_THE_LAST_CHAMBER, "DO YOU DARE ENTER THE LAST CHAMBER?");
    Set(STRING_ID_ANOTHER_LOST_TEMPLE, "ANOTHER LOST TEMPLE...");
    Set(STRING_ID_CAN_YOU_FULFILL_THE_PROPHECY_QM, "CAN YOU FULFILL THE PROPHECY?");
    Set(STRING_ID_THE_LAST_STAGE_QM, "THE LAST STAGE (?)");
    Set(STRING_ID_RUINED_TEMPLE_RISES, "A RUINED TEMPLE RISES FROM THE EARTH.");
    Set(STRING_ID_THE_FINAL_SECRET, "THE FINAL SECRET OF ZUMA BECKONS...");
    Set(STRING_ID_ENTER_THE_RUINS, "YOU ENTER THE RUINS ONE LAST TIME...");
    Set(STRING_ID_UNCOVER_THE_SECRET_TEMPLES, "Uncover the secret temples of Zuma!");
    Set(STRING_ID_CLICK_ON_A_DOORWAY, "Click on a doorway to select a different stage.");
    Set(STRING_ID_PLEASE_ENTER_YOUR_NAME, "Please enter your name.");
    Set(STRING_ID_WHATEVER, "Whatever");
    Set(STRING_ID_CREDITS, "CREDITS");
    Set(STRING_ID_GAME_DESIGN, "Game Design");
    Set(STRING_ID_PROGRAMMING, "Programming");
    Set(STRING_ID_ART, "Art");
    Set(STRING_ID_SOUND_FX, "Sound FX");
    Set(STRING_ID_MUSIC, "Music");
    Set(STRING_ID_LEVEL_DESIGN, "Level Design");
    Set(STRING_ID_QA, "QA");
    Set(STRING_ID_POPCAP_FRAMEWORK, "PopCap Framework");
    Set(STRING_ID_BIZ_DEV, "Biz Dev");
    Set(STRING_ID_SPECIAL_THANKS_TO, "Special Thanks To");
    Set(STRING_ID_SHOOT_BALLS_BY_CLICKING_ON_SCREEN, "Shoot balls by clicking on screen where you want the ball to go.");
    Set(STRING_ID_THE_GEM_IN_THE_BACK_OF_THE_FROG, "The gem in the back of the frog shows the color of the next ball.");
    Set(STRING_ID_CLICK, "CLICK!");
    Set(STRING_ID_CLEAR_BALLS_BY, "Clear balls by getting three or more of the same color in a row.");
    Set(STRING_ID_LIKE_COLORED_BALLS_ATTRACT, "Like colored balls attract and can create chain reactions!");
    Set(STRING_ID_DONT_LET_THE_BALLS_REACH_THE_SKULL, "Don't let the balls reach the skull!");
    Set(STRING_ID_SHOOT_COINS_FOR_EXTRA_POINTS, "Shoot coins for extra points!");
    Set(STRING_ID_RIGHT_MOUSE_BUTTON_WILL_SWAP_THE_BALLS, "Right mouse button will swap the balls.");
    Set(STRING_ID_DONT_SHOW_THIS_SCREEN_AT_STARTUP, "Don't show this screen at startup.");
    Set(STRING_ID_YOU_HAVE_1_MINUTE_LEFT_IN_THIS_TRIAL, "");
    Set(STRING_ID_YOU_HAVE, "");
    Set(STRING_ID_MINUTES_LEFT_IN_THIS_TRIAL, "");
    Set(STRING_ID_YOU_HAVE_1_FREE_PLAY_REMAINING, "");
    Set(STRING_ID_YOU_HAVE_2, "");
    Set(STRING_ID_FREE_PLAYS_REMAINING, "");
    Set(STRING_ID_CLICK_HERE_TO_PLAY, "Click Here To Play!");
    Set(STRING_ID_CLICK_HERE_TO_REGISTER, "Click Here To Register!");
    Set(STRING_ID_THANKS_FOR_REGISTERING, "");
    Set(STRING_ID_IF_THIS_IS_NOT_YOU, "If this is not you, click here.");
    Set(STRING_ID_WELCOME_TO_ZUMA, "Welcome to Zuma");
    Set(STRING_ID_BACK_TO_MAIN_MENU, "Back To Main Menu");
    Set(STRING_ID_MENU, "MENU");
    Set(STRING_ID_OPTIONS, "OPTIONS");
    Set(STRING_ID_BACK_TO_GAME, "Back To Game");
    Set(STRING_ID_DONE, "Done");
    Set(STRING_ID_HELP, "Help");
    Set(STRING_ID_CHECK_FOR_UPDATES, "Check For Updates");
    Set(STRING_ID_MUSIC_VOLUME, "Music Volume");
    Set(STRING_ID_SOUND_EFFECTS, "Sound Effects");
    Set(STRING_ID_FULLSCREEN, "Fullscreen");
    Set(STRING_ID_CUSTOM_CURSORS, "Custom Cursors");
    Set(STRING_ID_3D_HARDWARE_ACCELERATION, "3-D Hardware Acceleration");
    Set(STRING_ID_NO_WINDOWED_MODE, "No Windowed Mode");
    Set(STRING_ID_WINDOWED_MODE_IS_ONLY_AVAILABLE, "Windowed mode is only available if your desktop is running in either 16 bit or 32 bit color mode, which it is not.");
    Set(STRING_ID_NOT_SUPPORTED, "Not Supported");
    Set(STRING_ID_HARDWARE_ACCELERATION_CANNOT_BE_ENABLED, "Hardware Acceleration cannot be enabled on this computer.\nYour video card does not meet the minimum requirements for this game.");
    Set(STRING_ID_WARNING, "Warning");
    Set(STRING_ID_YOUR_VIDEO_CARD_MAY_NOT_FULLY_SUPPORT_THIS_FEATURE, "Your video card may not fully support this feature.\nIf you experience slower performance, please disable Hardware Acceleration.");
    Set(STRING_ID_LOCKED, "LOCKED");
    Set(STRING_ID_PRACTICE, "Practice");
    Set(STRING_ID_PRACTICE_A_SINGLE_BOARD_UNTIL_MASTERED, "PRACTICE A SINGLE BOARD UNTIL MASTERED.  CAN YOU BECOME A SUN GOD?");
    Set(STRING_ID_YOU_MUST_FIRST_UNLOCK_THIS_LEVEL_IN_ADVENTURE_MODE, "YOU MUST FIRST UNLOCK THIS LEVEL IN ADVENTURE MODE.");
    Set(STRING_ID_PLAY_RANDOM_LEVEL_UNLOCKED_IN_ADVENTURE_MODE, "PLAY A RANDOM LEVEL THAT YOU HAVE UNLOCKED IN ADVENTURE MODE.");
    Set(STRING_ID_PLAY_THROUGH_ALL_THE_LEVELS_IN_RANDOM_ORDER, "PLAY THROUGH ALL THE LEVELS YOU HAVE UNLOCKED IN ADVENTURE MODE IN RANDOM ORDER!");
    Set(STRING_ID_HOW_LONG_CAN_YOU_SURVIVE, "HOW LONG CAN YOU SURVIVE AN ENDLESS STREAM OF ZUMAIC INSANITY?");
    Set(STRING_ID_REGISTER_ZUMA, "");
    Set(STRING_ID_OBTAIN_A_REGISTRATION_CODE_ONLINE, "");
    Set(STRING_ID_CANCEL, "");
    Set(STRING_ID_CLICK_HERE_TO_GET_REGISTRATION_CODE, "");
    Set(STRING_ID_YOUR_NAME, "");
    Set(STRING_ID_YOUR_REGISTRATION_CODE, "");
    Set(STRING_ID_FILL_THE_ORANGE_GAUGE, "Fill the orange gauge to complete a level. No new balls will enter the screen.");
    Set(STRING_ID_SCORE_POINTS_TO_FILL_THE_ORANGE_GAUGE, "Score points to fill the orange gauge.  The more points, the faster the gauge fills.");
    Set(STRING_ID_CLICK_THE_RIGHT_MOUSE_BUTTON_TO_SWAP, "Click the right mouse button to swap the current ball with the next ball.");
    Set(STRING_ID_SHOOT_BALLS_THROUGH_A_GAP_TO_SCORE, "Shoot balls through a gap to score a gap bonus!  The smaller the gap, the larger the bonus!");
    Set(STRING_ID_CREATE_COMBOS_OF_2_OR_MORE_EXPLOSIONS, "Create combos of 2 or more explosions to score big points and push the curve backwards!");
    Set(STRING_ID_CLOSE_GAPS_BY_PLACING_BALLS_OF_THE_SAME_COLOR, "Close gaps by placing balls of the same color on either side.");
    Set(STRING_ID_GET_A_COIN_FOR_A_BIG_POINT_BOOST_TO_FINISH, "Get a coin for a big point boost to finish the level more quickly!");
    Set(STRING_ID_FINISH_A_LEVEL_QUICKLY, "Finish a level quickly and you can get bonus points!");
    Set(STRING_ID_GET_AN_EXTRA_FROG, "Get an extra frog every 50,000 points!");
    Set(STRING_ID_SHOOT_BALLS_THROUGH_TWO_OR_MORE_GAPS, "Shoot balls through two or more gaps to score a double gap bonus!");
    Set(STRING_ID_PRESS_SPACE_BAR_TO_PAUSE, "Press space bar to pause the game.");
    Set(STRING_ID_PRESS_T_TO_SHOW_TIME, "Press 't' to show the amount of time left to get an Ace time.");
    Set(STRING_ID_ONCE_YOU_CAN_ACCEPT_THE_UNIVERSE, "Once you can accept the universe as matter expanding into nothing that is something, wearing stripes with plaid comes easy.");
    Set(STRING_ID_THERE_IS_ONLY_ONE_CORNER_OF_THE_UNIVERSE, "There is only one corner of the universe you can be certain of improving and that is your own self.");
    Set(STRING_ID_IT_IS_ALWAYS_WISE_TO_LOOK_AHEAD, "It is always wise to look ahead but difficult to look farther than you can see.");
    Set(STRING_ID_GOOD_TIMBER_DOES_NOT_GROW_WITH_EASE, "Good timber does not grow with ease. The stronger the wind the stronger the trees.");
    Set(STRING_ID_THE_FACT_THAT_NO_ONE_UNDERSTANDS, "The fact that no one understands you doesn't make you an artist.");
    Set(STRING_ID_HONEST_CRITICISM_IS_HARD_TO_TAKE, "Honest criticism is hard to take, particularly from a relative, a friend, an acquaintance, or a stranger.");
    Set(STRING_ID_A_GOOD_PLAN_TODAY_IS_BETTER, "A good plan today is better than a perfect plan tomorrow.");
    Set(STRING_ID_GOOD_LUCK_IS_OFTEN_WITH_THE_MAN, "Good luck is often with the man who doesn't include it in his plans.");
    Set(STRING_ID_WINNING_TOTALS, "WINNING TOTALS");
    Set(STRING_ID_GAME_OVER, "GAME OVER");
    Set(STRING_ID_PRESS_THE_S_KEY, "Press the 's' key to toggle star scrolling.");
    Set(STRING_ID_BEST_TIME, "Best Time ");
    Set(STRING_ID_EXTRA_LIFE_BONUS, "EXTRA LIFE BONUS");
    Set(STRING_ID_TOTAL_TIME, "TOTAL TIME");
    Set(STRING_ID_COMBOS, "COMBOS");
    Set(STRING_ID_COINS, "COINS");
    Set(STRING_ID_GAPS, "GAPS");
    Set(STRING_ID_MAX_CHAIN, "MAX CHAIN");
    Set(STRING_ID_MAX_COMBO, "MAX COMBO");
    Set(STRING_ID_POINTS, "POINTS");
    Set(STRING_ID_YOUR_TIME, "YOUR TIME");
    Set(STRING_ID_ACE_TIME, "ACE TIME");
    Set(STRING_ID_NO_TIME_BONUS, "NO TIME BONUS");
    Set(STRING_ID_TIP, "TIP");
    Set(STRING_ID_WHO_ARE_YOU_QM, "Who are you?");
    Set(STRING_ID_RENAME, "Rename");
    Set(STRING_ID_DELETE, "Delete");
    Set(STRING_ID_CREATE_A_NEW_USER, "(Create a New User)");
    Set(STRING_ID_TITLE, "");
    Set(STRING_ID_SCORE, "Score:");
    Set(STRING_ID_RENAME_USER, "");
    Set(STRING_ID_NEW_USER, "NEW USER");
    Set(STRING_ID_STATS, "STATS");
    Set(STRING_ID_LAST_LIFE, "LAST LIFE!");
    Set(STRING_ID_LIVES_LEFT, "LIVES LEFT");
    Set(STRING_ID_STAGE, "STAGE");
    Set(STRING_ID_COMPLETE, "COMPLETE");
    Set(STRING_ID_INTERLOPER_OF_PUNY, "Interloper of puny!  You suppose the secret of Zuma it is possible to take the knowing so easily?  No!  There is defect for your thinking!  Our cryptic hidden nature cannot be discovered so directly.  Three mystery shrines protect method!  Never you will strike past them!");
    Set(STRING_ID_I_SEE_WHAT, "I see what?!?  You have lived still.  Is it possible to be the one of which the ancient prophecy speaks?  It was sent in order perhaps to release me from my wicked capture person finally.  But no namely this was the story of the exactly old wive.  You want to live, now retreat!");
    Set(STRING_ID_NOT_POSSIBLE_TO_BELIEVE, "As for me it is not possible to believe!  Your power namely that is large!  But there is one more temple which you do not find, that the Zuma deeply buried under the land.  It was hidden, it is the temple of secret!  You the final temple must fight in order to take the cover of position of my jail!!");
    Set(STRING_ID_FINAL_TEMPLE_STRUCK, "The final temple of Zuma was struck!  Your extreme power is not possible to be defeated!  As for me it can taste the taste whose almost freedom is sweet sweetly.  But God of the sun can be imprisoned at only that true house.");
    Set(STRING_ID_WE_COME_FROM_STAR, "We come from the star, return to the star!  It is of you to rescue of my star.");
    Set(STRING_ID_EXCELLENT_WORK_FROG_ONE, "Excellent work, mighty frog one!  You obtained your lawful place of the stars, at the side of the ancient ruler of the Zuma.  In the future, as for me, you are known as our sibling.  I will call you brother, and we control the outer space together!  It is joyous news!  Now we are to the dance!");
    Set(STRING_ID_RIBBIT, "Ribbit.");
    Set(STRING_ID_RIBBIT_QM, "Ribbit?");
    Set(STRING_ID_RIBBIT_EM, "Ribbit!");
    Set(STRING_ID_I_BEG_TO_DIFFER, "(I beg to differ.)");
    Set(STRING_ID_NEVER_GIVE_UP, "(I shall never give up.)");
    Set(STRING_ID_FULFILL_THE_PROPHECY, "(I will fulfill the prophecy.)");
    Set(STRING_ID_IN_SPACE, "(In space?)");
    Set(STRING_ID_WERD, "(Werd!)");
    Set(STRING_ID_CONGRATULATIONS, "CONGRATULATIONS!");
    Set(DIALOG_BUTTON_YES, "YES");
    Set(DIALOG_BUTTON_NO, "NO");
    Set(DIALOG_BUTTON_OK, "OK");
    Set(DIALOG_BUTTON_CANCEL, "CANCEL");
    Set(UPDATE_CHECK_TITLE, "");
    Set(UPDATE_CHECK_BODY, "");
    Set(UP_TO_DATE_TITLE, "");
    Set(UP_TO_DATE_BODY, "");
    Set(NEW_VERSION_TITLE, "");
    Set(NEW_VERSION_BODY, "");
    Set(STRING_ID_MORE_GAMES_URL, "");
    Set(STRING_ID_LVL, "LVL");
}
static std::string GetFormatter(int string_id, std::string value)
{
    std::string result;
    if (string_id == STRING_ID_LEVEL)
    {
        result = Sexy::StrFormat("%s %%d-%%d", value.c_str());
    }
    else if (string_id == STRING_ID_LVL)
    {
        result = Sexy::StrFormat("%s %%d-%%d", value.c_str());
    }
    else if (string_id == STRING_ID_LVL)
    {
        result = Sexy::StrFormat("%s %%d-%%d", value.c_str());
    }
    else if (string_id == STRING_ID_SCORE)
    {
        result = Sexy::StrFormat("%%s (%%s)\n%s： %%d\n", value.c_str());
    }
    else if (string_id == STRING_ID_COMBO)
    {
        result = Sexy::StrFormat("%s x%%d", value.c_str());
    }
    else if (string_id == STRING_ID_CHAIN_BONUS)
    {
        result = Sexy::StrFormat("%s x%%d", value.c_str());
    }
    else if (string_id == STRING_ID_BONUS)
    {
        result = Sexy::StrFormat("%s +%%d", value.c_str());
    }
    else if (string_id == STRING_ID_STAGE)
    {
        result = Sexy::StrFormat("%s %%d", value.c_str());
    }
    else if (string_id == STRING_ID_LIVES_LEFT)
    {
        result = Sexy::StrFormat("%%d %s", value.c_str());
    }
    else if (string_id == STRING_ID_WELCOME_TO_ZUMA)
    {
        result = Sexy::StrFormat("%s, %%s!", value.c_str());
    }
    else if (string_id == STRING_ID_THIS_WILL_PERMANENTLY_REMOVE_PLAYER_ROSTER)
    {
        result = "This will permanently remove '%s' from the player roster!";
    }
    return result;
}
const char *LS(int string_id)
{
    const char *result = "???";
    if (string_id >= 0 && string_id < STRING_ID_count)
    {
        if (STRINGS[string_id] == NULL)
        {
            const char *str_string_id = GetLocalizationIdString(string_id);
            std::string value = Sexy::gSexyAppBase->GetString(str_string_id, "");

            // use StrFormat newlines, too many in this string
            if (string_id == STRING_ID_YOUR_GAME_WAS_SAVED_WHEN_YOU_QUIT || 
                string_id == STRING_ID_DO_YOU_WANT_TO_CONTINUE_YOUR_LAST_GAME_QM)
            {
                // remove line endings in latin1 versions
                size_t i = value.find("\n\n\n\n");
                if (i != std::string::npos)
                {
                    value.erase(i);
                }
            }
            
            if (string_id == STRING_ID_YOUR_GAME_WAS_SAVED_WHEN_YOU_QUIT)
            {
                // TW two strings combined into one, separate them
                size_t i0 = value.find("\n\n");
                size_t i1 = value.rfind("\n\n");
                if (i0 != std::string::npos && i1 != std::string::npos && i0 < i1)
                {
                    std::string quit = value.substr(0, i0);
                    std::string cont = value.substr(i0 + 2, i1 - (i0 + 2));
                    value = quit;
                    STRINGS[STRING_ID_DO_YOU_WANT_TO_CONTINUE_YOUR_LAST_GAME_QM] = strdup(cont.c_str()); // not in TW
                }
            }

            // TW more strings use formatters
            if (value.find('%') == std::string::npos)
            {
                std::string formatter = GetFormatter(string_id, value);
                if (formatter.size())
                {
                    value = formatter;
                }
            }
            else
            {
                // make sure formatter strings match
                std::string formatter = GetFormatter(string_id, "ERROR_STRING_FORMAT");
                const char *a = value.c_str();
                const char *b = formatter.c_str();
                bool is_invalid = false;
                for (;;)
                {
                    const char *ap = strchr(a, '%');
                    const char *bp = strchr(b, '%');
                    if ((ap != NULL) != (bp != NULL))
                    {
                        is_invalid = true;
                        break;
                    }

                    if (ap == NULL || bp == NULL)
                        break;

                    if (ap[1] != bp[1])
                    {
                        is_invalid = true;
                        break;
                    }

                    a = ap + 1;
                    b = bp + 1;
                }

                // fallback to default on bad string formatter
                if (is_invalid)
                {
                    value = formatter;
                }
            }

            // TW comes with parenthesis
            if (string_id == STRING_ID_IF_THIS_IS_NOT_YOU)
            {
                if (value.find('(') == std::string::npos &&
                    value.find("（") == std::string::npos)
                {
                    value = Sexy::StrFormat("(%s)", value.c_str());
                }
            }

            // TW string that isn't in other versions
            if (string_id == STRING_ID_WELCOME_TO_ZUMA1)
            {
                if (value.size() == 0)
                {
                    // don't recall function, that returns format string
                    const char *str_string_id = GetLocalizationIdString(STRING_ID_WELCOME_TO_ZUMA);
                    value = Sexy::gSexyAppBase->GetString(str_string_id, "");
                    value = Sexy::StrFormat("%s!", value.c_str());
                }
            }

            const auto Override = [&](int id, const char *str_string_id)
            {
                if (string_id == id)
                {
                    std::string s = Sexy::gSexyAppBase->GetString(str_string_id, "");
                    if (s.size())
                    {
                        value = s;
                    }
                }
            };
            
            // TW different string ids
            Override(STRING_ID_BIZ_DEV, "STRING_ID_BIZDEV");
            Override(STRING_ID_3D_HARDWARE_ACCELERATION, "STRING_ID_HARDWARE_ACC");
            Override(STRING_ID_LAST_LIFE, "STRING_ID_LASTLIFE");
            Override(STRING_ID_SUN, "STRING_ID_SG");

            STRINGS[string_id] = strdup(value.c_str());
        }
        result = STRINGS[string_id];
    }
    return result;
}
const char *Sexy::gSmallGauntletStages(int stage)
{
    return (stage == 0) ? LS(STRING_ID_RAB) : 
           (stage == 1) ? LS(STRING_ID_EAG) : 
           (stage == 2) ? LS(STRING_ID_JAG) : 
           (stage == 3) ? LS(STRING_ID_SUN) : "???";
}
const char *Sexy::gGauntletStages(int stage)
{
    return (stage == 0) ? LS(STRING_ID_RABBIT) : 
           (stage == 1) ? LS(STRING_ID_EAGLE) : 
           (stage == 2) ? LS(STRING_ID_JAGUAR) : 
           (stage == 3) ? LS(STRING_ID_SUNGOD) : "???";
}
