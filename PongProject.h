/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  DIFFPANEL                        1
#define  DIFFPANEL_IMMPOSIBLE             2       /* control type: command, callback function: SetDifficultyImm */
#define  DIFFPANEL_HARD                   3       /* control type: command, callback function: SetDifficultyHard */
#define  DIFFPANEL_MEDIUM                 4       /* control type: command, callback function: SetDifficultyMed */
#define  DIFFPANEL_EASY                   5       /* control type: command, callback function: SetDifficultyEasy */

#define  GAMEPANEL                        2
#define  GAMEPANEL_GAME_CANVAS            2       /* control type: canvas, callback function: (none) */
#define  GAMEPANEL_BTN_RESTART_GAME       3       /* control type: command, callback function: RestartGameButton */
#define  GAMEPANEL_BTN_PAUSE              4       /* control type: command, callback function: PauseGame */
#define  GAMEPANEL_BTN_BACK_TO_MENU       5       /* control type: command, callback function: BackToMainMenu */
#define  GAMEPANEL_BTN_MODE               6       /* control type: command, callback function: SelectMode */
#define  GAMEPANEL_TIMER_GAME_LOOP        7       /* control type: timer, callback function: TimerCallback */
#define  GAMEPANEL_SCORE_PLAYER2          8       /* control type: numeric, callback function: (none) */
#define  GAMEPANEL_SCORE_PLAYER1          9       /* control type: numeric, callback function: (none) */

#define  MODEPANEL                        3
#define  MODEPANEL_VsPlayer               2       /* control type: command, callback function: ModeVsPlayer2 */
#define  MODEPANEL_VsCom                  3       /* control type: command, callback function: ModeVsAI */
#define  MODEPANEL_BTN_RESUME             4       /* control type: command, callback function: ResumeGame */
#define  MODEPANEL_BTN_BACK_TO_MENU       5       /* control type: command, callback function: BackToMainMenu */

#define  STARTPANEL                       4
#define  STARTPANEL_QUIT_BUTTON           2       /* control type: command, callback function: MyQuitStartPanel */
#define  STARTPANEL_START_GAME_BUTTON     3       /* control type: command, callback function: StartGameFromMenu */
#define  STARTPANEL_PICTURE               4       /* control type: picture, callback function: (none) */
#define  STARTPANEL_DECORATION            5       /* control type: deco, callback function: (none) */

#define  WINPANEL                         5
#define  WINPANEL_WinnerName              2       /* control type: string, callback function: (none) */
#define  WINPANEL_BTN_RESTART_GAME        3       /* control type: command, callback function: RestartGameButton */
#define  WINPANEL_BTN_BACK_TO_MENU        4       /* control type: command, callback function: BackToMainMenu */
#define  WINPANEL_BTN_MODE                5       /* control type: command, callback function: SelectMode */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BackToMainMenu(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ModeVsAI(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ModeVsPlayer2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MyQuitStartPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PauseGame(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RestartGameButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ResumeGame(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SelectMode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetDifficultyEasy(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetDifficultyHard(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetDifficultyImm(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetDifficultyMed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StartGameFromMenu(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
