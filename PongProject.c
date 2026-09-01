#include <cvirte.h>		
#include <userint.h>
#include "PongProject.h"
#include "pong_dll.h"   
#include <ansi_c.h>
#include <utility.h>
#include <formatio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>



#define PADDLE_SPEED_PER_SEC 500.0f 
#define WIN_SCORE 5 
// --- Global Handles ---
static int startPanelHandle = -1;
static int gamePanelHandle = -1;
static int modePanelHandle = -1;
static int diffPanelHandle = -1;
static int winPanelHandle = -1; 
static char playerName1[32] = "Player 1";
static char playerName2[32] = "Player 2";
static int keys[256] = {0};
static double lastKeyPressedTime[256] = {0};

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_W 3
#define KEY_S 4
#define KEY_ENTER 5
#define KEY_ESC 6

static double lastFrameTime = 0.0;
static float aiReactionDelayTimer = 0.0f;

// --- Function Prototypes ---
void DrawGame(void);
void UpdateScoreDisplay(void);
void HandleWin(void);
void RestartGame(void);



// --- CVI Callback Prototypes ---
int CVICALLBACK TimerCallback(int panel, int control,
                              int event, void *callbackData, int eventData1, int eventData2);
int CVICALLBACK StartGameFromMenu(int panel, int control, int event,
                                  void *callbackData, int eventData1, int eventData2);
int CVICALLBACK OpenSettingsFromMenu(int panel, int control, int event,
                                     void *callbackData, int eventData1, int eventData2);
int CVICALLBACK QuitFromMenu(int panel, int control, int event,
                             void *callbackData, int eventData1, int eventData2);
int CVICALLBACK RestartGameButton(int panel, int control, int event,
                                  void *callbackData, int eventData1, int eventData2);
int CVICALLBACK PauseGame(int panel, int control, int event,
                          void *callbackData, int eventData1, int eventData2);
int CVICALLBACK SelectMode(int panel, int control, int event,
                           void *callbackData, int eventData1, int eventData2);
int CVICALLBACK BackToMainMenu(int panel, int control, int event,
                               void *callbackData, int eventData1, int eventData2);
int CVICALLBACK MyQuitStartPanel(int panel, int control, int event,
                                void *callbackData, int eventData1, int eventData2);

// --- Math Functions ---
static inline float fmaxf(float a, float b) { return a > b ? a : b; }
static inline float fminf(float a, float b) { return a < b ? a : b; }

#ifndef EVENT_KEYRELEASE
#define EVENT_KEYRELEASE 4
#endif





//============================Game Drawing=========================================================
void DrawGame(void) {
    // Fill background with black
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_PEN_COLOR, VAL_BLACK);
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_BLACK);
    CanvasDrawRect(gamePanelHandle, GAMEPANEL_GAME_CANVAS,
                   MakeRect(0, 0, HEIGHT, WIDTH), VAL_DRAW_FRAME_AND_INTERIOR);

    // Draw outer boundary box (field of play)
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_PEN_COLOR, VAL_WHITE);
    CanvasDrawRect(gamePanelHandle, GAMEPANEL_GAME_CANVAS,
                   MakeRect(0, 0, HEIGHT, WIDTH), VAL_DRAW_FRAME);

    // Draw center line in white
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_PEN_COLOR, VAL_WHITE);
    CanvasDrawLine(gamePanelHandle, GAMEPANEL_GAME_CANVAS,
                   MakePoint(WIDTH / 2, 0), MakePoint(WIDTH / 2, HEIGHT));

    // Set white fill and outline for paddles and ball
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_PEN_COLOR, VAL_WHITE);
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_WHITE);

    // Draw Player 1 paddle
    CanvasDrawRect(gamePanelHandle, GAMEPANEL_GAME_CANVAS,
                   MakeRect((int)(player1Y + 0.5f), 30, PADDLE_HEIGHT, PADDLE_WIDTH),
                   VAL_DRAW_FRAME_AND_INTERIOR);

    // Draw Player 2 paddle
    CanvasDrawRect(gamePanelHandle, GAMEPANEL_GAME_CANVAS,
                   MakeRect((int)(player2Y + 0.5f), WIDTH - 40, PADDLE_HEIGHT, PADDLE_WIDTH),
                   VAL_DRAW_FRAME_AND_INTERIOR);

    // Draw ball
    CanvasDrawOval(gamePanelHandle, GAMEPANEL_GAME_CANVAS,
                   MakeRect((int)(ballY + 0.5f), (int)(ballX + 0.5f), BALL_SIZE, BALL_SIZE),
                   VAL_DRAW_FRAME_AND_INTERIOR);

    UpdateScoreDisplay();
}


//===========================Game Timer==========================================================

 int CVICALLBACK TimerCallback(int panel, int control,
                             int event, void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_TIMER_TICK) {

        if (lastFrameTime == 0.0) {
            lastFrameTime = Timer();
        }

        double currentTime = Timer();
        float deltaTime = (float)(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        deltaTime = fminf(deltaTime, 0.033f); 

        // Auto-reset keys[] if too much time has passed since keypress
        for (int i = 0; i < 256; ++i) {
            if (keys[i] == 1 && (currentTime - lastKeyPressedTime[i]) > 0.15)  // 150 ms
            {
                keys[i] = 0;
            }
        }

        if (gameRunning && !gamePaused) {
            float paddleMovementDelta = PADDLE_SPEED_PER_SEC * deltaTime;

            if (keys[KEY_UP])
                player1Y = fmaxf(0.0f, player1Y - paddleMovementDelta);
            if (keys[KEY_DOWN])
                player1Y = fminf(HEIGHT - PADDLE_HEIGHT, player1Y + paddleMovementDelta);

            if (gameMode == 1) {
                if (keys[KEY_W])
                    player2Y = fmaxf(0.0f, player2Y - paddleMovementDelta);
                if (keys[KEY_S])
                    player2Y = fminf(HEIGHT - PADDLE_HEIGHT, player2Y + paddleMovementDelta);
            }

            MoveBall(deltaTime);  // DLL function
            MoveAI(deltaTime);    // DLL function
            DrawGame();
			if (playerScore >= WIN_SCORE || opponentScore >= WIN_SCORE)
       		 HandleWin();
        }
    }
    return 0;
}


//=============================================================================================


int CVICALLBACK GamePanelEventHandler(int panel, int event, void *callbackData, int eventData1, int eventData2)
{


if (event == EVENT_KEYPRESS || event == EVENT_KEYRELEASE){
    int isPressed = (event == EVENT_KEYPRESS) ? 1 : 0;

    switch (eventData1) {
        case VAL_UP_ARROW_VKEY:
            keys[KEY_UP] = isPressed;
            if (isPressed) lastKeyPressedTime[KEY_UP] = Timer();  // update on press only
            break;

        case VAL_DOWN_ARROW_VKEY:
            keys[KEY_DOWN] = isPressed;
            if (isPressed) lastKeyPressedTime[KEY_DOWN] = Timer();
            break;

        case 'W':
        case 'w':
            keys[KEY_W] = isPressed;
            if (isPressed) lastKeyPressedTime[KEY_W] = Timer();
            break;

        case 'S':
        case 's':
            keys[KEY_S] = isPressed;
            if (isPressed) lastKeyPressedTime[KEY_S] = Timer();
            break;
    }
}
return 0; 
}


//===============================Modes & Menu===================================================


int CVICALLBACK StartGameFromMenu(int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT)
	{
        HidePanel(startPanelHandle);
        DisplayPanel(modePanelHandle);
    }
    return 0;
}



int CVICALLBACK SelectMode (int panel, int control, int event,
                            void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            gamePaused = 1;
            SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 0); // Stop the game loop timer
            HidePanel(winPanelHandle);
			DisplayPanel(modePanelHandle);  // Show the mode selection panel
            break;
    }
    return 0;
}


int CVICALLBACK ModeVsAI(int panel, int control, int event,
                        void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) {
		gameMode = 0;
        HidePanel(modePanelHandle);
        DisplayPanel(diffPanelHandle);
		SetActivePanel(diffPanelHandle); 
        
        
    }
    return 0;
}


int CVICALLBACK ModeVsPlayer2(int panel, int control, int event,
                             void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) {
        gameMode = 1; // Vs Player 2
        HidePanel(modePanelHandle);
        DisplayPanel(gamePanelHandle);
		SetActivePanel(gamePanelHandle); 
        RestartGame();
        SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 1);
    }
    return 0;
}


//=====================Difficulty Settings====================================================== 

int CVICALLBACK SetDifficultyEasy (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) {
		
        difficulty = 1;
		HidePanel(diffPanelHandle);
		DisplayPanel(gamePanelHandle);
		SetActivePanel(gamePanelHandle); 
        RestartGame();
        SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 1);
    }
	return 0;
}

int CVICALLBACK SetDifficultyMed (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) {
		
        difficulty = 2;
		HidePanel(diffPanelHandle);
		DisplayPanel(gamePanelHandle);
		SetActivePanel(gamePanelHandle); 
        RestartGame();
        SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 1);
    }
	return 0;
}
int CVICALLBACK SetDifficultyHard (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) {
		
        difficulty = 3;
		HidePanel(diffPanelHandle);
		DisplayPanel(gamePanelHandle);
		SetActivePanel(gamePanelHandle); 
        RestartGame();
        SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 1);
    }
	return 0;
}
int CVICALLBACK SetDifficultyImm (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) {
		
        difficulty = 4;
		HidePanel(diffPanelHandle);
		DisplayPanel(gamePanelHandle);
		SetActivePanel(gamePanelHandle); 
        RestartGame();
        SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 1);
    }
	return 0;
}


//=============================In Game Functions============================================= 

void UpdateScoreDisplay(void) {
    int result1 = SetCtrlVal(gamePanelHandle, GAMEPANEL_SCORE_PLAYER1, playerScore);
    int result2 = SetCtrlVal(gamePanelHandle, GAMEPANEL_SCORE_PLAYER2, opponentScore);
	

    if (result1 < 0 || result2 < 0) {
        MessagePopup("UI Error", "Failed to update one or both score controls!");
    }
}

void HandleWin(void) {
    gameRunning = 0;
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 0);

    char msg[128];
    char *winnerName = (playerScore >= WIN_SCORE) ? playerName1 : playerName2;
    sprintf(msg, "%s WON!", winnerName);  

    SetCtrlVal(winPanelHandle, WINPANEL_WinnerName, msg);  
    DisplayPanel(winPanelHandle);
}




//============================In Game Buttons================================================

void RestartGame(void) {
	HidePanel(winPanelHandle);
    playerScore = 0;
    opponentScore = 0;
    player1Y = HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
    player2Y = HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
    ResetBall();
    gameRunning = 1;
    gamePaused = 0;
    
    int result = SetCtrlAttribute(gamePanelHandle, GAMEPANEL_BTN_PAUSE, ATTR_LABEL_TEXT, "Pause");

    if (result < 0) MessagePopup("UI Error", "Failed to reset pause button!");

    result = SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 1);
    if (result < 0) MessagePopup("UI Error", "Failed to enable game timer!");

    UpdateScoreDisplay();
    lastFrameTime = 0.0;
    aiReactionDelayTimer = 0.0f;
}

int CVICALLBACK PauseGame(int panel, int control, int event,
                         void *callbackData, int eventData1, int eventData2) {
    if (event == EVENT_COMMIT && gameRunning) {
        gamePaused = !gamePaused;
        int result;
        if (gamePaused) {
            result = SetCtrlAttribute(gamePanelHandle, GAMEPANEL_BTN_PAUSE, ATTR_LABEL_TEXT, "Resume");

			
            if (result < 0) MessagePopup("UI Error", "Failed to set Resume button!");
            result = SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 0);
            if (result < 0) MessagePopup("UI Error", "Failed to disable game timer!");
        } else {
            result = SetCtrlAttribute(gamePanelHandle, GAMEPANEL_BTN_PAUSE, ATTR_LABEL_TEXT, "Pause");
            if (result < 0) MessagePopup("UI Error", "Failed to set Pause button!");
            result = SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 1);
            if (result < 0) MessagePopup("UI Error", "Failed to enable game timer!");
        }
        DrawGame();
    }
    return 0;
}

int CVICALLBACK BackToMainMenu(int panel, int control, int event,
                              void *callbackData, int eventData1, int eventData2) {
    if (event == EVENT_COMMIT) {
        gameRunning = 0;
        gamePaused = 0;
        SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 0);
        HidePanel(gamePanelHandle);
		HidePanel(winPanelHandle);
        DisplayPanel(startPanelHandle);
        lastFrameTime = 0.0;
        aiReactionDelayTimer = 0.0f;
    }
    return 0;
}

int CVICALLBACK ResumeGame (int panel, int control, int event,
                            void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if (!gameRunning) {
                MessagePopup("Info", "No active game to resume.");
                return 0;  // Do nothing if no active game
            }
            HidePanel(modePanelHandle); 
			HidePanel(winPanelHandle);
            gamePaused = 0;        // resume game
			SetCtrlAttribute(gamePanelHandle, GAMEPANEL_BTN_PAUSE, ATTR_LABEL_TEXT, "Pause");
            SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 1); // re-enable game loop timer
            break;
    }
    return 0;
}

int CVICALLBACK RestartGameButton(int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2) {
    if (event == EVENT_COMMIT) {
        RestartGame();
    }
    return 0;
}


//================================MAIN=======================================================

int main(int argc, char *argv[]) {
    if (InitCVIRTE(0, argv, 0) == 0)
        return -1;

    srand((unsigned int)time(NULL));
    memset(keys, 0, sizeof(keys));

    // Load Panels
    startPanelHandle = LoadPanel(0, "PongProject.uir", STARTPANEL);
    if (startPanelHandle < 0) {
        MessagePopup("Error", "Failed to load STARTPANEL!");
        return -1;
    }

    gamePanelHandle = LoadPanel(0, "PongProject.uir", GAMEPANEL);
    if (gamePanelHandle < 0) {
        MessagePopup("Error", "Failed to load GAMEPANEL!");
        DiscardPanel(startPanelHandle);
        return -1;
    }

    modePanelHandle = LoadPanel(0, "PongProject.uir", MODEPANEL);
    if (modePanelHandle < 0) {
        MessagePopup("Error", "Failed to load MODEPANEL!");
        DiscardPanel(startPanelHandle);
        DiscardPanel(gamePanelHandle);
        return -1;
    }

    diffPanelHandle = LoadPanel(0, "PongProject.uir", DIFFPANEL);
    if (diffPanelHandle < 0) {
        MessagePopup("Error", "Failed to load DIFFPANEL!");
        DiscardPanel(startPanelHandle);
        DiscardPanel(gamePanelHandle);
        DiscardPanel(modePanelHandle);
        return -1;
    }
	
	winPanelHandle = LoadPanel(0, "PongProject.uir", WINPANEL);
	if (winPanelHandle < 0) {
	    MessagePopup("Error", "Failed to load WINPANEL!");
	    return -1;
	}


    // Canvas Setup
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_WIDTH, 800);
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_HEIGHT, 600);
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_TOP, 0);
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_LEFT, 0);
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_GAME_CANVAS, ATTR_CTRL_MODE, VAL_HOT);
    SetActiveCtrl(gamePanelHandle, GAMEPANEL_GAME_CANVAS);

    // Timer Setup
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_INTERVAL, 0.020);
    SetCtrlAttribute(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, ATTR_ENABLED, 0);

    // Install Callbacks
    InstallCtrlCallback(startPanelHandle, STARTPANEL_START_GAME_BUTTON, StartGameFromMenu, 0);
    InstallCtrlCallback(startPanelHandle, STARTPANEL_QUIT_BUTTON, MyQuitStartPanel, 0);

    InstallCtrlCallback(modePanelHandle, MODEPANEL_VsCom, ModeVsAI, 0);
    InstallCtrlCallback(modePanelHandle, MODEPANEL_VsPlayer, ModeVsPlayer2, 0);
    InstallCtrlCallback(modePanelHandle, MODEPANEL_BTN_RESUME, ResumeGame, 0);
    InstallCtrlCallback(modePanelHandle, MODEPANEL_BTN_BACK_TO_MENU, BackToMainMenu, 0);

    InstallCtrlCallback(gamePanelHandle, GAMEPANEL_TIMER_GAME_LOOP, TimerCallback, 0);
    InstallCtrlCallback(gamePanelHandle, GAMEPANEL_BTN_RESTART_GAME, RestartGameButton, 0);
    InstallCtrlCallback(gamePanelHandle, GAMEPANEL_BTN_PAUSE, PauseGame, 0);
    InstallCtrlCallback(gamePanelHandle, GAMEPANEL_BTN_BACK_TO_MENU, BackToMainMenu, 0);
    InstallPanelCallback(gamePanelHandle, GamePanelEventHandler, 0);

	InstallCtrlCallback(winPanelHandle, WINPANEL_BTN_RESTART_GAME, RestartGameButton, 0);
	InstallCtrlCallback(winPanelHandle, WINPANEL_BTN_BACK_TO_MENU, BackToMainMenu, 0);
	InstallCtrlCallback(winPanelHandle, WINPANEL_BTN_MODE, SelectMode, 0);

    // Show initial panel
    DisplayPanel(startPanelHandle);

    RunUserInterface();

    // Cleanup
    DiscardPanel(startPanelHandle);
    DiscardPanel(modePanelHandle);
    DiscardPanel(gamePanelHandle);
    DiscardPanel(diffPanelHandle);
	DiscardPanel(winPanelHandle);

    return 0;
}


//=============================================================================================
int CVICALLBACK MyQuitStartPanel(int panel, int control, int event, 
								 void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_COMMIT) {
        QuitUserInterface(0);
    }
    return 0;
}

//============================================================================================= 

