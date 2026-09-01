#ifndef PONG_DLL_H
#define PONG_DLL_H

#include <ansi_c.h>
#include <cvidef.h>

// Define DLL_API depending on whether building or using DLL
#ifdef PONG_DLL_EXPORTS
    #define DLL_API __declspec(dllexport)
#else
    #define DLL_API __declspec(dllimport)
#endif

#define HEIGHT 600
#define WIDTH 800
#define PADDLE_HEIGHT 100
#define PADDLE_WIDTH 10
#define BALL_SIZE 10
#define WIN_SCORE 5

// Declare globals with DLL_API, no initialization here
extern DLL_API float ballX;
extern DLL_API float ballY;
extern DLL_API float ballSpeedX;
extern DLL_API float ballSpeedY;

extern DLL_API float player1Y;
extern DLL_API float player2Y;

extern DLL_API int playerScore;
extern DLL_API int opponentScore;

extern DLL_API int gameMode;
extern DLL_API int difficulty;

extern DLL_API int gameRunning;
extern DLL_API int gamePaused;

// Function declarations
DLL_API void MoveBall(float deltaTime);
DLL_API void MoveAI(float deltaTime);
DLL_API void BallBounceAngle(float paddleY, int paddleX);
DLL_API void ResetBall(void);

typedef void (*VoidCallback)(void);
DLL_API void SetGameCallbacks(VoidCallback drawFunc, VoidCallback updateScoreFunc, VoidCallback winFunc);

#endif // PONG_DLL_H
