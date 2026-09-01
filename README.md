# Pong Game 🏓

## Overview
A classic arcade Pong game developed in the LabWindows/CVI environment using the C programming language[cite: 5]. Developed by Idan Nudel, the game features an interactive graphical interface, realistic ball physics, and real-time score management[cite: 5]. The core game logic is modularly separated into a dynamically linked library (DLL)[cite: 1, 5].

## Features
* **Game Modes:** Support for Player vs. Computer (AI) and Player vs. Player (1v1) modes[cite: 5].
* **AI Difficulty Levels:** The computer opponent can be set to Easy, Medium, Hard, or Impossible, which directly affects its reaction speed[cite: 1, 5].
* **Realistic Physics:** The ball features variable bounce angles based on the impact position relative to the paddle, includes random angle jitter, and gradually increases in speed[cite: 1, 5].
* **Custom GUI:** Panel-based user interface including a main menu, game mode selection, difficulty selection, and a victory screen[cite: 5].
* **Win Condition:** The game stops and declares a winner when a player reaches 5 points[cite: 5].

## Controls
* **Player 1 (Left Paddle):** `Up` and `Down` arrow keys[cite: 5].
* **Player 2 (Right Paddle):** `W` and `S` keys (in 1v1 mode)[cite: 5].
* **Interface:** On-screen buttons to Pause, Restart, or return to the Main Menu[cite: 3, 5].

## Project Structure
* `PongProject.c`: The main application file handling the game loop, canvas drawing, UI events, and real-time keyboard input management[cite: 3, 5].
* `pong_dll.c` / `pong_dll.h`: The DLL source and header files containing the core math and logic for ball movement, collision detection, and AI behavior[cite: 1, 2, 5].
* `PongProject.uir`: The LabWindows User Interface Resource file containing the definitions for all graphical panels and controls[cite: 4, 5].
* `Pong Project - Idan Nudel.pdf`: Complete project documentation and summary[cite: 5].

## How to Run
1. Download or clone the repository to your local machine.
2. Open the `PongProject.prj` workspace file using the LabWindows/CVI IDE[cite: 5].
3. Compile and run the project directly from the development environment.
