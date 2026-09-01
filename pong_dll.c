#define PONG_DLL_EXPORTS   
#include "pong_dll.h"
#include <stdlib.h>
#include <math.h>

// --- Constants ---
#define MAX_BOUNCE_ANGLE_DEG 75.0f
#define ANGLE_JITTER_DEG 7.0f
#define MIN_BOUNCE_ANGLE_CLAMP_DEG 15.0f
#define SPEED_INCREASE_FACTOR 1.05f
#define MIN_BALL_SPEED_PER_SEC 250.0f
#define MAX_BALL_SPEED_PER_SEC 1500.0f
#define INITIAL_BALL_SPEED_PER_SEC 500.0f

// --- Globals ---
// Initialize globals here with DLL_API so they are exported, not imported
DLL_API float ballX = WIDTH / 2.0f - BALL_SIZE / 2.0f;
DLL_API float ballY = HEIGHT / 2.0f - BALL_SIZE / 2.0f;
DLL_API float ballSpeedX = 0.0f;
DLL_API float ballSpeedY = 0.0f;

DLL_API float player1Y = HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
DLL_API float player2Y = HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;

DLL_API int playerScore = 0;
DLL_API int opponentScore = 0;

DLL_API int gameMode = 0;
DLL_API int difficulty = 1;

DLL_API int gameRunning = 0;
DLL_API int gamePaused = 0;

// --- Callbacks---
static VoidCallback DrawGameCallback = NULL;
static VoidCallback UpdateScoreCallback = NULL;
static VoidCallback HandleWinCallback = NULL;

// --- Math helper functions ---
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#ifndef copysignf
#define copysignf(x, y) ((y) < 0.0f ? -fabsf(x) : fabsf(x))
#endif

#ifndef fabsf
#define fabsf(x) ((float)fabs((double)(x)))
#endif

static float fmaxf(float a, float b) { return (a > b) ? a : b; }
static float fminf(float a, float b) { return (a < b) ? a : b; }
static float sqrtf(float x) { return (float)sqrt((double)x); }
static float cosf(float x) { return (float)cos((double)x); }
static float sinf(float x) { return (float)sin((double)x); }

// --- Functions ---

DLL_API void MoveBall(float deltaTime) {
    float currentBallSpeed = sqrtf(ballSpeedX * ballSpeedX + ballSpeedY * ballSpeedY);

    if (currentBallSpeed == 0.0f) {
        ResetBall();
        return;
    }

    float maxMovementPerFrame = PADDLE_WIDTH / 2.0f;
    if (currentBallSpeed * deltaTime > maxMovementPerFrame) {
        deltaTime = maxMovementPerFrame / currentBallSpeed;
    }

    ballX += ballSpeedX * deltaTime;
    ballY += ballSpeedY * deltaTime;

    if (ballY <= 0) {
        ballY = 0;
        ballSpeedY = -ballSpeedY;
    } else if (ballY >= HEIGHT - BALL_SIZE) {
        ballY = HEIGHT - BALL_SIZE;
        ballSpeedY = -ballSpeedY;
    }

    // Left paddle collision
    if (ballSpeedX < 0 &&
        ballX <= 30 + PADDLE_WIDTH && ballX + BALL_SIZE >= 30 &&
        ballY + BALL_SIZE >= player1Y && ballY <= player1Y + PADDLE_HEIGHT) {
        BallBounceAngle(player1Y, 30);
        ballX = 30 + PADDLE_WIDTH;
        ballSpeedX = copysignf(fabsf(ballSpeedX) + 5.0f, ballSpeedX);
    }
    // Right paddle collision
    else if (ballSpeedX > 0 &&
             ballX + BALL_SIZE >= WIDTH - 40 - PADDLE_WIDTH && ballX <= WIDTH - 40 &&
             ballY + BALL_SIZE >= player2Y && ballY <= player2Y + PADDLE_HEIGHT) {
        BallBounceAngle(player2Y, WIDTH - 40);
        ballX = WIDTH - 40 - PADDLE_WIDTH - BALL_SIZE;
        ballSpeedX = copysignf(fabsf(ballSpeedX) + 5.0f, ballSpeedX);
    }

    if (ballX < -BALL_SIZE) {
        opponentScore++;
        if (UpdateScoreCallback) UpdateScoreCallback();
        ResetBall();
    } else if (ballX > WIDTH) {
        playerScore++;
        if (UpdateScoreCallback) UpdateScoreCallback();
        ResetBall();
    }

    if ((playerScore >= WIN_SCORE || opponentScore >= WIN_SCORE) && HandleWinCallback) {
        HandleWinCallback();
    }
}

DLL_API void MoveAI(float deltaTime) {
    if (gameMode == 0 && gameRunning && !gamePaused) {
        static float aiDelayTimer = 0.0f;

        aiDelayTimer -= deltaTime;
        if (aiDelayTimer > 0.0f) return;

        switch (difficulty) {
            case 1: aiDelayTimer = 0.06f; break;
            case 2: aiDelayTimer = 0.05f; break;
            case 3: aiDelayTimer = 0.03f; break;
            case 4: aiDelayTimer = 0.01f; break;
            default: aiDelayTimer = 0.06f; break;
        }

        float aiPaddleSpeedPerSec;
        switch (difficulty) {
            case 1: aiPaddleSpeedPerSec = 420.0f; break;
            case 2: aiPaddleSpeedPerSec = 440.0f; break;
            case 3: aiPaddleSpeedPerSec = 460.0f; break;
            case 4: aiPaddleSpeedPerSec = 480.0f; break;
            default: aiPaddleSpeedPerSec = 420.0f; break;
        }

        float centerPaddleY = player2Y + PADDLE_HEIGHT / 2.0f;
        float diff = ballY - centerPaddleY;
        float aiMovement = aiPaddleSpeedPerSec * deltaTime;

        if (fabsf(diff) > aiMovement)
            player2Y += (diff > 0) ? aiMovement : -aiMovement;
        else
            player2Y += diff;

        player2Y = fmaxf(0.0f, fminf(HEIGHT - PADDLE_HEIGHT, player2Y));
    }
}

DLL_API void BallBounceAngle(float paddleY, int paddleX) {
    float hitPoint = (ballY + BALL_SIZE / 2.0f - (paddleY + PADDLE_HEIGHT / 2.0f)) / (PADDLE_HEIGHT / 2.0f);
    hitPoint = fmaxf(-1.0f, fminf(1.0f, hitPoint));

    float maxBounceAngleRad = MAX_BOUNCE_ANGLE_DEG * (float)M_PI / 180.0f;
    float bounceAngle = hitPoint * maxBounceAngleRad;

    float angleJitterRad = ((float)(rand() % ((int)(ANGLE_JITTER_DEG * 2) + 1)) - ANGLE_JITTER_DEG) * (float)M_PI / 180.0f;
    bounceAngle += angleJitterRad;

    float minVerticalAngleRad = MIN_BOUNCE_ANGLE_CLAMP_DEG * (float)M_PI / 180.0f;
    if (fabsf(bounceAngle) < minVerticalAngleRad)
        bounceAngle = copysignf(minVerticalAngleRad, bounceAngle);

    bounceAngle = fmaxf(-maxBounceAngleRad, fminf(maxBounceAngleRad, bounceAngle));

    float speed = sqrtf(ballSpeedX * ballSpeedX + ballSpeedY * ballSpeedY);
    speed *= SPEED_INCREASE_FACTOR;
    speed = fmaxf(MIN_BALL_SPEED_PER_SEC, fminf(MAX_BALL_SPEED_PER_SEC, speed));

    int direction = (paddleX < WIDTH / 2) ? 1 : -1;
    ballSpeedX = direction * speed * cosf(bounceAngle);
    ballSpeedY = speed * sinf(bounceAngle);
}

DLL_API void ResetBall(void) {
    ballX = WIDTH / 2.0f - BALL_SIZE / 2.0f;
    ballY = HEIGHT / 2.0f - BALL_SIZE / 2.0f;

    ballSpeedX = (playerScore > opponentScore) ? -INITIAL_BALL_SPEED_PER_SEC : INITIAL_BALL_SPEED_PER_SEC;

    do {
        ballSpeedY = (float)(rand() % (int)(2.0f * INITIAL_BALL_SPEED_PER_SEC * 100)) / 100.0f - INITIAL_BALL_SPEED_PER_SEC;
    } while (fabsf(ballSpeedY) < 50.0f);

    if (DrawGameCallback) DrawGameCallback();
}

DLL_API void SetGameCallbacks(VoidCallback drawFunc, VoidCallback updateScoreFunc, VoidCallback winFunc) {
    DrawGameCallback = drawFunc;
    UpdateScoreCallback = updateScoreFunc;
    HandleWinCallback = winFunc;
}
