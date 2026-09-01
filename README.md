# Pong Game 🏓

## Overview
A classic arcade Pong game developed in the LabWindows/CVI environment using the C programming language. Developed by Idan Nudel, the game features an interactive graphical interface, realistic ball physics, and real-time score management. The core game logic is modularly separated into a dynamically linked library (DLL).

## Features
* **Game Modes:** Support for Player vs. Computer (AI) and Player vs. Player (1v1) modes.
* **AI Difficulty Levels:** The computer opponent can be set to Easy, Medium, Hard, or Impossible, which directly affects its reaction speed.
* **Realistic Physics:** The ball features variable bounce angles based on the impact position relative to the paddle, includes random angle jitter, and gradually increases in speed.
* **Custom GUI:** Panel-based user interface including a main menu, game mode selection, difficulty selection, and a victory screen.
* **Win Condition:** The game stops and declares a winner when a player reaches 5 points.

## Controls
* **Player 1 (Left Paddle):** `Up` and `Down` arrow keys.
* **Player 2 (Right Paddle):** `W` and `S` keys (in 1v1 mode).
* **Interface:** On-screen buttons to Pause, Restart, or return to the Main Menu.

## Project Structure
* `PongProject.c`: The main application file handling the game loop, canvas drawing, UI events, and real-time keyboard input management.
* `pong_dll.c` / `pong_dll.h`: The DLL source and header files containing the core math and logic for ball movement, collision detection, and AI behavior.
* `PongProject.uir`: The LabWindows User Interface Resource file containing the definitions for all graphical panels and controls.
* `Pong Project - Idan Nudel.pdf`: Complete project documentation and summary.

## How to Run
1. Download or clone the repository to your local machine.
2. Open the `PongProject.prj` workspace file using the LabWindows/CVI IDE.
3. Compile and run the project directly from the development environment.
