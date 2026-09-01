# Pong Game – LabWindows/CVI

A classic Pong game developed in **C using LabWindows/CVI**, with modular game logic separated into a dynamically linked library (DLL).

The project was developed as an academic programming project and focuses on event-driven programming, game-state management, collision handling, keyboard input, GUI interaction, and modular C software design.

## Features

- Player vs. Computer and Player vs. Player game modes
- Four AI difficulty levels: Easy, Medium, Hard, and Impossible
- Collision-based ball movement and paddle interaction
- Variable bounce angles based on paddle impact position
- Gradual increase in ball speed during gameplay
- Main menu, game mode selection, difficulty selection, and victory screen
- Pause, restart, and return-to-menu controls
- First player to reach 5 points wins

## Software Architecture

The project separates the user interface and application flow from the core gameplay logic.

```text
LabWindows/CVI Application
        |
        |-- UI Panels and Events
        |-- Keyboard Input
        |-- Game State Management
        |-- Canvas Rendering
        |
        v
      Game DLL
        |
        |-- Ball Movement
        |-- Collision Detection
        |-- AI Paddle Logic
        |-- Gameplay Calculations
